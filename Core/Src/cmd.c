/*
 * cmd.c
 *
 *  Created on: Mar 3, 2022
 *      Author: wen
 */


#include <cmd.h>
#include <config.h>
#include <delay.h>
#include <cute_dog.h>
#include <data_save.h>
#include <pid_ctl.h>

void reply_init(void)
{
    reply_cmd[0] = 0x5E;
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = 0x5EU;
    reply_cmd[3] = 0x5EU;
    reply_cmd[4] = 0x5EU;
    reply_cmd[5] = 0x5EU;
    reply_cmd[6] = 0x5EU;
    reply_cmd[7] = 0x5EU;
}


void cmd_deliver(uint8_t arg[], uint8_t len)
{
    HAL_IWDG_Refresh(&hiwdg);
    if((arg[2]==0xFFU) && (arg[3]==0xFFU) && (arg[4]==0xFFU) && (arg[5]==0xFFU) && (arg[6]==0xAAU) && (arg[7]==0xBBU))
    {
        reset_para();
    }
    switch(arg[2])
    {
        case 0x16: pid_handle(arg); break;
        case 0x17: dz_handle(arg); break;
        case 0x18: zz_handle(arg); break;
        case 0x19: prs_handle(arg); break;
        case 0x1B: addr_handle(arg); break;
        case 0x20: valve_handle(arg); break;
    }

}

void valve_handle(uint8_t arg[])
{
	switch(arg[3])
	    {
	        case 0x80: set_valve(arg); break;
	        case 0x00: report_valve(arg); break;
	    }
}

void report_valve()
{
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = 0x20;
    reply_cmd[3] = 0x40;
    reply_cmd[4] = valve_stat;
    reply_cmd[5] = 0x00;
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void set_valve(uint8_t arg[])
{
	switch(arg[4])
	    {
	        case 0x01: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); valve_stat = 1; report_valve(); break;
	        case 0x00: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); valve_stat = 0; report_valve(); break;
	    }
}



void reset_para()
{


    kp = 40;
    ki = 1;
    kd = 70;
    dz = 5; //hPa
    zz = 0; //hPa
    aim_prs = 100; //hPa
    ser_addr = 0x0A;
    para_list[0] = kp;
    para_list[1] = ki;
    para_list[2] = kd;
    para_list[3] = dz;
    para_list[4] = zz;
    para_list[5] = aim_prs;
    para_list[6] = ser_addr;
    para_list[7] = 0xCC; //unused
    save_para(para_list, 8);
    pid_init();

    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = 0xAA;
    reply_cmd[3] = 0xBB;
    reply_cmd[4] = 0xCC;
    reply_cmd[5] = 0xDD;
    reply_cmd[6] = 0xEE;
    reply_cmd[7] = 0xFF;
    reply(&huart2);

}



void pid_handle(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x81: pid_set(arg); break;
        case 0x82: pid_set(arg); break;
        case 0x83: pid_set(arg); break;
        case 0x01: pid_report(arg); break;
        case 0x02: pid_report(arg); break;
        case 0x03: pid_report(arg); break;
    }
}

void pid_set(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x81: kp = (arg[5]*256+arg[4])/10; para_list[0] = kp; break;
        case 0x82: ki = (arg[5]*256+arg[4])/10; para_list[1] = ki; break;
        case 0x83: kd = (arg[5]*256+arg[4])/10; para_list[2] = kd; break;
    }



    save_para(para_list, 8);


    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]-0x40U;
    reply_cmd[4] = arg[4];
    reply_cmd[5] = arg[5];
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}


void pid_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    int32_t tmp = 0;
    switch(arg[3])
    {
        case 0x01: tmp = kp*10; break;
        case 0x02: tmp = ki*10; break;
        case 0x03: tmp = kd*10; break;
    }

    reply_cmd[4] = tmp & 0xFFU;
    reply_cmd[5] = ((tmp >> 8) & 0xFFU);

    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void dz_handle(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x80: dz_set(arg); break;
        case 0x00: dz_report(arg); break;
    }
}



void dz_set(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    reply_cmd[1] = 0xFF & ser_addr;
    dz = (arg[5]*256+arg[4])/10;


    para_list[3] = dz;
    save_para(para_list, 8);

    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]-0x40U;
    reply_cmd[4] = arg[4];
    reply_cmd[5] = arg[5];
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void dz_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = dz & 0xFFU;
    reply_cmd[5] = ((dz >> 8) & 0xFFU);
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}


void zz_handle(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x80: zz_set(arg); break;
        case 0x00: zz_report(arg); break;
    }
}

void zz_set(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    zz = (arg[5]*256+arg[4])/10;


    para_list[4] = zz;
    save_para(para_list, 8);


    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = arg[4];
    reply_cmd[5] = arg[5];
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void zz_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = zz & 0xFFU;
    reply_cmd[5] = ((zz >> 8) & 0xFFU);
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}


void prs_handle(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x80: prs_set(arg); break;
        case 0x00: prs_report(arg); break;
        case 0x0A: prs_set_point_report(arg); break;
    }
}

void prs_set(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    aim_prs = (arg[5]*256+arg[4]);



    para_list[5] = aim_prs;
    save_para(para_list, 8);


    pid_init();
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]-0x40U;
    reply_cmd[4] = arg[4];
    reply_cmd[5] = arg[5];
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void prs_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    int32_t rpt_prs = 0;
    rpt_prs = curr_prs/10;  //unit of rpt_prs is hPa, curr_prs is dPa
    //reply_cmd[0] = 0x5E;
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = rpt_prs & 0xFFU;
    reply_cmd[5] = ((rpt_prs >> 8) & 0xFFU);
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void  prs_set_point_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    int32_t rpt_prs = 0;
    rpt_prs = aim_prs;  //unit of rpt_prs is hPa, aim_prs is dPa
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = rpt_prs & 0xFFU;
    reply_cmd[5] = ((rpt_prs >> 8) & 0xFFU);
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}



void addr_handle(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    switch(arg[3])
    {
        case 0x80: addr_set(arg); break;
        case 0x00: addr_report(arg); break;
    }
}

void addr_set(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    ser_addr = (arg[5]*256+arg[4]);
    para_list[6] = ser_addr;
    save_para(para_list, 8);
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]-0x40U;
    reply_cmd[4] = arg[4];
    reply_cmd[5] = arg[5];
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}

void addr_report(uint8_t arg[])
{
    HAL_IWDG_Refresh(&hiwdg);
    reply_cmd[1] = 0xFF & ser_addr;
    reply_cmd[2] = arg[2];
    reply_cmd[3] = arg[3]+0x40U;
    reply_cmd[4] = ser_addr & 0xFFU;
    reply_cmd[5] = ((ser_addr >> 8) & 0xFFU);
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&reply_cmd[0], 4);
    reply_cmd[6] = (wReCRC & 0xFFU);
    reply_cmd[7] = ((wReCRC >> 8) & 0xFFU);
    reply(&huart2);
}




/*void reply(void)
{
    while(tx_flag<1)
    {rt_hw_us_delay(10);}
    tx_flag--;
    HAL_UART_Transmit(&huart2, &reply_cmd[0], 8, 2);

    tx_flag = 1;

}*/
void reply(UART_HandleTypeDef *huart)
{
    //HAL_IWDG_Refresh(&hiwdg);
    while(tx_flag<1)
    {delay_us(50);}
    tx_flag--;
    //for(int i = 0; i<8;)
    //{
        //HAL_UART_Transmit(&huart2, &reply_cmd[i], 1, 1);
        //i++;
    //}
    //while((huart->Instance->ISR & USART_ISR_TC) == 0)
    while((huart->Instance->ISR & USART_ISR_BUSY) != 0)
    {

        delay_us(50);
    }
    HAL_UART_Transmit(&huart2, &reply_cmd[0], 8, 2);
    //HAL_UART_Transmit_DMA(&huart2, &reply_cmd[0], 8);
    //while(HAL_UART_Transmit(&huart2, &reply_cmd[0], 8, 2) != HAL_OK){HAL_Delay(1);HAL_IWDG_Refresh(&hiwdg);};
    //SET_BIT(huart->Instance->ICR, USART_ICR_TCCF);

    tx_flag = 1;

    //HAL_UART_Transmit(&huart2, &reply_cmd[0], 8, 1);

}
