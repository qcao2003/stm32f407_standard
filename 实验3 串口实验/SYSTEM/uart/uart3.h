#ifndef __UART3_H
#define __UART3_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//如果想串口中断接收，请不要注释以下宏定义
void uart3_init(u32 bound);
void u3_printf(char* fmt, ...);
void USART3_IRQHandler(void);
 
 
#endif

