/*
 * data_save.h
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */

#ifndef INC_DATA_SAVE_H_
#define INC_DATA_SAVE_H_
#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include <config.h>

uint32_t flash_addr;
FLASH_EraseInitTypeDef My_Flash;

void save_para(int32_t arg[], uint8_t len);
void read_saved_para(uint8_t len);
void para_init();


#endif /* INC_DATA_SAVE_H_ */
