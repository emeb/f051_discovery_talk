/*
 * led.c - f051 discover LED setup
 */

#include "led.h"

/*
 * Initialize the breakout board LED
 */
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* turn on clock for LED GPIO */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
		/* Enable PC9,8 for output */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/*
 * Turn on LED
 */
void led_on(uint32_t LED)
{
	GPIOC->ODR |= LED;
}

/*
 * Turn off LED
 */
void led_off(uint32_t LED)
{
	GPIOC->ODR &= ~LED;
}

/*
 * Toggle LED
 */
void led_toggle(uint32_t LED)
{
	GPIOC->ODR ^= LED;
}

