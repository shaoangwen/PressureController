/*
 * cute_dog.h
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */

#ifndef INC_CUTE_DOG_H_
#define INC_CUTE_DOG_H_

#include "stm32l4xx_hal.h"
//#include "stm32f0xx_hal_iwdg.h"

void soft_reset(void);

//#define __HAL_IWDG_START(__HANDLE__)                WRITE_REG((__HANDLE__)->Instance->KR, IWDG_KEY_ENABLE)
IWDG_HandleTypeDef hiwdg;

void feed_dog(void);



#endif /* INC_CUTE_DOG_H_ */
