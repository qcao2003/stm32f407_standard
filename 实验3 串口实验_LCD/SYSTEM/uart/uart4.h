#ifndef __UART4_H
#define __UART4_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//如果想串口中断接收，请不要注释以下宏定义
void uart4_init(u32 bound);
void u4_printf(char* fmt, ...);
void USART4_IRQHandler(void);
 
 
#endif
