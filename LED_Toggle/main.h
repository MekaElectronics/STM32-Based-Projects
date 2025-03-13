/*
 * main.h
 *
 *  Created on: Mar 12, 2025
 *      Author: Majd
 */

#ifndef MAIN_H_
#define MAIN_H_

//#define STRUCT_BASED


#define	PERIPH_BASE			(0x40000000UL)
#define APB1_BASE			(PERIPH_BASE 	+ 	0x00000000UL)
#define AHB1_BASE			(PERIPH_BASE 	+ 	0x00020000UL)

#define GPIOA_BASE			(AHB1_BASE 		+ 	0x00000000UL)
#define RCC_BASE			(AHB1_BASE 		+ 	0x00003800UL)


#define GPIO_MODER_INPUT	(0x00)
#define GPIO_MODER_OUTPUT	(0x01)
#define GPIO_MODER_ALTER	(0x10)
#define GPIO_MODER_ANALOG	(0x11)

#define RCC_GPIOAEN_BIT		(1U				<<	0)

#define __IO volatile

#ifndef STRUCT_BASED

	#define RCC_AHB1ENR			(*(volatile uint32_t *)(RCC_BASE 		+ 	0x00000030UL))

	#define GPIOA_MODER			(*(volatile uint32_t *)(GPIOA_BASE		+ 	0x00000000UL))
	#define GPIOA_ODR			(*(volatile uint32_t *)(GPIOA_BASE 		+	0x00000014UL))
	#define GPIOA_BSRR			(*(volatile uint32_t *)(GPIOA_BASE		+	0x00000018UL))

#else

	typedef struct
	{
		__IO uint32_t NotNeeded[12];
		__IO uint32_t AHB1ENR;
		__IO uint32_t AHB2ENR;
		__IO uint32_t APB1ENR;
	}RCC_TypeDef;

	typedef struct
	{
		__IO uint32_t MODER;
		__IO uint32_t OTYPER;
		__IO uint32_t OSPEEDR;
		__IO uint32_t PUPDR;
		__IO uint32_t IDR;
		__IO uint32_t ODR;
		__IO uint32_t BSRR;
		__IO uint32_t LCKR;
		__IO uint32_t AFR[2];
	}GPIO_TypeDef;

	#define RCC			((RCC_TypeDef*) RCC_BASE)
	#define GPIOA		((GPIO_TypeDef*) GPIOA_BASE)

#endif

typedef enum
{
	REG_ODR,
	REG_BSRR,
}LED_Reg_t;

void STM32_SetBasicDelay(int count);
void STM32_LEDToggle(LED_Reg_t RegName);
void STM32_LEDInit(void);


#endif /* MAIN_H_ */
