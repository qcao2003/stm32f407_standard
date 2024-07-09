#ifndef __UART5_H
#define __UART5_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//如果想串口中断接收，请不要注释以下宏定义
void uart5_init(u32 bound);
void u5_printf(char* fmt, ...);
void USART5_IRQHandler(void);
 
 
#endif

