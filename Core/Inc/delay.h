/*
 * delay.h
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_
#include "stm32l4xx_hal.h"
#define CPU_FREQUENCY_MHZ    80		// STM32时钟主频
void delay_us(__IO uint32_t delay);

#endif /* INC_DELAY_H_ */
