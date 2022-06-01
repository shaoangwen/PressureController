/*
 * Copyright (c) 2006-2021, Dr.Wen
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Shao'ang Wen       the first version
 */
#include "stm32l4xx_hal.h"
#include <pid_ctl.h>
#include <config.h>
#include <stdlib.h>
#include <I2C1.h>
#include <stdio.h>
#include <cute_dog.h>


int32_t *prs_list;


void pid_init(void)
{
    TIM2->CCR1 = 99;
    TIM2->CCR2 = 99;
    err_curr = 0;
    err_sum = 0;
    err_last = 0;
    pwm_value = 0;
}


void pid_run(void)
{

    prs_list = air_pressure();
    curr_prs = prs_list[4]; //unit is dPa
    err_curr = aim_prs*10 - curr_prs; //unit is dPa
    if (abs(err_curr) <= dz*10)
    {
        pid_init();
    }
    else
    {
        err_sum = err_sum + err_curr;

        pwm_value =(int32_t)(kp*err_curr/10 + ki*err_sum/1000000 + kd*(err_curr-err_last)/10);
        //HAL_IWDG_Refresh(&hiwdg);


        if(abs(pwm_value) > 100)
        {
            if(pwm_value > 0)
            {
                pwm_value = 100;
            }else if(pwm_value < 0)
            {
                pwm_value = -100;
            }
        }
        //else if(pwm_value < 0)
        //{
        //        pwm_value = 0;
        //}


        /*if(pwm_value > 85)
        {
            pwm_value = 90;
        }
        else if(pwm_value < 25)
        {
            pwm_value = 1;
        }*/
        err_last = err_curr;
        if(pwm_value > 0 )//NORMAL > 0
        {
            pwm_value = abs(pwm_value);
            pwm_value = pwm_value*6/10+25;

            TIM2->CCR1 = 99-(pwm_value);//air in
            TIM2->CCR2 = 99 ;
        }
        else if (pwm_value < 0 )
        {
            pwm_value = abs(pwm_value);
            pwm_value = pwm_value*6/10+25;

            TIM2->CCR1 = 99;
            TIM2->CCR2 = 99-(pwm_value);//air release
        }
        else
        {
            TIM2->CCR1 = 99;
            TIM2->CCR2 = 99;
        }

    }
    HAL_IWDG_Refresh(&hiwdg);
    //printf("pid_running \r\n");
}
