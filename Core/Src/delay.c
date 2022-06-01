/*
 * delay.c
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */
#include <delay.h>
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"


void delay_us(__IO uint32_t delay)
{
	//not stable, use 1ms delay instead;
	osDelay(1);
	/*
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
             while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }*/

}


