/*
 * tch.h
 *
 *  Created on: 15 jul. 2018
 *      Author: Ludo
 */

#ifndef TCH_H
#define TCH_H

/*** TACHRO macros ***/

// Maximum speed displayed on the Tachro (in km/h).
#define TCH_SPEED_MAX_KMH	160

/*** TACHRO functions ***/

void TCH_Init(void);
void TCH_Task(void);

#endif /* TCH_H */
