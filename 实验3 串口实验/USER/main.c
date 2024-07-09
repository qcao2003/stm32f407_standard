#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart2.h"
#include "led.h"
#include "key.h"


//ALIENTEK 探索者STM32F407开发板 实验4
//串口通信实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

u8 len;

int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	uart2_init(115200);
	LED_Init();		  		//初始化与LED连接的硬件接口  
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					 
			
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("%s\r\n",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF") == 0)//对比数组
			{
				printf("OFF\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			USART_RX_STA=0;
		}
		if(USART2_RX_STA&0x8000)
		{					 
			
			len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			u2_printf("%s\r\n",USART2_RX_BUF);
			if(strcmp(USART2_RX_BUF,"LED_OFF") == 0)//对比数组
			{
				u2_printf("OFF\r\n");
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//清空数组内存
			}
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//清空数组内存
			USART2_RX_STA=0;
		}
	}
}

