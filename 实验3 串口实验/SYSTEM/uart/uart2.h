
#ifndef __UART2_H
#define __UART2_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern char USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart2_init(u32 bound);
void u2_printf(char* fmt, ...);
void USART2_IRQHandler(void);
 
 
#endif

