
#ifndef __ENCODER_H__
#define __ENCODER_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Defines ---------------------------------------------------------------------//

// Prototypes ------------------------------------------------------------------//
void encoder_reset (encoder_data_t * );
int32_t read_enc_data (encoder_data_t * , turn_data_t * );

// Variables ------------------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

