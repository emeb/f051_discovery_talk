/*
 * pwm.c - pwm DAC routines
 * 03-28-2015 E. Brombaugh
 */

#include "pwm.h"
#include "lpc.h"

/* counter to decimate from 48kHz PWM rate to 8kHz LPC rate */
#define DECIM_RATE 6
uint8_t decim_ctr = 0;

/*
 * Initialize Timer for PWM
 */
void pwm_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* setup some rate stuff */
	decim_ctr = 0;

	/* GPIOA Clock enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

	/* GPIOA Configuration: Channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);

	/* set up GPIO to track execution */
	/* GPIOA 9 pulses at sample rate */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Enable the TIM1 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM1 Configuration ---------------------------------------------------
	TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
	=> TIM1CLK = PCLK2 = SystemCoreClock
	TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	SystemCoreClock is set to 48 MHz for STM32F0xx devices

	Note: 
	SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
	Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	function to update SystemCoreClock variable value. Otherwise, any configuration
	based on this variable will be incorrect. 
	----------------------------------------------------------------------- */

	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1023;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 512;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	/* TIM Interrupt enable */
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*
 * Timer ISR
 */
void TIM1_CC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		
		/* decimate */
		if(++decim_ctr == DECIM_RATE)
		{
			/* reset counter */
			decim_ctr = 0;
			
			/* set diag bit */
			GPIOA->ODR |= (1<<9);

			/* fetch a sample from the LPC generator */
			TIM1->CCR1 = lpc_get_sample();
	
			/* clear diag bit */
			GPIOA->ODR &= ~(1<<9);
		}
	}
}