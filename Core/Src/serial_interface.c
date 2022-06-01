/*
 * Copyright (c) Dr. Wen
 *

 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-03     wen       the first version
 */

#include <cute_dog.h>
#include <serial_interface.h>
#include <delay.h>
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal.h"
#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"

uint16_t buff_size = 1024;









void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);

    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_PE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_FE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_NE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_ORE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_CMF);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_BUSY);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TXE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_IDLE);

    //__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_);
    if(huart == &huart2){
    //if(huart->Instance == USART2 ){
    	if(rx_index < buff_size)
    	{
    		rx_buffer[rx_index] = aRxBuffer;
    		rx_index++;
    		rx_flag++;
    	}else{
    		rx_index = 0;
    		rx_buffer[rx_index] = aRxBuffer;
    		rx_index++;
    		rx_flag++;

    	}
    	data_bytes ++;
/*
    if((huart->Instance->ISR & USART_ISR_RXNE) != 0U)
    {

        if(rx_index < buff_size)
        {
            rx_buffer[rx_index] = huart->Instance->RDR;
            rx_index++;
            rx_flag++;
        }else{
            rx_index = 0;
            rx_buffer[rx_index] = huart->Instance->RDR;
            rx_index++;
            rx_flag++;
        }

        ////huart->Instance->RQR |= USART_RQR_RXFRQ;
        //SET_BIT(huart->Instance->RQR, USART_RQR_RXFRQ);
        //SET_BIT(huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);
    }*/
    	//HAL_UART_Receive_IT(&huart2, (uint8_t*)&aRxBuffer, 1);
    	while(HAL_UART_Receive_IT(&huart2, (uint8_t*)&aRxBuffer, 1)!=HAL_OK){osDelay(1);};

    	//HAL_UART_Receive_DMA(&huart2, (uint8_t*)&aRxBuffer, 1);

    	//HAL_IWDG_Refresh(&hiwdg);

    }
}

void HAL_UART_ErrorCallback (UART_HandleTypeDef * huart)
{
	if(huart == &huart2)
	{
		printf("uart2 err \r\n");
	}
	//HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE);

}


//static int read_1_byte(uint8_t *buffer)
void read_1_byte()
{
		//while(HAL_UART_Receive_DMA(&huart2, (uint8_t*)&aRxBuffer, 1)!=HAL_OK){osDelay(1);};
        while(rx_flag<1)
        {delay_us(100);HAL_IWDG_Refresh(&hiwdg);}
        //{delay_us(100);}
        rx_flag--;

        if(read_index < buff_size)
        {
            //buffer = &rx_buffer[read_index];
            sig_head = rx_buffer[read_index];
            read_index++;
        }else{
            read_index = 0;
            //buffer = &rx_buffer[read_index];
            sig_head = rx_buffer[read_index];
            read_index++;
        }

        data_bytes--;
}


//static void post_read_1_byte(uint8_t *buff){}








void serial2_thread_entry()
{
    //uint8_t sig_head;
    uint8_t cmd_words[8];
    cmd_words[0] = 0x5EU;
    //uint8_t test_words[3] = {0xAA, 0xBB, 0xCC};

    while (1)
    {


        int i = 0;
        //read_1_byte(&huart2, &sig_head);
        //while (read_1_byte(&sig_head) != 1)
        /*while (read_1_byte() != 1)
        {++++++++++++++++++++++++++++
            rt_sem_take(&rx2_sem, RT_WAITING_FOREVER);
        }*/
        //HAL_UART_Transmit(&huart2, &test_words[0], 3, 2);
        read_1_byte();
        HAL_IWDG_Refresh(&hiwdg);
        //rt_kprintf("10ok");
        if (sig_head == 0x5EU)
        {
            cmd_words[0] = sig_head;
            i = 1;

            while(i < sizeof(cmd_words)/sizeof(uint8_t)){
                //read_1_byte(&sig_head);
                read_1_byte();
                cmd_words[i] = sig_head;
                i +=1 ;

            }
            i -= 1;
            if((cmd_words[1] == ser_addr) || (cmd_words[1] == 0xFFU))
            {
                uint16_t wReCRC = 0;
                wReCRC = wCRCCheck_Uart_Data(&cmd_words[0], 4);

                if (((cmd_words[6] == (wReCRC & 0xFFU)) && (cmd_words[7] == ((wReCRC >> 8) & 0xFFU))) || ((cmd_words[6] == 0xAAU) && (cmd_words[7] == 0xBBU)))

                {
                    cmd_deliver(cmd_words, sizeof(cmd_words)/sizeof(cmd_words[0]));
                }else{//CRC error
                    cmd_words[3] = 0xFFU;
                    cmd_words[4] = 0x07U;
                    while(tx_flag<1)
                    //{delay_us(50); HAL_IWDG_Refresh(&hiwdg);}
                    {delay_us(50);}
                    tx_flag--;
                    //rt_sem_take(&tx2_sem, RT_WAITING_FOREVER);
                    //HAL_UART_Transmit_DMA(&huart2, &cmd_words[0], 8);
                    HAL_UART_Transmit(&huart2, &cmd_words[0], 8, 2);
                    //while(HAL_UART_Transmit(&huart2, &cmd_words[0], 8, 2) != HAL_OK){osDelay(1); HAL_IWDG_Refresh(&hiwdg);};
                    //rt_thread_mdelay(1);
                    tx_flag = 1;
                    //rt_sem_release(&tx2_sem);
                }
            }else{
                //rt_sem_take(&tx2_sem, RT_WAITING_FOREVER);
                //rt_thread_mdelay(5);
                //int dog = 0;
                //while((HAL_UART_Transmit(&huart2, &cmd_words[0], 8, 1) != HAL_OK) && dog<7)
                //while(dog<7)
                //{
                while(tx_flag<1)
                //{delay_us(50); HAL_IWDG_Refresh(&hiwdg);}
                {delay_us(50);}
                tx_flag--;
                HAL_UART_Transmit(&huart2, &cmd_words[0], 8, 2);
                //HAL_UART_Transmit_DMA(&huart2, &cmd_words[0], 8);
                //while(HAL_UART_Transmit(&huart2, &cmd_words[0], 8, 2) != HAL_OK){osDelay(1); HAL_IWDG_Refresh(&hiwdg);};
                //rt_thread_mdelay(1);
                tx_flag = 1;

                //    dog++;
                //}
                //rt_sem_release(&tx2_sem);
            }
        }
        i=0;
    }
}






void uart2_init(void){

    rx_index = 0;
    read_index = 0;
    data_bytes = 0;
    rx_flag = 0;
    tx_flag = 1;
}


uint16_t wCRCCheck_Uart_Data(uint8_t *bpData, uint8_t bLen)
{
    uint8_t i, j;
    uint8_t bTemp = 0;
    uint16_t wResulrCrc = 0xffff;
    if (bLen == 0) {
        goto Exit;
    }

    for (i = 1; i < bLen; i++) {
        bTemp = *(bpData + 2 + i);  //bpData[3:6) (3 bytes) were used to perform CRC
        for (j = 0; j < 8; j++) {
            if (((wResulrCrc ^ bTemp) & 0x0001) != 0)
                    {
                wResulrCrc >>= 1;
                wResulrCrc ^= 0x1021;
            } else {
                wResulrCrc >>= 1;
            }
            bTemp >>= 1;
        }
    }
    Exit: return wResulrCrc + 1;
}








































