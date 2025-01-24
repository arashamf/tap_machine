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
// Private variables -------------------------------------------------------------------------------//
uint8_t flag_end_bounce = OFF;
volatile uint8_t status_sensor = OFF;
volatile uint8_t machine_state = SENSOR_STATE_OFF; 

// Prototypes ---------------------------------------------------------------------------------------//
static uint8_t Read_Status_Sensor (void);
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
  GPIO_InitStruct.Pin = SENSOR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SENSOR_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DIR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(DIR_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = BTN1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(BTN1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = BTN2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(BTN2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ENABLE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(ENABLE_GPIO_Port, &GPIO_InitStruct);

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

//------------------------------------------------------------------------------------------//
static uint8_t Read_Status_Sensor (void)
{
	return (LL_GPIO_IsInputPinSet(SENSOR_GPIO_Port, SENSOR_Pin));
}

//------------------------------------------------------------------------------------------//
static uint8_t Read_Status_BTN1 (void)
{
	uint8_t status_BTN = OFF;
	if (LL_GPIO_IsInputPinSet(BTN1_GPIO_Port, BTN1_Pin) == OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
	{	status_BTN = ON; }
	return status_BTN;
}

//------------------------------------------------------------------------------------------//
static uint8_t Read_Status_BTN2 (void)
{
	uint8_t status_BTN = OFF;
	if (LL_GPIO_IsInputPinSet(BTN2_GPIO_Port, BTN2_Pin) == OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
	{	status_BTN = ON; }
	return status_BTN;
}


//--------------------------------------------------------------------------------------------------//
void init_status_sensor (void)
{
	status_sensor = Read_Status_Sensor();
	if (status_sensor == ON)
	{	machine_state = SENSOR_STATE_ON;	}
	else
	{
		if (status_sensor == OFF)
		{	machine_state = SENSOR_STATE_OFF;	}
	}
}

//--------------------------------------------------------------------------------------------------//
uint8_t status_sensor_machine (void)
{
	static __IO uint16_t key_repeat_time; // ������� ������� �������
	static __IO int8_t reliability_count = DEFAULT_COUNT_RELIABILITY;
	
	if(machine_state == SENSOR_STATE_OFF) //������ - ������ ����� ����
	{
		if ((status_sensor = Read_Status_Sensor()) == ON)
		{	
			machine_state = SENSOR_STATE_BOUNCE;	
			repeat_time (SENSOR_BOUNCE_DELAY);
		}
		return OFF;
	}
	
	if (machine_state == SENSOR_STATE_BOUNCE)	//������ - ������� �������
	{
		if (flag_end_bounce == ON)
		{
			flag_end_bounce = OFF;
			if((status_sensor = Read_Status_Sensor()) == ON)	
			{	reliability_count++;	}	
			else 
			{
				if((status_sensor = Read_Status_Sensor()) == OFF)	
				{	reliability_count--;	}
			}
			if ((reliability_count < MAX_COUNT_RELIABILITY) && (reliability_count > MIN_COUNT_RELIABILITY))
			{	
				repeat_time (SENSOR_BOUNCE_DELAY);	
				return OFF;
			}
			else
			{
				if (reliability_count == MAX_COUNT_RELIABILITY)
				{	
					machine_state = SENSOR_STATE_ON;	
				}
				else
				{	
					if (reliability_count == MIN_COUNT_RELIABILITY)
					{	
						machine_state = SENSOR_STATE_OFF;	
					}
				}
				reliability_count = DEFAULT_COUNT_RELIABILITY;
				return ON;
			}
		}
	}
	if(machine_state == SENSOR_STATE_ON)	//������ - ������ ����� �������
	{
		if ((status_sensor = Read_Status_Sensor()) == OFF)
		{	
			machine_state = SENSOR_STATE_BOUNCE;	
			repeat_time (SENSOR_BOUNCE_DELAY);
		}
	}	
	return OFF;
}

//--------------------------------------------------------------------------------------------------//
uint8_t scan_btn (void)
{
	static __IO uint8_t key_state = KEY_STATE_OFF; // ��������� ��������� ������ - �� ������
	static __IO uint8_t count_autorepeat = 0;
	
	if(key_state == KEY_STATE_OFF) //���� ������ ���� �������� - �������� �������
	{
		if(Read_Status_BTN2() == ON)	//���� ������ ������
		{
			key_state =  KEY_STATE_ON; //������� �� ��������� ������ - ����� �������� ������� ������
		}
	}
	
	if (key_state ==  KEY_STATE_ON)  //����� �������� ������� ������
	{
		repeat_time (BTN_BOUNCE_TIME); //������ ������� �������� ��������� ��������
		key_state = KEY_STATE_BOUNCE; // ������� � ����� ��������� ��������
	}
	
	if(key_state == KEY_STATE_BOUNCE) //����� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			if(Read_Status_BTN2() == OFF)	 // ���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return OFF; //������� 0 
			}	
			else //���� ������ ���������� ������������
			{	
				repeat_time (BTN_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				key_state = KEY_STATE_AUTOREPEAT;   //������� � ����� ����������� 
				count_autorepeat = 0;
			}
		}
	}
	if (key_state == KEY_STATE_AUTOREPEAT) //����� �����������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� (��������������� � ���������� �������)
		{
			if(Read_Status_BTN2() == OFF)	 // ���� ������ ���� �������� 
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return ON; //�������� ��������� ������
			}
			else //���� ������ ���������� ������������ - �������� ���������� ������
			{	
				repeat_time (BTN_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������				
				if (count_autorepeat < COUNT_REPEAT_BUTTON) //�������� 500 ��
				{	count_autorepeat++;	}
				else //���� ������ ������������ ����� 650 ��
				{	
					key_state = KEY_STATE_WAIT_TURNOFF; //������ �������� ���������� ������
					return ON; //�������� ��������� ������
				}
			} 	
		}					
	}
	
	if (key_state == KEY_STATE_WAIT_TURNOFF) //�������� �������� ���������� ������
	{	
		if (end_bounce == SET) //���� ���� ��������� �������� ���������� (��������������� � ���������� �������)
		{
			if(Read_Status_BTN2() == OFF)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{				
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
			}
			else
			{
				repeat_time (BTN_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			}
		}
	}
	return OFF;
}


//----------------------------------------------------------------------------------------------------//
void Sensor_Callback(void)
{
	status_sensor = Read_Status_Sensor();
}

//----------------------------------------------------------------------------------------------------//
void Timer_Bounce_Callback(void)
{
	flag_end_bounce = ON;
	LL_TIM_DisableCounter(TIM_BOUNCE); //���������� �������	
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer, "end_bounce=%u\r\n", flag_end_bounce);
		DBG_PutString(DBG_buffer);
	#endif
}
/* USER CODE END 2 */
