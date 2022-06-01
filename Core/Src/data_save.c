/*
 * data_save.c
 *
 *  Created on: Mar 2, 2022
 *      Author: wen
 */


#include <data_save.h>
#include <config.h>
#include "stdio.h"













void save_para(int32_t arg[], uint8_t len)
{



	//printf("saving data");
    uint32_t Store_Start_Add = flash_addr;
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    My_Flash.Banks = FLASH_BANK_1;
    My_Flash.TypeErase = FLASH_TYPEERASE_PAGES ;  //标明Flash执行页面只做擦除操作
    My_Flash.Page = 60; // 60 = (Store_Start_Add-0x8000000)/(2*1024);  声明要擦除的地址
    My_Flash.NbPages = 1;
    //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值
    uint32_t PageError = 0;                    //设置PageError,如果出现错误这个变量会被设置为出错的FLASH地址
    HAL_FLASHEx_Erase(&My_Flash, &PageError);  //调用擦除函数擦除,擦除PageError == 0xFFFFFFFF表示成功

    //uint16_t Write_Flash_Data = my_add;
    for(int i = 0; i < (len/2); i++)
    {
        //HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Store_Start_Add, arg[i]);
    	uint64_t flashwritedata = (uint64_t)(((uint64_t)(arg[i*2+1]) << 32) + arg[i*2]);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Store_Start_Add, flashwritedata);
        Store_Start_Add += 8;
    }
    HAL_FLASH_Lock();
}


void read_saved_para(uint8_t len)
{
    uint32_t Store_Start_Add = flash_addr;
    for(int i = 0; i < (len/2); i++)
    {
    	uint64_t tmp;
        tmp = *(__IO uint64_t*)(Store_Start_Add);  //(__IO uint16_t )是读取该地址的参数值,其值为16位数据,一次读取两个字节，(__IO uint32_t )就一次读4个字节
        para_list[i*2] = (0xFFFFFFFF & tmp);
        para_list[i*2+1] = (tmp >> 32);
        Store_Start_Add += 8;
    }

}



void para_init()
{
    //flash_addr = 0x08000000 + (0x400*120);
    flash_addr = 0x0801E000;
    //flash_addr = 0x08000000UL + (0x1E000UL); // 1024*120
	if(version == 0)
	{

		kp = 40;
		ki = 1;
		kd = 70;
		dz = 5;
		zz = 0;
		aim_prs = 200UL;
		ser_addr = 0xFFUL;
	    para_list[0] = kp;
	    para_list[1] = ki;
	    para_list[2] = kd;
	    para_list[3] = dz;
	    para_list[4] = zz;
	    para_list[5] = aim_prs;
	    para_list[6] = ser_addr;
	    para_list[7] = 0xCCUL;
	    save_para(para_list, 8);
	}else if(version == 1)
	{

	    read_saved_para(8);
	    kp = para_list[0];
	    ki = para_list[1];
	    kd = para_list[2];
	    dz = para_list[3];
	    zz = para_list[4];
	    aim_prs = para_list[5];
	    ser_addr = para_list[6];
	    para_list[7] = 0xCCUL;

	}

}
