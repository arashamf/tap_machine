
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"
#include "typedef.h"
#include "usart.h"

#include <stdio.h>
// Functions --------------------------------------------------------------------------------------//

// Variables --------------------------------------------------------------------------------------//

//--------------------------------------����� ��������� ��������--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //���������� ��� �������� ������ ��������
	encCounter = LL_TIM_GetCounter(ENC_TIM); //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((encCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->delta = 0; //��������� �� �������� ����������
}

//-------------��������� ��������� �������� � ������ ��������� ���������� �������� ���������-------------//
int32_t read_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	int32_t currCounter=0; //������� ��������� ��������
	int32_t delta = 0; //������� ����� ������� � ���������� ���������� ��������
	currCounter = LL_TIM_GetCounter(ENC_TIM); //������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetRotation != HandleEncData->prevCounter_SetRotation) //���� ������� �������� �������� �� ����� �����������
	{
		delta = (HandleEncData->currCounter_SetRotation - HandleEncData->prevCounter_SetRotation); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� �������� ���������� ��������    
    if	(delta != 0) //���� ���������� ��������� ��������  
		{
			if ((delta > -10) && (delta < 10))// ������ �� �������� ��������� � ������������ �������� 
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


