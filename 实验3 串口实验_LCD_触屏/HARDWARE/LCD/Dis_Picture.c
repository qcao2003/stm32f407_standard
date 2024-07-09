
#include "delay.h"	
#include "Dis_Picture.h" 
										    

//////////////////////////////////////////////////////////////////////////////////	 
/******************************************************************************/	 
//图片显示 驱动代码	

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
/******************************************************************************/							  


//16位 垂直扫描  右到左  高位在前
void Show_Picture(void)
{
	u32 i,j,k=0;
	
	u16 picH,picL;
	
	LCD_Clear(WHITE);//清屏
	
	Draw_Font16B(24,16,BLUE,"3: 图片显示测试");	
	
	delay_ms(1000);    //延时显示
	
	LCD_Clear(WHITE);  //清屏
	
	#if USE_HORIZONTAL==1	//使用横屏	

	{
		
		LCD_Set_Window(0,0,320,240);//设置一个自动换行显示区域窗口

		LCD_WriteRAM_Prepare();     	//开始写入GRAM	
						
		for(i=0;i<240;i++)
		for(j=0;j<320;j++)
		{
			picH=gImage_LCD280[k++];
			picL=gImage_LCD280[k++];
//			
//			
//			LCD_WR_DATA8(picH);  //写8位显示数据
//			LCD_WR_DATA8(picL);
			
			LCD->LCD_RAM=(picH<<8)+picL; 
			
		}			
	}	

		
	#else	
	
	{
		
		LCD_Set_Window(0,0,240,320);//设置一个自动换行显示区域窗口

		LCD_WriteRAM_Prepare();     	//开始写入GRAM	
						
		for(i=0;i<320;i++)
		for(j=0;j<240;j++)
		{
			picH=gImage_LCD280[k++];
			picL=gImage_LCD280[k++];
//			
//			LCD_WR_DATA8(picH);  //写8位显示数据
//			LCD_WR_DATA8(picL);
			LCD->LCD_RAM=(picH<<8)+picL; 
		}	
	}

	
	#endif
	
} 





/*******************************************************************************/

//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/



//参数输入一个函数然后用这个输入的函数获取像素点的数据
void Show_Srceen(void(*fun)(u16* lcd_point,u32 k))
{
	u32 i,j,k=0;
	u16 lcd_point;

	LCD_Clear(WHITE);  //清屏
	
	LCD_Set_Window(0,0,240,320);//设置一个自动换行显示区域窗口

	LCD_WriteRAM_Prepare();     	//开始写入GRAM	
					
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



//16位 垂直扫描  右到左  高位在前
void myStyle_Show_Picture(void)
{
	u32 i,j,k=0;
	
	LCD_Clear(WHITE);  //清屏

	LCD_Set_Window(0,0,240,320);//设置一个自动换行显示区域窗口

	LCD_WriteRAM_Prepare();     	//开始写入GRAM	
					
	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			LCD->LCD_RAM=(gImage_LCD280[k]<<8)+gImage_LCD280[k+1];
			k = k + 2;
		}	
	}
}





