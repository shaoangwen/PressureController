/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-21     wen       the first version
 */
#ifndef APPLICATIONS_SERIAL_INTERFACE_H_
#define APPLICATIONS_SERIAL_INTERFACE_H_

#include <cmd.h>
#include <config.h>
#include "stm32l4xx_hal.h"



void uart2_init(void);
void serial2_thread_entry();

UART_HandleTypeDef huart2;
uint8_t aRxBuffer;
uint8_t sig_head;

uint16_t rx_buffer[1024];
uint16_t rx_index;
uint16_t read_index;
uint16_t data_bytes;
uint8_t rx_flag;
uint8_t tx_flag;


//struct rt_semaphore rx2_sem;
//struct rt_semaphore tx2_sem;
uint16_t wCRCCheck_Uart_Data(uint8_t *bpData, uint8_t bLen);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void read_1_byte();
//static void reading_byte_callback();
//void reading_byte();


#endif /* APPLICATIONS_SERIAL_INTERFACE_H_ */
