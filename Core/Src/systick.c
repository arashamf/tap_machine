//Includes ------------------------------------------------------------------------------------------//
#include "systick.h"
#include "typedef.h"
#include <stdio.h>

//Private defines ------------------------------------------------------------------------------------//

// Prototypes ---------------------------------------------------------------------------------------//

// Private variables ---------------------------------------------------------------------------------//
static SysTick_CALLBACK SysTick_CallbackFunction = NULL; //инициализация указателя на ф-ю обработки (коллбэка) xTimer_Task
static uint32_t TicksCounter = 0; //счётчик системных тиков


//----------------------------------------------------------------------------------------------------//
void SysTick_Init(SysTick_CALLBACK CallbackFunction)
{
  MODIFY_REG(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk,	(CPU_CLOCK/1000)-1);
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
	
	SysTick_CallbackFunction = CallbackFunction; //присвоение указателю значения ф-и xTimer_Task
}

// коллбэк прерывания таймера SysTick ----------------------------------------------------------------------//
void SysTick_Callback(void)
{
    TicksCounter++; 
		if ( SysTick_CallbackFunction != NULL ) //если указатель на коллбэк xTimer_Task проинициализирован
			{SysTick_CallbackFunction(TicksCounter);} //вызов коллбэка
}

// возвращает значение SysTick--------------------------------------------------------------------------------//
uint32_t Get_SysTick(void)
{
	return TicksCounter;
}
 
// задержка в мс---------------------------------------------------------------------------------------------//
void delay_ms(uint32_t delay)
{	
  uint32_t ticks = 0;

	ticks = Get_SysTick(); //количество тиков на момент вызова ф-ии
	while((Get_SysTick() - ticks) < delay)	{}
}