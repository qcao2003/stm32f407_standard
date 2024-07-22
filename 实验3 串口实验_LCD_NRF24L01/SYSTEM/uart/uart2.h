
#ifndef __UART2_H
#define __UART2_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern char USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
void u2_printf(char* fmt, ...);
void USART2_IRQHandler(void);
 
 
#endif

