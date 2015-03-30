/*
 * lpc.h
 * 03-26-2015 E. Brombaugh
 * borrows heavily from Peter Knight's Talkie library for Arduino
 * This code is released under GPLv2 license
 */

#ifndef _lpc_
#define _lpc_

#include "stm32f0xx.h"

void lpc_init(void);
uint16_t lpc_get_sample(void);
void lpc_say(uint8_t* address);
uint8_t lpc_busy(void);

#endif
