#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart2.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "GBK_LibDrive.h"	


//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

u8 len;
u8 lcd_id[12];				//���LCD ID�ַ���

int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200);
	LCD_Init();
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	POINT_COLOR=RED;      //������ɫ����ɫ	
	GBK_Lib_Init();       //Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ����
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id����
	while(1)
	{
		Demo_Menu();//��ʾ����
		if(USART_RX_STA&0x8000)
		{					 
			
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("%s\r\n",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF") == 0)//�Ա�����
			{
				printf("OFF\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			USART_RX_STA=0;
		}
		if(USART2_RX_STA&0x8000)
		{					 
			
			len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			u2_printf("%s\r\n",USART2_RX_BUF);
			if(strcmp(USART2_RX_BUF,"LED_OFF") == 0)//�Ա�����
			{
				u2_printf("OFF\r\n");
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
			}
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
			USART2_RX_STA=0;
		}
		delay_ms(1000);
	}
}

