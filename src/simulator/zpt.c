/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#include "zpt.h"

#include "adc.h"
#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
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
	SW4_init(&zpt_ctx.sw4, &GPIO_ZPT, 500, (uint32_t*) &(lsmcu_ctx.adc_data[ADC_DATA_INDEX_ZPT]));
	zpt_ctx.state = ZPT_STATE_0;
	GPIO_configure(&GPIO_VLG, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_write(&GPIO_VLG, 1);
	// Init global context.
	lsmcu_ctx.zpt_raised = 0;
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
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_UP);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Rise both pantographs.
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_UP);
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_ARAV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P3:
				// Rise front pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
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
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 1);
				break;
			case SW4_P1:
				// Nothing to do.
				break;
			case SW4_P2:
				// Rise front pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_ARAV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P3:
				// Lower back and raise front pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_UP);
				zpt_ctx.state = ZPT_STATE_AV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			}
		}
		else {
			// Disable ZPT.
			LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
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
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P1:
				// Lower back and raise front pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Nothing to do.
				break;
			case SW4_P3:
				// Lower back pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
				zpt_ctx.state = ZPT_STATE_AV;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			}
		}
		else {
			// Disable ZPT.
			LSAGIU_Send(LSMCU_OUT_ZPT_REAR_DOWN);
			LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
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
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_0;
				lsmcu_ctx.zpt_raised = 0;
				GPIO_write(&GPIO_VLG, 1);
				break;
			case SW4_P1:
				// Rise back and lower front pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_UP);
				LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
				zpt_ctx.state = ZPT_STATE_AR;
				lsmcu_ctx.zpt_raised = 1;
				GPIO_write(&GPIO_VLG, 0);
				break;
			case SW4_P2:
				// Rise back pantograph.
				LSAGIU_Send(LSMCU_OUT_ZPT_REAR_UP);
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
			LSAGIU_Send(LSMCU_OUT_ZPT_FRONT_DOWN);
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
