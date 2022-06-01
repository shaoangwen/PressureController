/*
 * I2C1.h
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */

#ifndef INC_I2C1_H_
#define INC_I2C1_H_
#include <stdint.h>
#include "stm32l4xx_hal.h"
uint8_t data_buf;
uint8_t cmd_buf; //measure command buffer, write to 0x30 register
uint8_t p1;  //8 bit MSB of 24 bit pressure
uint8_t p2;  //8 bit CSB of 24 bit pressure
uint8_t p3;  //8 bit LSB of 24 bit pressure
int32_t prs[5];    //24 bit pressure, raw value
I2C_HandleTypeDef dev;
int32_t* air_pressure();
void i2c_init(I2C_HandleTypeDef device);

#endif /* INC_I2C1_H_ */
