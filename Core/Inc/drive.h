
#ifndef __DRIVE_H__
#define __DRIVE_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Defines ---------------------------------------------------------------------//

// Prototypes ------------------------------------------------------------------//
void drive_init ( turn_data_t * HandleTurnData );
void turn_drive_soft_start (turn_data_t * );
void turn_drive_stop (void);
void main_loop (encoder_data_t * , turn_data_t * );

// Variables ------------------------------------------------------------------//
extern volatile uint8_t drive_status;
#ifdef __cplusplus
}
#endif

#endif 

