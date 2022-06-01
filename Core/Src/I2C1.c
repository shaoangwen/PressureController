/*
 * I2C.c
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */
#include<I2C1.h>

#include <main.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"
#define pressure_sensor_addr 0x6D  //address of sensor


void i2c_init(I2C_HandleTypeDef device)
{
	dev = device;
}



static void write_reg(I2C_HandleTypeDef bus, uint16_t reg, uint8_t *data)
{
	HAL_I2C_Mem_Write(&bus, (pressure_sensor_addr << 1), reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xff);
}

static uint8_t read_regs(I2C_HandleTypeDef bus, uint16_t reg)
{
	uint8_t buf;
	HAL_I2C_Mem_Read(&bus, ((pressure_sensor_addr << 1)+1), reg, I2C_MEMADD_SIZE_8BIT, &buf, 1, 0xff);
    return buf;
}

int32_t* air_pressure(){
    // read the calibrate value and write back after &0xFD
    data_buf = 0x00;
    data_buf = read_regs(dev, 0xA5);
    //rt_kprintf("data_buf is %x \r\n", data_buf);
    data_buf =  data_buf & 0xFD;
    write_reg(dev, 0xA5, &data_buf);

    cmd_buf = 0x0A; //measure command, once temperature and once pressure
    write_reg(dev, 0x30, &cmd_buf);      //send command to 0x30 registor
    osDelay(10);
    cmd_buf = read_regs(dev, 0x30); //read the status of measurement, cmd_buf & 0x80 == 0 indicates the finish of measurement
    //rt_kprintf("(cmd_buf & 0x08) is %x \r\n", (cmd_buf & 0x08));
    if((cmd_buf & 0x08) > 0){
        while ((read_regs(dev, 0x30) & 0x08) > 0){
            osDelay(10);
        }
    }
    p1 = read_regs(dev, 0x06);
    //rt_kprintf("p1 is %x \r\n", p1);
    p2 = read_regs(dev, 0x07);
    //rt_kprintf("p2 is %x \r\n", p2);
    p3 = read_regs(dev, 0x08);
    //rt_kprintf("p3 is %x \r\n", p3);
    prs[0] = p1*65536+p2*256+p3;
    prs[1] = p1;
    prs[2] = p2;
    prs[3] = p3;
    //printf("reading pressure  \r\n");
   //printf("raw pressure is %d  \r\n", prs[0]);
    int32_t prs_kpa;
    if (prs[0] > 8388608){
        //prs_kpa = ((rt_int32_t)prs[0] - 16777216)/32/1000;
        prs[4] = ((int32_t)prs[0] - 16777216)/64/10;//for range is 0-100Kpa, K is 64
        prs_kpa = prs[4]/100;
        //printf("vacuum pressure is dPa \r\n");
        //printf(prs);
        //return prs_kpa;
        return prs;
    }else {
        //prs_kpa = prs/32/1000;
        prs[4] = prs[0]/64/10;
        prs_kpa = prs[4]/100;
       // printf("pressure is dPa \r\n");
        //return prs_kpa;
        return prs;// returned in dPa
    }
}
