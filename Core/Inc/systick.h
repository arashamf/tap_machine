#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif
//Includes -------------------------------------------------------------------------------//
#include "main.h"

//Exported types -----------------------------------------------------------------//
typedef void (*SysTick_CALLBACK)(uint32_t); //указатель типа void на ф-ю

//Prototypes-------------------------------------------------------------------------------//
void SysTick_Init(SysTick_CALLBACK );
void SysTick_Callback(void);
uint32_t Get_SysTick(void);
void delay_ms	(uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif