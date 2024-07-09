#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
/*************************��Ҫ�޸ĵĵط�*************************/
#define OV7670_XCLK_RCC		RCC_AHB1Periph_GPIOA
#define OV7670_XCLK_Pin		GPIO_Pin_8
#define OV7670_XCLK_GPIO	GPIOA
#define STM32_MCO1_DIV		RCC_MCO1Div_4

#define OV7670_SCCB_RCC		RCC_AHB1Periph_GPIOB
#define OV7670_SCCB_Pin		GPIO_Pin_12|GPIO_Pin_13
#define OV7670_SCCB_GPIO	GPIOB

//IO��������
#define SCCB_SDA_IN()  		{GPIOB->MODER&=~(0x0003<<12*2);GPIOB->MODER|=(0x0000<<12*2);}	//PB12 ���� �Ĵ�������2*12=24λ
#define SCCB_SDA_OUT() 		{GPIOB->MODER&=~(0x0003<<12*2);GPIOB->MODER|=(0x0001<<12*2);} 	//PB12 ���


//IO��������	 
#define SCCB_SCL    		PBout(13)	 	//SCL
#define SCCB_SDA    		PBout(12) 		//SDA	 

#define SCCB_READ_SDA   	PBin(12)  		//����SDA    
#define SCCB_ID   			0X42  			//OV7670��ID


void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif













