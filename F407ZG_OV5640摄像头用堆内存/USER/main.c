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

//STM32F407开发板 
//摄像头 实验 -库函数版本
#define jpeg_buf_size 20*1024  			//定义JPEG数据缓存jpeg_buf的大小(*4字节)
u8 Com1SendFlag;//串口1发送数据标记
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG数据缓存buf

int main(void)
{ 
//	u16 picH,picL;
	//u32 *p=0;
	u32 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	
	uart_init(115200);		//初始化串口波特率为115200   此处不再初始化串口，根据设置模式再初始化波特率
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化 
	//my_mem_init(SRAMIN);		//初始化内部内存池
	//p=mymalloc(SRAMIN,10*1024);//申请2K字节
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

  //rgb565(printf,delay_ms,&Com1SendFlag,(u32)&LCD->LCD_RAM);
	
	rgb565(printf,delay_ms,&Com1SendFlag,(u32)&jpeg_buf,jpeg_buf_size);
	DCMI_Stop();
	OV5640_OutSize_Set(4,0,200,200);
	DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能

	//j = 0;
	//LCD_Set_Window(0,0,320,240);//设置一个自动换行显示区域窗口

	//DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
	//while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)//等待DMA2_Stream1可配置
	

	//DMA_Cmd(DMA2_Stream1, ENABLE);			//重新传输
	//DCMI_CaptureCmd(ENABLE);//DCMI捕获使能 
	delay_ms(2000);
	DCMI_Stop();//只传输一侦的图像
	LCD_Display_Dir(1); //横屏方向
	LCD_Set_Window(0,0,200,200);//设置一个自动换行显示区域窗口
	LCD_SetCursor(0,0);
	LCD_WriteRAM_Prepare();     	//开始写入GRAM	
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
	//printf("Value of jpeg_buf is 0x%X\r\n",(u32)*jpeg_buf); //图片的值
	
}
