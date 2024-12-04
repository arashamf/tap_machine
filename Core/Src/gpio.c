/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "typedef.h"
#include "tim.h"
#include "drive.h"
#include "usart.h"

#include <stdio.h>
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(DIR_GPIO_Port, DIR_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ENABLE_GPIO_Port, ENABLE_Pin);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTF, LL_SYSCFG_EXTI_LINE0);

  /**/
  LL_GPIO_SetPinPull(SENSOR_GPIO_Port, SENSOR_Pin, LL_GPIO_PULL_DOWN);

  /**/
  LL_GPIO_SetPinMode(SENSOR_GPIO_Port, SENSOR_Pin, LL_GPIO_MODE_INPUT);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DIR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(DIR_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ENABLE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(ENABLE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI0_1_IRQn, 0);
  NVIC_EnableIRQ(EXTI0_1_IRQn);

}

/* USER CODE BEGIN 2 */
//------------------------------------------------------------------------------------------//
void Select_Drive_Direction (uint8_t dir)
{
	if (dir == DIRECT)
	{	LL_GPIO_SetOutputPin (DIR_GPIO_Port, DIR_Pin);	}
	else
	{
		if (dir == REVERSE)
		{	LL_GPIO_ResetOutputPin(DIR_GPIO_Port, DIR_Pin);	}
	}
}

//------------------------------------------------------------------------------------------//
void Enable_Drive (uint8_t command)
{
	if (command == ON)
	{	LL_GPIO_ResetOutputPin (ENABLE_GPIO_Port, ENABLE_Pin);	}
	else
	{
		if (command == OFF)
		{	LL_GPIO_SetOutputPin(ENABLE_GPIO_Port, ENABLE_Pin);	}
	}
}

//----------------------------------------------------------------------------------------------------//
void Sensor_Callback(void)
{
	if (drive_status < DRIVE_REVERSE_CONTINUE)
	{ drive_status++;	}
	else
	{	drive_status = DRIVE_OFF;	}
		
	NVIC_DisableIRQ(EXTI0_1_IRQn);
	repeat_time (KEY_BOUNCE_TIME);
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer, "irq %u\r\n", drive_status);
		DBG_PutString(DBG_buffer);
	#endif
}
/* USER CODE END 2 */
