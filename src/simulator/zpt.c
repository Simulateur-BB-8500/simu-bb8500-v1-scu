/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#include "zpt.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw4.h"
#include "stdint.h"

/*** ZPT local structures ***/

typedef enum {
	ZPT_STATE_0,
	ZPT_STATE_AR,
	ZPT_STATE_ARAV,
	ZPT_STATE_AV
} ZPT_State;

typedef struct {
	SW4_context_t sw4;
	ZPT_State state;
} ZPT_context_t;

/*** ZPT external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** ZPT local global variables ***/

static ZPT_context_t zpt_ctx;

/*** ZPT functions ***/

/* INIT ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_init(void) {
	// Init GPIOs.
	SW4_init(&zpt_ctx.sw4, &GPIO_ZPT, 500);
	zpt_ctx.state = ZPT_STATE_0;
	GPIO_configure(&GPIO_VLG, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_write(&GPIO_VLG, 1);
	// Init global context.
	lsmcu_ctx.zpt_raised = 0;
}

/* UPDATE THE VOLTAGE READ ON ZPT SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void ZPT_set_voltage_mv(uint32_t zpt_voltage_mv) {
	SW4_set_voltage_mv(&zpt_ctx.sw4, zpt_voltage_mv);
}

/* MAIN ROUTINE OF ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_task(void) {
	// Update selector state.
	SW4_update_state(&zpt_ctx.sw4);
	// Perform state machine.
	switch (zpt_ctx.state) {
	case ZPT_STATE_0:
		if (lsmcu_ctx.bl_unlocked != 0) {
			switch (zpt_ctx.sw4.state) {
			case SW4_P0:
				// Nothing to do.
				break;
			case SW4_P1:
				// Rise back pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_UP);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Rise both pantographs.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_UP);
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_ARAV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P3:
				// Rise front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_AV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			default:
				break;
			}
		}
		break;
	case ZPT_STATE_AR:
		if (lsmcu_ctx.bl_unlocked != 0) {
			switch (zpt_ctx.sw4.state) {
			case SW4_P0:
				// Lower back pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 1);
				break;
			case SW4_P1:
				// Nothing to do.
				break;
			case SW4_P2:
				// Rise front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_ARAV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P3:
				// Lower back and raise front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_AV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			}
		}
		else {
			// Disable ZPT.
			LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
			zpt_ctx.state = ZPT_STATE_0;
			lsmcu_ctx.zpt_raised = 0;
			GPIO_write(&GPIO_VLG, 1);
		}
		break;
	case ZPT_STATE_ARAV:
		if (lsmcu_ctx.bl_unlocked != 0) {
			switch (zpt_ctx.sw4.state) {
			case SW4_P0:
				// Lower both pantographs.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P1:
				// Lower back and raise front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Nothing to do.
				break;
			case SW4_P3:
				// Lower back pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
				zpt_ctx.state = ZPT_STATE_AV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			}
		}
		else {
			// Disable ZPT.
			LSSGIU_Send(LSMCU_OUT_ZPT_BACK_DOWN);
			LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
			zpt_ctx.state = ZPT_STATE_0;
			lsmcu_ctx.zpt_raised = 0;
			GPIO_write(&GPIO_VLG, 1);
		}
		break;
	case ZPT_STATE_AV:
		if (lsmcu_ctx.bl_unlocked != 0) {
			switch (zpt_ctx.sw4.state) {
			case SW4_P0:
				// Lower front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 1);
				break;
			case SW4_P1:
				// Rise back and lower front pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_UP);
				LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Rise back pantograph.
				LSSGIU_Send(LSMCU_OUT_ZPT_BACK_UP);
				zpt_ctx.state = ZPT_STATE_ARAV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P3:
				// Nothing to do.
				break;
			}
		}
		else {
			// Disable ZPT.
			LSSGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
			zpt_ctx.state = ZPT_STATE_0;
			lsmcu_ctx.zpt_raised = 0;
			GPIO_write(&GPIO_VLG, 1);
		}
		break;
	default:
		// Unknown state.
		zpt_ctx.state = ZPT_STATE_0;
		break;
	}
}
