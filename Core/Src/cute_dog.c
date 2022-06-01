/*
 * cute_dog.c
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */

#include <cute_dog.h>





void feed_dog(void)
{
    WRITE_REG((&hiwdg)->Instance->KR,0x0000CCCCu);
    while(1)
    {
        HAL_IWDG_Refresh(&hiwdg);
        HAL_Delay(1);
        //rt_hw_us_delay(20);
    }
}

