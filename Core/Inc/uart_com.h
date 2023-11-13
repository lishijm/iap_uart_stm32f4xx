#ifndef __UATR_COM_H__
#define __UATR_COM_H__

#include "usart.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void uart1_data_check(void);

#endif
