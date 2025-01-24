
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
	uint16_t 	PulsePeriod; 			//период импульса
	uint32_t 	PulseInSecond;		//количество импульсов в секунду
	uint16_t 	TurnInMinute;			//количество оборотов вала с учётом редуктора
} turn_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetRotation; 			//сохранённое показание энкодера
	int32_t 	currCounter_SetRotation; 			//текущее показание энкодера
	int32_t 	delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t pwm_on									: 1;	//состояние ШИМ канала
		uint8_t curr_direct							: 1; //текущее показание направления вращения
		uint8_t prev_direct							: 1; //сохранённое показание направления вращения 
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

#define 	CPU_CLOCK									(48000000UL)				// Частота контроллера 
#define 	TIMER_CLOCK_PRESCALER			(CPU_CLOCK/1000000UL)
#define 	TIMER_CLOCK								(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define 	TICKS_PER_SECOND					(1000UL) 

#define 	SENSOR_BOUNCE_DELAY 			(5UL) 		// время дребезга в мс	
#define 	MAX_COUNT_RELIABILITY 		(20UL)
#define 	DEFAULT_COUNT_RELIABILITY (10UL)
#define 	MIN_COUNT_RELIABILITY 		(0UL)
#define 	BTN_BOUNCE_TIME 					(50UL) 		// время дребезга в мс
#define 	BTN_AUTOREPEAT_TIME 			(100UL)
#define 	COUNT_REPEAT_BUTTON 			(5UL)

#define 	START_TURN_SETUP 					(120UL) 	//начальное количество оборотов в минуту
#define   REDUCER 									(80UL)		//делитель редуктора
#define 	STEPS_IN_REV							(3200UL) 	//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера
#define 	CIRCLE_IN_STEP						(200UL)		//количество шагов (1,8гр) в одном полном обороте (360 гр)
#define 	STEP_DIV 									(STEPS_IN_REV/CIRCLE_IN_STEP)	//количество микрошагов в одном шаге двигателя (1,8гр)
#define 	STEP_TOOL									(STEPS_IN_REV*REDUCER) 				//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера и редуктора 
#define 	STEP18_IN_SEC							(3125UL) 			//количество секунд в одном шаге двигателя (1,8гр)
#define 	CIRCLE_IN_SEC							(STEP18_IN_SEC*CIRCLE_IN_STEP)	//количество секунд в одном полном обороте двигателя (360 гр)
#define 	SECOND_PER_MINUTE 				(60UL)
#define 	SECOND_PER_DEGREE 				(3600UL)

#define 	LOWER_PERIOD_SOFT_START		(6250UL) 		//начальный период плавного пуска ШИМ
#define 	STEP_PERIOD_SOFT_START		(10UL) 				//шаг уменьшения периода плавного пуска ШИМ

// Private variables ----------------------------------------------------------------------------------------//
typedef enum 
{
	SENSOR_STATE_OFF 				= 0	,			//режим - статус сенсора не изменился
	SENSOR_STATE_BOUNCE					, 		//режим -  дребезг сенсора
	SENSOR_STATE_ON							,			//режим - статус сенсора изменился
} SENSOR_STATE_t; 									//статус сканирования сенсора

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,				//режим - кнопка не нажата
	KEY_STATE_ON							,				//режим - кнопка нажата
	KEY_STATE_BOUNCE					, 			//режим -  дребезг кнопки
	KEY_STATE_AUTOREPEAT			,	 			//режим - режим ожидания (автоповтора) отжатия кнопки
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 										//статус сканирования кнопки

#ifdef __cplusplus
}
#endif

#endif 

