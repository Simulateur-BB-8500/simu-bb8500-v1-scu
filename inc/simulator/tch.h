/*
 * tch.h
 *
 *  Created on: 15 july 2018
 *      Author: Ludovic
 */

#ifndef SIMULATOR_TACHRO_H
#define SIMULATOR_TACHRO_H

/*** TACHRO macros ***/

// Maximum speed displayed on the Tachro (in km/h).
#define TCH_SPEED_MAX_KMH	160

/*** TACHRO functions ***/

void TCH_Init(void);
void TCH_SetSpeed(unsigned char new_speed_kmh);
void TCH_Routine(void);

#endif /* SIMULATOR_TACHRO_H */
