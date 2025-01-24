
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"

// Exported types ---------------------------------------------------------------//
typedef struct 
{
	uint16_t 	PulsePeriod; 			//������ ��������
	uint32_t 	PulseInSecond;		//���������� ��������� � �������
	uint16_t 	TurnInMinute;			//���������� �������� ���� � ������ ���������
} turn_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetRotation; 			//���������� ��������� ��������
	int32_t 	currCounter_SetRotation; 			//������� ��������� ��������
	int32_t 	delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t pwm_on									: 1;	//��������� ��� ������
		uint8_t curr_direct							: 1; //������� ��������� ����������� ��������
		uint8_t prev_direct							: 1; //���������� ��������� ����������� �������� 
		uint8_t reserve									:	5;
	};
	uint8_t flag;
}STATUS_FLAG_DRIVE_t;

//-----------------------------------------------------------------------------------//
typedef enum 
{
	DRIVE_OFF						=		0,
	DRIVE_DIRECT						,
	DRIVE_REVERSE						,
	DRIVE_REVERSE_CONTINUE	,
	
} DRIVE_STATUS_t;

// Defines ----------------------------------------------------------------------//
#define 	ON 													1
#define 	OFF 												0
#define 	DIRECT 											1
#define 	REVERSE 										0
#define 	ENABLE 											1
#define 	DISABLE 										0
#define 	PERIODIC 										1
#define 	ONCE 												0
#define 	MAX_xTIMERS 								5

#define 	CPU_CLOCK									(48000000UL)				// ������� ����������� 
#define 	TIMER_CLOCK_PRESCALER			(CPU_CLOCK/1000000UL)
#define 	TIMER_CLOCK								(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define 	TICKS_PER_SECOND					(1000UL) 

#define 	SENSOR_BOUNCE_DELAY 			(5UL) 		// ����� �������� � ��	
#define 	MAX_COUNT_RELIABILITY 		(20UL)
#define 	DEFAULT_COUNT_RELIABILITY (10UL)
#define 	MIN_COUNT_RELIABILITY 		(0UL)
#define 	BTN_BOUNCE_TIME 					(50UL) 		// ����� �������� � ��
#define 	BTN_AUTOREPEAT_TIME 			(100UL)
#define 	COUNT_REPEAT_BUTTON 			(5UL)

#define 	START_TURN_SETUP 					(120UL) 	//��������� ���������� �������� � ������
#define   REDUCER 									(80UL)		//�������� ���������
#define 	STEPS_IN_REV							(3200UL) 	//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	CIRCLE_IN_STEP						(200UL)		//���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 									(STEPS_IN_REV/CIRCLE_IN_STEP)	//���������� ���������� � ����� ���� ��������� (1,8��)
#define 	STEP_TOOL									(STEPS_IN_REV*REDUCER) 				//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� �������� � ��������� 
#define 	STEP18_IN_SEC							(3125UL) 			//���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC							(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 				(60UL)
#define 	SECOND_PER_DEGREE 				(3600UL)

#define 	LOWER_PERIOD_SOFT_START		(6250UL) 		//��������� ������ �������� ����� ���
#define 	STEP_PERIOD_SOFT_START		(10UL) 				//��� ���������� ������� �������� ����� ���

// Private variables ----------------------------------------------------------------------------------------//
typedef enum 
{
	SENSOR_STATE_OFF 				= 0	,			//����� - ������ ������� �� ���������
	SENSOR_STATE_BOUNCE					, 		//����� -  ������� �������
	SENSOR_STATE_ON							,			//����� - ������ ������� ���������
} SENSOR_STATE_t; 									//������ ������������ �������

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,				//����� - ������ �� ������
	KEY_STATE_ON							,				//����� - ������ ������
	KEY_STATE_BOUNCE					, 			//����� -  ������� ������
	KEY_STATE_AUTOREPEAT			,	 			//����� - ����� �������� (�����������) ������� ������
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 										//������ ������������ ������

#ifdef __cplusplus
}
#endif

#endif 

