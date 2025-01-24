//Includes ------------------------------------------------------------------------------------------//
#include "systick.h"
#include "typedef.h"
#include <stdio.h>

//Private defines ------------------------------------------------------------------------------------//

// Prototypes ---------------------------------------------------------------------------------------//

// Private variables ---------------------------------------------------------------------------------//
static SysTick_CALLBACK SysTick_CallbackFunction = NULL; //������������� ��������� �� �-� ��������� (��������) xTimer_Task
static uint32_t TicksCounter = 0; //������� ��������� �����


//----------------------------------------------------------------------------------------------------//
void SysTick_Init(SysTick_CALLBACK CallbackFunction)
{
  MODIFY_REG(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk,	(CPU_CLOCK/1000)-1);
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
	
	SysTick_CallbackFunction = CallbackFunction; //���������� ��������� �������� �-� xTimer_Task
}

// ������� ���������� ������� SysTick ----------------------------------------------------------------------//
void SysTick_Callback(void)
{
    TicksCounter++; 
		if ( SysTick_CallbackFunction != NULL ) //���� ��������� �� ������� xTimer_Task ������������������
			{SysTick_CallbackFunction(TicksCounter);} //����� ��������
}

// ���������� �������� SysTick--------------------------------------------------------------------------------//
uint32_t Get_SysTick(void)
{
	return TicksCounter;
}
 
// �������� � ��---------------------------------------------------------------------------------------------//
void delay_ms(uint32_t delay)
{	
  uint32_t ticks = 0;

	ticks = Get_SysTick(); //���������� ����� �� ������ ������ �-��
	while((Get_SysTick() - ticks) < delay)	{}
}