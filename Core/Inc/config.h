/*
 * config.h
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
#include <stdint.h>
#include <pid_ctl.h>
//#include <data_save.h>
#include <config.h>
extern int32_t kp;
extern int32_t ki;
extern int32_t kd;
extern int32_t dz;
extern int32_t zz;
extern int32_t aim_prs;
extern int32_t err_sum;
extern int32_t err_last;
extern int32_t ser_addr;
extern int8_t version;
int8_t valve_stat;

int32_t para_list[8];
void reset_para();


#endif /* INC_CONFIG_H_ */
