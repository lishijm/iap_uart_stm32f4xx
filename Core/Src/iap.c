#include "main.h"
#include <stdio.h>
#include "flash_if.h"
#include "iap.h"

pFunction JumpToApplication;
uint32_t JumpAddress;

void LoadApplication(uint32_t AppAddr){

    /*检查栈顶地址是否合法*/
	if(((*(__IO uint32_t*)AppAddr) & 0x2FFF0000 ) == 0x20000000){

        printf("%x\r\n",AppAddr);

        JumpAddress = *(__IO uint32_t*) (AppAddr + 4);
        
        /* 首地址是MSP，地址+4是复位中断服务程序地址 */
		JumpToApplication=(pFunction)*(__IO uint32_t*)(AppAddr+4);

        /* 关闭全局中断 */
        __set_PRIMASK(1);
                 
        /* 关闭滴答定时器，复位到默认值*/
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;
        
        /* 设置所有时钟到默认状态 */
        HAL_RCC_DeInit();
        
        /* 关闭所有中断，清除所有中断挂起标志 */  
        for (int counter = 0; counter < 8; counter++){
            NVIC->ICER[counter]=0xFFFFFFFF;
            NVIC->ICPR[counter]=0xFFFFFFFF;
        }
        
        /* 使能全局中断 */ 
        __set_PRIMASK(0);
        
        /* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
        __set_CONTROL(0);

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*)AppAddr);
        
        /*跳转到Application*/
		JumpToApplication();
	}
}
