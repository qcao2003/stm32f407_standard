#ifndef __UART5_H
#define __UART5_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart5_init(u32 bound);
void u5_printf(char* fmt, ...);
void USART5_IRQHandler(void);
 
 
#endif

