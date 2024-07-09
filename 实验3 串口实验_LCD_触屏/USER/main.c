#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart2.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "GBK_LibDrive.h"	
#include "w25qxx.h"
#include "touch.h"
#include "Picture.h"
#include "Dis_Picture.h"


//ALIENTEK 探索者STM32F407开发板 实验4
//串口通信实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

u8 len;
u8 lcd_id[12];				//存放LCD ID字符串

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
//	GBK_LibFont_Test();//字库显示测试
	LCD_Clear(WHITE);	//清屏   
 	POINT_COLOR=RED;	//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,RED,"RST");//显示清屏区域
  POINT_COLOR=RED;	//设置画笔红色 
	
}

/************************************************************************/

//电阻触摸屏测试函数
 
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

/************************************************************************/


void rtp_test(void)
{
	u8 key;
	u8 i=0;
	while(1)
	{
		
	 	key=KEY_Scan(0);
		
		tp_dev.scan(0); 	
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16) //如果在这个区域内被按下
					Load_Drow_Dialog();//清除
				else 
					TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}
		else delay_ms(10);	//没有按键按下的时候 	  
		
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);  //清屏
		  TP_Adjust();  	   //屏幕校准  
			Load_Drow_Dialog();
		}
		if(USART_RX_STA&0x8000) //通过字符串下命令
		{					 
			
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("%s\r\n",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Read") == 0)//对比数组
			{
				Show_Srceen(W25QXX_Read_2Byte);
				printf("Read\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			if(strcmp(USART_RX_BUF,"Write") == 0)//对比数组
			{
				Save_lcd_screen(W25QXX_Write_2Byte,W25QXX_Erase_Chip);
				printf("Write\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			if(strcmp(USART_RX_BUF,"Readshowpic") == 0)//对比数组
			{
				myStyle_Show_Picture();
				printf("Readshowpic\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			USART_RX_STA=0;
		}
		i++;
		if(i%20==0)
		{
		  LED1=!LED1;//LED灯闪，指示系统在运行
		}
		
	}
}


int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	uart2_init(115200);
	LCD_Init();
	LED_Init();		  		//初始化与LED连接的硬件接口 
	KEY_Init();
	W25QXX_Init();			//W25QXX初始化
	POINT_COLOR=RED;      //画笔颜色：红色	
	
	while(W25QXX_ReadID()!=W25Q16)								//检测不到W25QXX
	{
		LCD_ShowString(30,150,200,16,16,RED,"W25Q16 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,RED,"Please Check!      ");
		delay_ms(500);
		LED1=!LED1;		//DS0闪烁
	}
	LCD_ShowString(30,150,200,16,16,RED,"W25Q16 Ready!");
	printf("W25Q16 Ready!\r\n");
	
	GBK_Lib_Init();       //硬件GBK字库初始化--(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库初始化）
	
	tp_dev.init();				//触摸屏初始化
		
	LCD_Clear(WHITE);//清除屏幕
	
	delay_ms(1500);
 	Load_Drow_Dialog();	 	
	
	rtp_test(); 					//电阻屏测试

}

