#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

#include "timer.h" 
#include "ov5640.h" 
#include "dcmi.h" 



//STM32F407开发板 
//摄像头 实验 -库函数版本

u8 Com1SendFlag;//串口1发送数据标记

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	
	uart_init(115200);		//初始化串口波特率为115200   此处不再初始化串口，根据设置模式再初始化波特率
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化 

	TIM3_Int_Init(10000 - 1, 84 - 1); //设置定时器的定时频率为10ms  1秒钟中断100次
	
	
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"STM32F4xx");	
	LCD_ShowString(30,70,200,16,16,"OV5640 TEST");	
	LCD_ShowString(30,90,200,16,16,"mcudev.taobao.com");
	LCD_ShowString(30,110,200,16,16,"2023/11");  
	
	while(OV5640_Init())//初始化OV5640
	{
		LCD_ShowString(30,130,240,16,16,"OV5640 ERR");
		delay_ms(200);
	  LCD_Fill(30,130,239,170,WHITE);
		delay_ms(200);
		LED0=!LED0;
	}
	LCD_ShowString(30,130,200,16,16,  "OV5640 OK");  	

  
 	while(1)
	{	
		rgb565(printf,delay_ms,&Com1SendFlag);
	}
	
}
