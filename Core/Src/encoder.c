
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"
#include "typedef.h"
#include "usart.h"

#include <stdio.h>
// Functions --------------------------------------------------------------------------------------//

// Variables --------------------------------------------------------------------------------------//

//--------------------------------------сброс показаний энкодера--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //переменна€ дл€ хранени€ данных энкодера
	encCounter = LL_TIM_GetCounter(ENC_TIM); //сохранение текущего показани€ энкодера
	HandleEncData->currCounter_SetRotation = (32767 - ((encCounter-1) & 0xFFFF))/2; //преобразование полученного показани€ энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //сохранение преобразованного текущего показани€ энкодера в структуру установки шага поворота	
	HandleEncData->delta = 0; //показани€ от энкодера обнул€ютс€
}

//-------------обработка показаний энкодера в режиме установки количества оборотов двигател€-------------//
int32_t read_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	int32_t currCounter=0; //текущее показание энкодера
	int32_t delta = 0; //разница между текущим и предыдущим показанием энкодера
	currCounter = LL_TIM_GetCounter(ENC_TIM); //текущее показание энкодера
	HandleEncData->currCounter_SetRotation = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счЄт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetRotation != HandleEncData->prevCounter_SetRotation) //если текущее значение энкодера на равно предыдущему
	{
		delta = (HandleEncData->currCounter_SetRotation - HandleEncData->prevCounter_SetRotation); //разница между текущим и предыдущим показанием энкодера
    HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //сохранение текущего показанаи€ энкодера    
    if	(delta != 0) //если изменилось положение энкодера  
		{
			if ((delta > -10) && (delta < 10))// защита от дребезга контактов и переполнени€ счетчика 
			{  
				HandleTurnData->TurnInMinute += delta;	
				if (HandleTurnData->TurnInMinute < 6)
				{	HandleTurnData->TurnInMinute = 6;	}
				else
				{
					if (HandleTurnData->TurnInMinute > 200)
					{	HandleTurnData->TurnInMinute = 200;	}
				}
				/*	#ifdef __USE_DBG
						sprintf (DBG_buffer, "T%u\r\n", HandleTurnData->TurnInMinute);
						DBG_PutString(DBG_buffer);
					#endif*/

			}
		}
	}	
	return delta;
}


