#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "pic.h"
#include "timer.h" 
#include "ov5640.h" 
#include "dcmi.h" 
#include "malloc.h"

//STM32F407������ 
//����ͷ ʵ�� -�⺯���汾
#define jpeg_buf_size 20*1024  			//����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)
u8 Com1SendFlag;//����1�������ݱ��
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG���ݻ���buf

int main(void)
{ 
//	u16 picH,picL;
	//u32 *p=0;
	u32 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200   �˴����ٳ�ʼ�����ڣ���������ģʽ�ٳ�ʼ��������
	
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ�� 
	//my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	//p=mymalloc(SRAMIN,10*1024);//����2K�ֽ�
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

  //rgb565(printf,delay_ms,&Com1SendFlag,(u32)&LCD->LCD_RAM);
	
	rgb565(printf,delay_ms,&Com1SendFlag,(u32)&jpeg_buf,jpeg_buf_size);
	DCMI_Stop();
	OV5640_OutSize_Set(4,0,200,200);
	DMA_Cmd(DMA2_Stream1, ENABLE);//����DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��

	//j = 0;
	//LCD_Set_Window(0,0,320,240);//����һ���Զ�������ʾ���򴰿�

	//DMA_Cmd(DMA2_Stream1, DISABLE);//ֹͣ��ǰ���� 
	//while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)//�ȴ�DMA2_Stream1������
	

	//DMA_Cmd(DMA2_Stream1, ENABLE);			//���´���
	//DCMI_CaptureCmd(ENABLE);//DCMI����ʹ�� 
	delay_ms(2000);
	DCMI_Stop();//ֻ����һ���ͼ��
	LCD_Display_Dir(1); //��������
	LCD_Set_Window(0,0,200,200);//����һ���Զ�������ʾ���򴰿�
	LCD_SetCursor(0,0);
	LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
	for(i=0;i<20*1024;i++)
	{
		//picH=Image_LCD280[j++];	
		//printf("vlaue of gImage_LCD280 is 0x%X\r\n",gImage_LCD280[j]);
		//picL=Image_LCD280[j++];
		//LCD->LCD_RAM = (picH<<8) + picL;
		LCD->LCD_RAM = ((u32)jpeg_buf[i])>>16 & 0xFFFF;
		LCD->LCD_RAM = ((u32)jpeg_buf[i]) & 0xFFFF;
		 
		printf("jpeg_buf is 0x%X\r\n",(u32)jpeg_buf[i]);
	}	
	//printf("LCD->LCD_RAM is 0x%X\r\n",(u32)&LCD->LCD_RAM);
	//printf("Value of jpeg_buf is 0x%X\r\n",(u32)*jpeg_buf); //ͼƬ��ֵ
	
}
