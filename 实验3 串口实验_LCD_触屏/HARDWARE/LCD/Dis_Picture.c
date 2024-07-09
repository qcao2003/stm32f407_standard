
#include "delay.h"	
#include "Dis_Picture.h" 
										    

//////////////////////////////////////////////////////////////////////////////////	 
/******************************************************************************/	 
//ͼƬ��ʾ ��������	

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
/******************************************************************************/							  


//16λ ��ֱɨ��  �ҵ���  ��λ��ǰ
void Show_Picture(void)
{
	u32 i,j,k=0;
	
	u16 picH,picL;
	
	LCD_Clear(WHITE);//����
	
	Draw_Font16B(24,16,BLUE,"3: ͼƬ��ʾ����");	
	
	delay_ms(1000);    //��ʱ��ʾ
	
	LCD_Clear(WHITE);  //����
	
	#if USE_HORIZONTAL==1	//ʹ�ú���	

	{
		
		LCD_Set_Window(0,0,320,240);//����һ���Զ�������ʾ���򴰿�

		LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
						
		for(i=0;i<240;i++)
		for(j=0;j<320;j++)
		{
			picH=gImage_LCD280[k++];
			picL=gImage_LCD280[k++];
//			
//			
//			LCD_WR_DATA8(picH);  //д8λ��ʾ����
//			LCD_WR_DATA8(picL);
			
			LCD->LCD_RAM=(picH<<8)+picL; 
			
		}			
	}	

		
	#else	
	
	{
		
		LCD_Set_Window(0,0,240,320);//����һ���Զ�������ʾ���򴰿�

		LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
						
		for(i=0;i<320;i++)
		for(j=0;j<240;j++)
		{
			picH=gImage_LCD280[k++];
			picL=gImage_LCD280[k++];
//			
//			LCD_WR_DATA8(picH);  //д8λ��ʾ����
//			LCD_WR_DATA8(picL);
			LCD->LCD_RAM=(picH<<8)+picL; 
		}	
	}

	
	#endif
	
} 





/*******************************************************************************/

//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/



//��������һ������Ȼ�����������ĺ�����ȡ���ص������
void Show_Srceen(void(*fun)(u16* lcd_point,u32 k))
{
	u32 i,j,k=0;
	u16 lcd_point;

	LCD_Clear(WHITE);  //����
	
	LCD_Set_Window(0,0,240,320);//����һ���Զ�������ʾ���򴰿�

	LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
					
	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			(*fun)(&lcd_point,k);
			LCD->LCD_RAM=lcd_point; 
			k=k+2;
		}	
	}

} 



//16λ ��ֱɨ��  �ҵ���  ��λ��ǰ
void myStyle_Show_Picture(void)
{
	u32 i,j,k=0;
	
	LCD_Clear(WHITE);  //����

	LCD_Set_Window(0,0,240,320);//����һ���Զ�������ʾ���򴰿�

	LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
					
	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			LCD->LCD_RAM=(gImage_LCD280[k]<<8)+gImage_LCD280[k+1];
			k = k + 2;
		}	
	}
}





