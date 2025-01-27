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
#include "systick.h"
#include "usart.h"
#include <stdio.h>

// Prototypes ------------------------------------------------------------------//
static void encoder_init(void);
static void tim_delay_init (void);
static void timer_bounce_init (void);

// Variables -----------------------------------------------------------------//
static portTickType xTimeNow; //��������� �� �-� �������� ���������� ������� ���� uint32_t Get_SysTick(void)
static xTIMER xTimerList[MAX_xTIMERS]; //������ �������� ����������� ��������

/*static void vTimerUARTmsgCallback(xTimerHandle xTimer); 
static xTimerHandle xTimerUARTmsg;*/
static void vTimerBounceSensorCallback(xTimerHandle xTimer); 
static xTimerHandle xTimerBounceSensor;
static void vTimerBounceBtnCallback(xTimerHandle xTimer); 
static xTimerHandle xTimerBounceBtn;
/* USER CODE END 0 */

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
	LL_TIM_DisableCounter(PWM_TIM); //���������� �������
	LL_TIM_OC_SetCompareCH1(PWM_TIM, PulsePeriod/2); //��������� �������� ��������� (����������)
	LL_TIM_SetAutoReload (PWM_TIM, PulsePeriod); //��������� ������� ��������
	LL_TIM_SetCounter(PWM_TIM, 0); //����� �������� �������
	LL_TIM_CC_EnableChannel(PWM_TIM, LL_TIM_CHANNEL_CH1); //��������� ������ 1 �������
  LL_TIM_EnableCounter(PWM_TIM);	//��������� ������� ��� ��������� ���
}

//-----------------------------------------------------------------------------------------------------//
void drive_PWM_mod (uint32_t PulsePeriod) 
{
	LL_TIM_SetCounter(PWM_TIM, 0); //����� �������� �������
	LL_TIM_OC_SetCompareCH1(PWM_TIM, PulsePeriod/2); //��������� �������� ��������� (����������)
	LL_TIM_SetAutoReload (PWM_TIM, PulsePeriod); //��������� ������� ��������
//	LL_TIM_EnableCounter(PWM_TIM);	//��������� �������  ��� ��������� ���
}

//--------------------------------------------------------------------------------------------------//
void drive_PWM_stop (void)
{
	LL_TIM_DisableCounter(PWM_TIM); //���������� �������
	LL_TIM_CC_DisableChannel(PWM_TIM, LL_TIM_CHANNEL_CH1); //���������� ������ 1 �������
}

//-----------------------------------------------------------------------------------------------------//
static void encoder_init(void) 
{
    
  LL_TIM_SetCounter(ENC_TIM, 32767); // ��������� �������� ��������:
	
	LL_TIM_CC_EnableChannel(ENC_TIM, LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(ENC_TIM, LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(ENC_TIM);     // ��������� �������
}

//---------------------------������������� ������� ��� �������������� ��������---------------------------//
static void tim_delay_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_APB1_GRP2_EnableClock(TIM_DELAY_APB1_BIT); // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK/1000000)-1); //������������ 48���/48=1���
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM_DELAY, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_DELAY);
}

//--------------------------------------�-� �������������� ��������--------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_DELAY, delay); //
	LL_TIM_ClearFlag_UPDATE(TIM_DELAY); //����� ����� ���������� �������
	LL_TIM_SetCounter(TIM_DELAY, 0); //����� �������� ��������
	LL_TIM_EnableCounter(TIM_DELAY); //��������� �������
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY) == 0) {} //�������� ��������� ����� ���������� ������� 
	LL_TIM_DisableCounter(TIM_DELAY); //���������� �������		
}

//--------------------------------������������� ������� �������� ��������--------------------------------//
static void timer_bounce_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(TIM_BOUNCE_APB1_BIT);   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK/2000)-1); //������������ 48���/24000=2���
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM_BOUNCE, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_BOUNCE);	
	
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE); //����� ����� ���������� �������
	LL_TIM_EnableIT_UPDATE(TIM_BOUNCE);
	NVIC_SetPriority(TIM_BOUNCE_IRQn, 1);
  NVIC_EnableIRQ(TIM_BOUNCE_IRQn);
}

//--------------------------------�-� �������� �������� ��������� ��������--------------------------------//
void repeat_time (uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_BOUNCE, 2*delay); 
	LL_TIM_SetCounter(TIM_BOUNCE, 0); //����� �������� ��������
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE); //����� ����� ���������� �������
	LL_TIM_EnableCounter(TIM_BOUNCE); //��������� �������	
}

//---------------------------�-� ���������� ������� �������� ��������� ��������---------------------------//
void TIM_BOUNCE_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_BOUNCE) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TIM_BOUNCE); //����� ����� ���������� �������
	//	Timer_Bounce_Callback();
	}
}

//----------------------------------------------------------------------------------------------------------//
void xTimer_Init	(uint32_t (*GetSysTick)(void))
{
	xTimeNow = GetSysTick; //������������� ��������� �� �-� ��������� ���������� �������
}

//-----------------------------------------------------------------------------------------------------------//
xTimerHandle xTimer_Create(uint32_t xTimerPeriodInTicks, FunctionalState AutoReload, 
tmrTIMER_CALLBACK CallbackFunction, FunctionalState NewState)
{
	xTimerHandle NewTimer = NULL; //������������� ��������� �� ��������� ��� ������������ �������
	uint16_t count;
	
	for (count = 0; count < MAX_xTIMERS; count++) //����� ���������� �������� ������� �������� � ���������
	{
		if (xTimerList[count].CallbackFunction == NULL) 		//���� ��������� �� �-� �������� ����
		{
			xTimerList[count].periodic = xTimerPeriodInTicks;		//����� �������� � ��
			xTimerList[count].AutoReload = AutoReload;				//������������� ������ ��� ���������
			xTimerList[count].CallbackFunction = CallbackFunction;	//��������� �� �-� ��������
			
			if (NewState != DISABLE) //���� ����� ������ � ���������  ENABLE
			{
				xTimerList[count].expiry = xTimeNow() + xTimerPeriodInTicks; //���������� ������� ���������� ������ ��������
				xTimerList[count].State = __ACTIVE; //��������� ������� �������
			} 
			else 
			{	xTimerList[count].State = __IDLE;	}	//���� ����� ������ � ���������  DISABLE	
			NewTimer = (xTimerHandle)(count+1); //������� ������ ��������� �� ��������� �������
			break;
    }
  }
	return NewTimer;
}

//-------------------------------------------------------------------------------------------------------------//
void xTimer_SetPeriod(xTimerHandle xTimer, uint32_t xTimerPeriodInTicks) 
{
	if ( xTimer != NULL )  
	{	xTimerList[(uint32_t)xTimer-1].periodic = xTimerPeriodInTicks;	} //��������� ������ �������� �������� �������
}

//-------------------------------------------------------------------------------------------------------------//
void xTimer_Reload(xTimerHandle xTimer) 
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].expiry = xTimeNow() + xTimerList[(uint32_t)xTimer-1].periodic; //��������� ������� �������
		xTimerList[(uint32_t)xTimer-1].State = __ACTIVE; //������ �������
	}
}

//-------------------------------------------------------------------------------------------------------------//
void xTimer_Delete(xTimerHandle xTimer)
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].CallbackFunction = NULL;
		xTimerList[(uint32_t)xTimer-1].State = __IDLE;
		xTimer = NULL;
	}		
}

//------------------------------------------�-� ���������� xTimer------------------------------------------//
void xTimer_Task(uint32_t portTick)
{
	uint16_t i;
	
	for (i = 0; i < MAX_xTIMERS; i++) {
		switch (xTimerList[i].State) 
		{
			case __ACTIVE: //���� ������ �������
				if ( portTick >= xTimerList[i].expiry ) //���� ������� ��������� ����� ������ ��� ����� ��������������
				{				
					if ( xTimerList[i].AutoReload != DISABLE ) //���� ������ �������������
					{	xTimerList[i].expiry = portTick + xTimerList[i].periodic;	} //��������� ������ ������� ������������ �������
					else 
					{	xTimerList[i].State = __IDLE;	}	//������ ������� �� �������		
					xTimerList[i].CallbackFunction((xTimerHandle)(i+1)); //����� ��������
				}					
				break;
				
			default:
				break;
		}
	}	
}

//---------------------------------------------------------------------------------------------------------//
static void vTimerBounceSensorCallback(xTimerHandle xTimer)
{
	Timer_Bounce_Sensor_Callback();
}

//---------------------------------------------------------------------------------------------------------//
static void vTimerBounceBtnCallback(xTimerHandle xTimer)
{
	Timer_Bounce_Btn_Callback();
}

//---------------------------------------------------------------------------------------------------------//
void timers_init (void)
{
	encoder_init(); 			//������������� ������� ��������
	tim_delay_init(); 		//������������� TIM16 ��� �������������� ��������
//	timer_bounce_init();	//������������� TIM17	��� ������ �������� �������� ������ 	
	SysTick_Init(&xTimer_Task); //����������� ������� SysTick ���������� �� �-� ���������� xTimer_Task
	xTimer_Init(&Get_SysTick); //������������� ��������� xTimer �-�� ��������� ���������� ������� SysTick
	
	xTimerBounceSensor = xTimer_Create(SENSOR_BOUNCE_DELAY, ONCE, &vTimerBounceSensorCallback, DISABLE); 
	xTimerBounceBtn = xTimer_Create(BTN_BOUNCE_TIME, ONCE, &vTimerBounceBtnCallback, DISABLE);
}

//---------------------------------------------------------------------------------------------------------//
void xTimersBounceSensor_Reload (uint16_t delay_ms)
{
	xTimer_SetPeriod(xTimerBounceSensor, delay_ms);
	xTimer_Reload(xTimerBounceSensor);	
}

//---------------------------------------------------------------------------------------------------------//
void xTimerBounceBtn_Reload (uint16_t delay_ms)
{
	xTimer_SetPeriod(xTimerBounceBtn, delay_ms);
	xTimer_Reload(xTimerBounceBtn);	
}


//---------------------------------------------------------------------------------------------------------//
/* USER CODE END 1 */
