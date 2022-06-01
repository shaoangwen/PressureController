/*
 * Copyright (c) 2006-2021, Dr.Wen
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Shao'ang Wen       the first version
 */

#ifndef APPLICATIONS_CONFIG_C_
#define APPLICATIONS_CONFIG_C_

#include <config.h>
int32_t kp;
int32_t ki;
int32_t kd;
int32_t dz; //hPa
int32_t zz; //hPa
int32_t aim_prs; //hPa
int32_t err_sum = 0;
int32_t err_last = 0;
int32_t ser_addr;

//int8_t valve_stat;

int8_t version = 1;

#endif /* APPLICATIONS_CONFIG_C_ */

