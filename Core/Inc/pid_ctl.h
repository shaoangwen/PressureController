/*
 * pid_ctl.h
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */

#ifndef INC_PID_CTL_H_
#define INC_PID_CTL_H_
#include <stdint.h>

int32_t curr_prs;    //dPa
extern int32_t aim_prs; //hPa
extern int32_t kp;
extern int32_t ki;
extern int32_t kd;
extern int32_t dz;   //hPa
extern int32_t zz;   //hPa
int32_t err_curr;
int32_t err_sum;
int32_t err_last;
int32_t pwm_value;

void pid_run(void);
void pid_init(void);
#endif /* INC_PID_CTL_H_ */
