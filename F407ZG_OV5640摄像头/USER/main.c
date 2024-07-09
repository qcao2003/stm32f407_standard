#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

#include "timer.h" 
#include "ov5640.h" 
#include "dcmi.h" 



//STM32F407������ 
//����ͷ ʵ�� -�⺯���汾

u8 Com1SendFlag;//����1�������ݱ��

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200   �˴����ٳ�ʼ�����ڣ���������ģʽ�ٳ�ʼ��������
	
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ�� 

	TIM3_Int_Init(10000 - 1, 84 - 1); //���ö�ʱ���Ķ�ʱƵ��Ϊ10ms  1�����ж�100��
	
	
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"STM32F4xx");	
	LCD_ShowString(30,70,200,16,16,"OV5640 TEST");	
	LCD_ShowString(30,90,200,16,16,"mcudev.taobao.com");
	LCD_ShowString(30,110,200,16,16,"2023/11");  
	
	while(OV5640_Init())//��ʼ��OV5640
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
