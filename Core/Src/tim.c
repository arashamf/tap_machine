/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "typedef.h"
#include "gpio.h"

// Prototypes ------------------------------------------------------------------//
static void encoder_init(void);
static void tim_delay_init (void);
static void timer_bounce_init (void);

// Variables -----------------------------------------------------------------//
uint8_t end_bounce = 0;
volatile uint16_t delay_ms = 0;
/* USER CODE END 0 */

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration
  PA6   ------> TIM3_CH1
  PA7   ------> TIM3_CH2
  */
  GPIO_InitStruct.Pin = ENC_CH1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(ENC_CH1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ENC_CH2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(ENC_CH2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  LL_TIM_SetEncoderMode(TIM3, LL_TIM_ENCODERMODE_X2_TI1);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1_N2);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1_N4);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}
/* TIM14 init function */
void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  TIM_InitStruct.Prescaler = 47;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM14);
  LL_TIM_OC_EnablePreload(TIM14, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 499;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM14, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM14, LL_TIM_CHANNEL_CH1);
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**TIM14 GPIO Configuration
    PA4     ------> TIM14_CH1
    */
  GPIO_InitStruct.Pin = PULSE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(PULSE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 1 */
//--------------------------------------------------------------------------------------------------//
void drive_PWM_start (uint32_t PulsePeriod) 
{
	LL_TIM_DisableCounter(PWM_TIM); //отключение таймера
	LL_TIM_OC_SetCompareCH1(PWM_TIM, PulsePeriod/2); //установка регистра сравнения (скважность)
	LL_TIM_SetAutoReload (PWM_TIM, PulsePeriod); //установка периода импульса
	LL_TIM_SetCounter(PWM_TIM, 0); //сброс счётчика таймера
	LL_TIM_CC_EnableChannel(PWM_TIM, LL_TIM_CHANNEL_CH1); //включение канала 1 таймера
  LL_TIM_EnableCounter(PWM_TIM);	//включение таймера для генерации ШИМ
}

//-----------------------------------------------------------------------------------------------------//
void drive_PWM_mod (uint32_t PulsePeriod) 
{
	LL_TIM_SetCounter(PWM_TIM, 0); //сброс счётчика таймера
	LL_TIM_OC_SetCompareCH1(PWM_TIM, PulsePeriod/2); //установка регистра сравнения (скважность)
	LL_TIM_SetAutoReload (PWM_TIM, PulsePeriod); //установка периода импульса
//	LL_TIM_EnableCounter(PWM_TIM);	//включение таймера  для генерации ШИМ
}

//--------------------------------------------------------------------------------------------------//
void drive_PWM_stop (void)
{
	LL_TIM_DisableCounter(PWM_TIM); //отключение таймера
	LL_TIM_CC_DisableChannel(PWM_TIM, LL_TIM_CHANNEL_CH1); //выключение канала 1 таймера
}

//-----------------------------------------------------------------------------------------------------//
static void encoder_init(void) 
{
    
  LL_TIM_SetCounter(ENC_TIM, 32767); // начальное значение счетчика:
	
	LL_TIM_CC_EnableChannel(ENC_TIM, LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(ENC_TIM, LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(ENC_TIM);     // включение таймера
}

//---------------------------инициализация таймера для микросекундной задержки---------------------------//
static void tim_delay_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_APB1_GRP2_EnableClock(TIM_DELAY_APB1_BIT); // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK/1000000)-1); //предделитель 48МГц/48=1МГц
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM_DELAY, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_DELAY);
}

//--------------------------------------ф-я микросекундной задержки--------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_DELAY, delay); //
	LL_TIM_ClearFlag_UPDATE(TIM_DELAY); //сброс флага обновления таймера
	LL_TIM_SetCounter(TIM_DELAY, 0); //сброс счётного регистра
	LL_TIM_EnableCounter(TIM_DELAY); //включение таймера
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY) == 0) {} //ожидание установки флага обновления таймера 
	LL_TIM_DisableCounter(TIM_DELAY); //выключение таймера		
}

//--------------------------------инициализация таймера задержки дребезга--------------------------------//
static void timer_bounce_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(TIM_BOUNCE_APB1_BIT);   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK/2000)-1); //предделитель 48МГц/24000=2КГц
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM_BOUNCE, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_BOUNCE);	
	
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE); //сброс флага обновления таймера
	LL_TIM_EnableIT_UPDATE(TIM_BOUNCE);
	NVIC_SetPriority(TIM_BOUNCE_IRQn, 2);
  NVIC_EnableIRQ(TIM_BOUNCE_IRQn);
}

//--------------------------------ф-я задержки ожидания окончания дребезга--------------------------------//
void repeat_time (uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_BOUNCE, 2*delay); 
	LL_TIM_SetCounter(TIM_BOUNCE, 0); //сброс счётного регистра
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE); //сброс флага обновления таймера
	LL_TIM_EnableCounter(TIM_BOUNCE); //включение таймера	
}

//---------------------------ф-я прерывания таймера ожидания окончания дребезга---------------------------//
void TIM_BOUNCE_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_BOUNCE) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TIM_BOUNCE); //сброс флага обновления таймера
		Timer_Bounce_Callback();
	}
}

//-----------------------------функция задержки, использующая таймер SysTickа-----------------------------//
void Delay_Ms (uint16_t bounce_delay)
{
	delay_ms = bounce_delay ;
	while (delay_ms > 0) {}
}

//----------------------------------------коллбэк таймера SysTickа--------=--------------------------------//
void TIM_DelayMs_Callback (void)
{
	if(delay_ms > 0)  
	{	
		delay_ms--;	
	}
}

//---------------------------------------------------------------------------------------------------------//
void timers_init (void)
{
	encoder_init(); 			//инициализация таймера энкодера
	tim_delay_init(); 		//инициализация TIM16 для микросекундных задержек
	timer_bounce_init();	//инициализация TIM17	для отчёта задержек дребезга кнопок 							
}
/* USER CODE END 1 */
