#include "UART.h"
#include <stdio.h>


int __io_putchar(int ch)
{
	UART_Write(USART2,ch);
	return ch;
}

/*
 * main function
 */
int main(void)
{
	uint8_t TxMode=0xFF;
	char ch[20];
	/*
	 * This part will handle the initialization of the User button
	 * which will allow the STM32 to switch between Rx and Tx modes
	 */
	RCC->AHB1ENR |= GPIOCEN;
	GPIOC->MODER &=~(3U<<26);


	UART2_RXTX_Init();
	while(1)
	{
		if ((GPIOC->IDR & BUTTON_PIN))
		{
			while ((GPIOC->IDR & BUTTON_PIN));
			TxMode ^= 0xFF;
		}
		switch (TxMode)
		{
		case 0xFF:
			printf("Hello from USART2 STM32F401RE\n");
			break;
		case 0x00:
			UART_ReadBuffer(USART2,ch, sizeof(ch));
			printf("Received Data : %s\n",ch);
		default:
			break;
		}
	}
}
