/*
 * tch.h
 *
 *  Created on: 15 jul. 2018
 *      Author: Ludo
 */

#ifndef __TCH_H__
#define __TCH_H__

/*** TACHRO macros ***/

// Maximum speed displayed on the Tachro (in km/h).
#define TCH_SPEED_MAX_KMH	160

/*** TACHRO functions ***/

void TCH_init(void);
void TCH_task(void);

#endif /* __TCH_H__ */
