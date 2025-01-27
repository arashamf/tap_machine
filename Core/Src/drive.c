
// Includes ------------------------------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"
#include "stanok_math.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"

#include <stdio.h>
// Exported types ------------------------------------------------------------------------------------//

//Private defines ------------------------------------------------------------------------------------//

// Prototypes ----------------------------------------------------------------------------------------//
uint8_t setup_turn (encoder_data_t * , turn_data_t * );
void turn_drive_start (turn_data_t * );
void turn_drive_mod (turn_data_t * );

// Variables -----------------------------------------------------------------------------------------//
volatile uint8_t drive_status = DRIVE_OFF;
STATUS_FLAG_DRIVE_t status_flag = {0};

//----------------------------------------------------------------------------------------------------//
void drive_init ( turn_data_t * HandleTurnData ) 
{
	HandleTurnData->TurnInMinute = START_TURN_SETUP;
	calc_period_pulse (HandleTurnData); //расчёт периода импульса
	status_flag.pwm_on = OFF;
}


//----------------------------------------------------------------------------------------------------//
uint8_t setup_turn (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	if ((read_enc_data (HandleEncData, HandleTurnData)) != 0)	//проверка показаний энкодера
	{	
		calc_period_pulse (HandleTurnData); //расчёт периода импульса
		return ON;
	}
	else
	{	return OFF;	}
}

//----------------------------------------------------------------------------------------------------//
void turn_drive_start (turn_data_t * HandleTurnData)
{
	drive_PWM_start(HandleTurnData->PulsePeriod); //подача изменнёного ШИМа на двигатель
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_soft_start (turn_data_t * HandleTurnData)
{
	volatile uint32_t period_PWM = LOWER_PERIOD_SOFT_START;
	drive_PWM_start(period_PWM); //подача изменнёного ШИМа на двигатель
	delay_us(2000);
	
	while (period_PWM > HandleTurnData->PulsePeriod)
	{

		period_PWM = (period_PWM - STEP_PERIOD_SOFT_START);
		drive_PWM_mod(period_PWM);
		delay_us(2000);
	}

	drive_PWM_mod(HandleTurnData->PulsePeriod);
}

//----------------------------------------------------------------------------------------------------//
void turn_drive_mod (turn_data_t * HandleTurnData)
{
	drive_PWM_mod(HandleTurnData->PulsePeriod); 
}

//----------------------------------------------------------------------------------------------------//
void turn_drive_stop (void)
{
	drive_PWM_stop(); //остановка генерации ШИМа
}

//----------------------------------------------------------------------------------------------------//
void main_loop (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData)
{
	uint8_t dif_status = 0;
	uint8_t status_encoder = OFF;
//	status_encoder = setup_turn (HandleEncData, HandleTurnData); //проверка состояния энкодера
	dif_status = status_sensor_machine();
	if (dif_status)
	{
		drive_status += dif_status;
		dif_status = 0;
		
		#ifdef __USE_DBG
			sprintf (DBG_buffer, "drive_status=%u\r\n", drive_status);
			DBG_PutString(DBG_buffer);
		#endif		
	}
	
	switch (drive_status)
	{
		case DRIVE_OFF: 			//состояние машины - выключено
			status_flag.pwm_on = OFF;
			drive_PWM_stop ();
			Enable_Drive (OFF);
			break;
		
		case DRIVE_DIRECT:		//состояние машины - прямое вращение
			if (status_flag.pwm_on == OFF) //если мотор выключен
			{
				Enable_Drive (ON);               //разрешение включения двигателя
				status_flag.curr_direct = DIRECT; //установка флага направления вращения двигателя
				Select_Drive_Direction (status_flag.curr_direct); //выбор направления вращения
				turn_drive_soft_start (HandleTurnData); //плавный пуск двигателя
				status_flag.pwm_on = ON; //установка флага состояния двигателя - включено
			}
			/*else
			{
				if (status_encoder == ON)				//проверка энкодера
				{	turn_drive_mod (HandleTurnData);	}
			}*/
			break;
		
		case DRIVE_REVERSE:			//состояние машины - обратное вращение
			if (status_flag.curr_direct == DIRECT)
			{
				Enable_Drive (OFF);		 //запрещение включения двигателя
				turn_drive_stop (); 		//остановка ШИМа
				status_flag.curr_direct = REVERSE; //установка флага направления вращения двигателя
				Select_Drive_Direction (status_flag.curr_direct); //выбор направления вращения
				Enable_Drive (ON); 				//разрешение включения двигателя
				turn_drive_soft_start (HandleTurnData);	//плавный пуск двигателя
				status_flag.pwm_on = ON;	//установка флага состояния двигателя - включено
			}
		/*	if (status_encoder == ON)				 //проверка энкодера
			{	turn_drive_mod (HandleTurnData);	}*/
			break;
			
		case DRIVE_REVERSE_CONTINUE:		//состояние машины - продолжение обратного вращения
		/*	if (status_encoder == ON)					//проверка энкодера
			{	turn_drive_mod (HandleTurnData);	}*/
			break;
		
		default:
			drive_status = DRIVE_OFF;
			break;
	}
	
	if (scan_btn() == ON) //проверка состояния кнопки стоп
	{
		drive_status = DRIVE_OFF;
		Enable_Drive (OFF);		 //запрещение включения двигателя
		turn_drive_stop (); 		//остановка ШИМа
	}
}


