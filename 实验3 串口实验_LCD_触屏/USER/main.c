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


//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

u8 len;
u8 lcd_id[12];				//���LCD ID�ַ���

//�����Ļ�������Ͻ���ʾ"RST"
void Load_Drow_Dialog(void)
{
//	GBK_LibFont_Test();//�ֿ���ʾ����
	LCD_Clear(WHITE);	//����   
 	POINT_COLOR=RED;	//��������Ϊ��ɫ 
	LCD_ShowString(lcddev.width-24,0,200,16,16,RED,"RST");//��ʾ��������
  POINT_COLOR=RED;	//���û��ʺ�ɫ 
	
}

/************************************************************************/

//���败�������Ժ���
 
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

/************************************************************************/


void rtp_test(void)
{
	u8 key;
	u8 i=0;
	while(1)
	{
		
	 	key=KEY_Scan(0);
		
		tp_dev.scan(0); 	
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16) //�������������ڱ�����
					Load_Drow_Dialog();//���
				else 
					TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//��ͼ	  			   
			}
		}
		else delay_ms(10);	//û�а������µ�ʱ�� 	  
		
		if(key==KEY0_PRES)	//KEY0����,��ִ��У׼����
		{
			LCD_Clear(WHITE);  //����
		  TP_Adjust();  	   //��ĻУ׼  
			Load_Drow_Dialog();
		}
		if(USART_RX_STA&0x8000) //ͨ���ַ���������
		{					 
			
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("%s\r\n",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Read") == 0)//�Ա�����
			{
				Show_Srceen(W25QXX_Read_2Byte);
				printf("Read\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			if(strcmp(USART_RX_BUF,"Write") == 0)//�Ա�����
			{
				Save_lcd_screen(W25QXX_Write_2Byte,W25QXX_Erase_Chip);
				printf("Write\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			if(strcmp(USART_RX_BUF,"Readshowpic") == 0)//�Ա�����
			{
				myStyle_Show_Picture();
				printf("Readshowpic\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			USART_RX_STA=0;
		}
		i++;
		if(i%20==0)
		{
		  LED1=!LED1;//LED������ָʾϵͳ������
		}
		
	}
}


int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200);
	LCD_Init();
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	KEY_Init();
	W25QXX_Init();			//W25QXX��ʼ��
	POINT_COLOR=RED;      //������ɫ����ɫ	
	
	while(W25QXX_ReadID()!=W25Q16)								//��ⲻ��W25QXX
	{
		LCD_ShowString(30,150,200,16,16,RED,"W25Q16 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,RED,"Please Check!      ");
		delay_ms(500);
		LED1=!LED1;		//DS0��˸
	}
	LCD_ShowString(30,150,200,16,16,RED,"W25Q16 Ready!");
	printf("W25Q16 Ready!\r\n");
	
	GBK_Lib_Init();       //Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ����
	
	tp_dev.init();				//��������ʼ��
		
	LCD_Clear(WHITE);//�����Ļ
	
	delay_ms(1500);
 	Load_Drow_Dialog();	 	
	
	rtp_test(); 					//����������

}

