/*
 * cmd.h
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */

#ifndef INC_CMD_H_
#define INC_CMD_H_

#include <serial_interface.h>

#include <config.h>
#include "stm32l4xx_hal.h"

uint8_t reply_cmd[8];
void cmd_deliver(uint8_t arg[], uint8_t len);

void pid_handle(uint8_t arg[]);
void pid_set(uint8_t arg[]);
void pid_report(uint8_t arg[]);

void dz_handle(uint8_t arg[]);
void dz_set(uint8_t arg[]);
void dz_report(uint8_t arg[]);

void zz_handle(uint8_t arg[]);
void zz_set(uint8_t arg[]);
void zz_report(uint8_t arg[]);


void prs_handle(uint8_t arg[]);
void prs_set(uint8_t arg[]);
void prs_report(uint8_t arg[]);
void  prs_set_point_report(uint8_t arg[]);

void addr_handle(uint8_t arg[]);
void addr_set(uint8_t arg[]);
void addr_report(uint8_t arg[]);

void valve_handle(uint8_t arg[]);
void report_valve();
void set_valve(uint8_t arg[]);



void reply_init(void);
void reply(UART_HandleTypeDef *huart);



#endif /* INC_CMD_H_ */
