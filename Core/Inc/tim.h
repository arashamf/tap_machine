/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define 		PWM_TIM								TIM14
#define 		ENC_TIM 							TIM3

#define			TIM_DELAY							TIM16
#define 		TIM_DELAY_APB1_BIT 		LL_APB1_GRP2_PERIPH_TIM16

#define			TIM_BOUNCE						TIM17
#define 		TIM_BOUNCE_APB1_BIT		LL_APB1_GRP2_PERIPH_TIM17 
#define			TIM_BOUNCE_IRQn				TIM17_IRQn
#define			TIM_BOUNCE_IRQHandler	TIM17_IRQHandler

//Exported types -----------------------------------------------------------------//
typedef void * xTimerHandle; //��������� ���� void �� ��������� �������

//---------------------------------------------------------	
typedef void (*tmrTIMER_CALLBACK)(xTimerHandle xTimer);	//��������� �� ������� ������������ �������

//---------------------------------------------------------	
typedef uint32_t (*portTickType)(void);

//---------------------------------------------------------
typedef enum 
{
	__IDLE = 0, 
	__ACTIVE, 
	__DONE
} tmrTIMER_STATE;

//---------------------------------------------------------
typedef struct
{
	uint32_t expiry;
  uint32_t periodic;
	tmrTIMER_STATE State;
	FunctionalState	AutoReload;
	tmrTIMER_CALLBACK	CallbackFunction;
} xTIMER;	
/* USER CODE END Private defines */

void MX_TIM14_Init(void);

/* USER CODE BEGIN Prototypes */
void drive_PWM_start (uint32_t ) ;
void drive_PWM_mod (uint32_t ) ;
void drive_PWM_stop (void) ;
void delay_us(uint16_t );
void repeat_time (uint16_t );

void xTimer_Init(uint32_t (*GetSysTick)(void));
xTimerHandle xTimer_Create(uint32_t xTimerPeriodInTicks, FunctionalState AutoReload, 
													tmrTIMER_CALLBACK CallbackFunction, FunctionalState NewState);
void xTimer_SetPeriod(xTimerHandle xTimer, uint32_t xTimerPeriodInTicks);
void xTimer_Reload(xTimerHandle xTimer);
void xTimer_Delete(xTimerHandle xTimer);
void xTimer_Task(uint32_t portTick);

void timers_init (void);
void xTimersBounceSensor_Reload (uint16_t	);
void xTimerBounceBtn_Reload (uint16_t );
//Variables -----------------------------------------------------------------------//

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

