#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart2.h"
#include "led.h"
#include "key.h"


//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

u8 len;

int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200);
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	while(1)
	{
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
	}
}

