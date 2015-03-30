/*
 * systick.c - systick routines
 */

#include "systick.h"
#include "debounce.h"

static __IO uint32_t TimingDelay;
debounce_state dbs_btn;

/*
 * init the system tick infrastructure
 */
void systick_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* enable clocks for GPIOA */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* init GPIO for button on PA[0] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* init the user button debounce */
	init_debounce(&dbs_btn, 15);
	
	/* start the 1ms system tick */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

/*
 * delay for number of milliseconds
 */
void systick_delayms(uint32_t ms)
{
	TimingDelay = ms;

	while(TimingDelay != 0);
}

/*
 * get_button(n) - returns state of button #n
 */
uint8_t systick_get_button(void)
{
	return dbs_btn.state;
}

/*
 * IRQ handler
 */
void SysTick_Handler(void)
{
	/* debounce button */
	debounce(&dbs_btn, (GPIOA->IDR&1));

	/* update ms delay timer */
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
