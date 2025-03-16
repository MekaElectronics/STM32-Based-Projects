#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

/*
 * bit macros
 */
#define GPIOAEN		(1U<<0)
#define GPIOCEN		(1U<<2)
#define UART2EN		(1U<<17)
#define UART_CR1_TE (1U<<3)
#define UART_CR1_RE	(1U<<2)
#define UART_ENABLE	(1U<<13)
#define BUTTON_PIN	(1U<<13)
/*
 * Frequency & BaudRate macros
 */
#define SYS_FREQ	16000000
#define BAUDRATE	115200

/*
 * Function Prototypes
 */
void UART2_TX_Init();
void UART2_RX_Init();
void UART2_RXTX_Init();
static uint16_t GetUARTDiv(uint32_t Clk, uint32_t BaudRate);
static void SetUARTBaudrate(USART_TypeDef *USARTx,uint32_t Clk, uint32_t BaudRate);
void UART_Write(USART_TypeDef * USARTx,int ch);
void UART_ReadBuffer(USART_TypeDef * USARTx, char * Buf, uint32_t Len);


#endif /* UART_H_ */
