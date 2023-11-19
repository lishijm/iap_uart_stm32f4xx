#include <stdio.h>
#include <string.h>
#include "main.h"
#include "usart.h"
#include "iap.h"
#include "uart_com.h"
#include "flash_if.h"
#include "ymodem.h"
#include "common.h"

__IO uint8_t uart1_rx_buf[16];
__IO uint8_t uart1_get_new_data=0;
__IO uint8_t update_flag=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1){
        uart1_get_new_data=1;
    }
}

void uart1_data_check(void)
{
    uint8_t checksum;
    if(uart1_get_new_data){
        uart1_get_new_data=0;
        if(0x55==uart1_rx_buf[0]){
            checksum=uart1_rx_buf[1];
            if(checksum==uart1_rx_buf[2]){
                switch (uart1_rx_buf[1])
                {
                case APP_UPDATE:
                    update_flag=1;
                    printf("-----APP UPDATE-----\r\n");
                    Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);
                    printf("----UPDATE ENDING---\r\n");
                    break;
                case APP_RUN:
                    printf("-----APP RUN-----\r\n");
                    LoadApplication(APP_START_ADDR);
                    break;
                default:
                    break;
                }
                memset((void *)uart1_rx_buf,0,sizeof(uart1_rx_buf));
                HAL_UART_Receive_IT(&huart1,(uint8_t *)uart1_rx_buf,3);
                return ;
            }
            memset((void *)uart1_rx_buf,0,sizeof(uart1_rx_buf));
            HAL_UART_Receive_IT(&huart1,(uint8_t *)uart1_rx_buf,3);
            return ;
        }
        memset((void *)uart1_rx_buf,0,sizeof(uart1_rx_buf));
        HAL_UART_Receive_IT(&huart1,(uint8_t *)uart1_rx_buf,3);
        return ;
    }
}
