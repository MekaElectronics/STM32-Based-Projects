#include <stdint.h>
#include "main.h"

#define Delay		100000

int main(void)
{
	LED_Reg_t RegName = REG_ODR;
	STM32_LEDInit();
	while(1)
	{
		STM32_LEDToggle(RegName);
		STM32_SetBasicDelay(100000);
	}
}

void STM32_SetBasicDelay(int count)
{
	int i;
	for (i=0;i<count;i++);
}

void STM32_LEDInit(void)
{
#ifndef STRUCT_BASED
	RCC_AHB1ENR |= RCC_GPIOAEN_BIT;
	GPIOA_MODER |= 1U << 5*2;
	GPIOA_MODER &=~(1U <<(5*2+1));
#else

	RCC->AHB1ENR |= RCC_GPIOAEN_BIT;
	GPIOA->MODER |= 1U << 5*2;
	GPIOA->MODER &=~(1U <<(5*2+1));
#endif
}

void STM32_LEDToggle(LED_Reg_t RegName)
{
	switch(RegName)
	{
	case REG_ODR:
#ifndef STRUCT_BASED
		GPIOA_ODR ^= (1U<<5);
#else
		GPIOA->ODR ^= (1U<<5);
#endif
		STM32_SetBasicDelay(Delay);
		break;
	case REG_BSRR:
#ifndef STRUCT_BASED
		GPIOA_BSRR = (1U<<5);
		STM32_SetBasicDelay(Delay/2);
		GPIOA_BSRR = (1U<<21);
		STM32_SetBasicDelay(Delay/2);
#else
		GPIOA->BSRR = (1U<<5);
		STM32_SetBasicDelay(Delay/2);
		GPIOA->BSRR = (1U<<21);
		STM32_SetBasicDelay(Delay/2);
#endif
		break;
	}
}
