#ifndef __Dis_Picture_H__
#define __Dis_Picture_H__


#include "lcd.h"
#include "Text.h"	
#include "string.h"	
#include "Picture.h"

//////////////////////////////////////////////////////////////////////////////////	 
/******************************************************************************/	 
//ͼƬ��ʾ ��������	

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
/******************************************************************************/		 
 					     
void Show_Picture(void);//��ʾһ��ͼƬ
void Show_Srceen(void(*fun)(u16* lcd_point,u32 k));
void myStyle_Show_Picture(void);

#endif
