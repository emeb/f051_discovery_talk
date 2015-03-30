/*
 * led.c - f051 discover LED setup
 */

#ifndef __led__
#define __led__

#include "stm32f0xx.h"

#define LED_GREEN (1 << 9) /* port C, pin 9 */
#define LED_BLUE (1 << 8) /* port C, pin 8 */

void led_init(void);
void led_on(uint32_t LED);
void led_off(uint32_t LED);
void led_toggle(uint32_t LED);

#endif
