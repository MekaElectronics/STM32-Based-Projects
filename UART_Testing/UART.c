#include "UART.h"

void UART2_TX_Init()
{
	/*
	 * Starting first by enabling clock access to GPIOA which is located on AHB1
	 */
	RCC->AHB1ENR |=GPIOAEN;
	/*
	 * Setting the mode register to alternate function
	 * (Value 0b10 as indicated in the datasheet)
	 * Since it is PA2 for TX, this is what I will be working with.
	 */
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	/*
	 * Now onto selecting which alternate function I am configuring
	 * The value needed in AFRL2 is 0b0111 hence the usage of 7 which
	 * is in binary 0b111
	 */
	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (7U<<8);
	/*
	 * Now, moving onto enabling the UART2 clock access
	 */
	RCC->APB1ENR |= UART2EN;

	SetUARTBaudrate(USART2,SYS_FREQ,115200);
	/*
	 * making sure that only transmission is enabled
	 */
	USART2->CR1 = UART_CR1_TE | UART_ENABLE;
}

void UART2_RX_Init()
{
	/*
	 * Starting first by enabling clock access to GPIOA which is located on AHB1
	 */
	RCC->AHB1ENR |=GPIOAEN;
	/*
	 * Setting the mode register to alternate function
	 * (Value 0b10 as indicated in the datasheet)
	 * Since it is PA3 for RX, this is what I will be working with.
	 */
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	/*
	 * Now onto selecting which alternate function I am configuring
	 * The value needed in AFRL2 is 0b0111 hence the usage of 7 which
	 * is in binary 0b111
	 */
	GPIOA->AFR[0] &=~(1U<<15);
	GPIOA->AFR[0] |= (7U<<12);
	/*
	 * Now, moving onto enabling the UART2 clock access
	 */
	RCC->APB1ENR |= UART2EN;

	SetUARTBaudrate(USART2,SYS_FREQ,115200);
	/*
	 * making sure that only transmission is enabled
	 */
	USART2->CR1 = UART_CR1_RE | UART_ENABLE;
}

void UART2_RXTX_Init()
{
	/*
	 * Starting first by enabling clock access to GPIOA which is located on AHB1
	 */
	RCC->AHB1ENR |=GPIOAEN;
	/*
	 * Setting the mode register to alternate function
	 * (Value 0b10 as indicated in the datasheet)
	 * Since it is PA3 for Rx and PA2 for Tx, this is what I will be working with.
	 */
	GPIOA->MODER &=~((1U<<6) | (1U<<4));
	GPIOA->MODER |= (1U<<7) | (1U<<5);
	/*
	 * Now onto selecting which alternate function I am configuring
	 * The value needed in AFRL2 is 0b0111 hence the usage of 7 which
	 * is in binary 0b111
	 */
	GPIOA->AFR[0] &=~((1U<<15) | (1U<<11));
	GPIOA->AFR[0] |= (7U<<12) | (7U<<8);
	/*
	 * Now, moving onto enabling the UART2 clock access
	 */
	RCC->APB1ENR |= UART2EN;

	SetUARTBaudrate(USART2,SYS_FREQ,115200);
	/*
	 * making sure that only transmission is enabled
	 */
	USART2->CR1 = UART_CR1_RE | UART_CR1_TE | UART_ENABLE;
}

void SetUARTBaudrate(USART_TypeDef *USARTx,uint32_t Clk, uint32_t BaudRate)
{
	USARTx->BRR = GetUARTDiv(Clk,BaudRate);
}

uint16_t GetUARTDiv(uint32_t Clk, uint32_t BaudRate)
{
	return ((Clk +(BaudRate/2U))/BaudRate);
}

void UART_ReadBuffer(USART_TypeDef * USARTx, char * Buf, uint32_t Len)
{
	for (uint32_t i = 0; i< Len;i++)
	{
		while (!(USARTx->SR & (1U<<5)));
		Buf[i] = (char)(USARTx->DR & 0xFF);
		if (Buf[i]=='\n')
		{
			Buf[i+1] = '\0';
			return;
		}
	}
	Buf[Len - 1] = '\0';
}

void UART_Write(USART_TypeDef * USARTx,int ch)
{
	/*
	 * Making sure that the data register is empty
	 */
	while (!(USARTx->SR & (1U<<7)));

	/*
	 * in this part, I will be loading the value that i need into the
	 * data register.
	 */
	USARTx->DR = (ch & 0xFF);

}
