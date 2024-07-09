#include "sccb.h"
#include "delay.h"

/*******************************
SCCB����
ʱ����PD6,������PD7
******************************/
//��ʼ��SCCB�ӿ� 
void SCCB_Init(void)
{				
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(OV7670_SCCB_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(OV7670_XCLK_RCC, ENABLE);

	//STM32F4ʱ�����XCLK
	GPIO_InitStructure.GPIO_Pin = OV7670_XCLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz

	GPIO_Init(OV7670_XCLK_GPIO, &GPIO_InitStructure);

	RCC_MCO1Config(RCC_MCO1Source_HSI,STM32_MCO1_DIV);

	//GPIOB12,B13��ʼ������
	GPIO_InitStructure.GPIO_Pin = OV7670_SCCB_Pin;//PB12,13 �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //PB12,13 �������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(OV7670_SCCB_GPIO, &GPIO_InitStructure);//��ʼ��
 
	GPIO_SetBits(OV7670_SCCB_GPIO,OV7670_SCCB_Pin);
	SCCB_SDA_OUT();	   
}			 		 


//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
		SCCB_SDA_OUT();	
    SCCB_SDA=1;     //�����߸ߵ�ƽ	   
		delay_us(2);
    SCCB_SCL=1;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(2);  
    SCCB_SDA=0;
    delay_us(2);	 
    SCCB_SCL=0;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
	SCCB_SDA_OUT();
	SCCB_SDA=0;
	delay_us(2);	 
	SCCB_SCL=1;	
	delay_us(2); 
	SCCB_SDA=1;	
	delay_us(2);
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
	SCCB_SDA_OUT();
	SCCB_SDA=1;
	delay_us(2);
	SCCB_SCL=1;	
	delay_us(2);
	SCCB_SCL=0;	
	delay_us(2);
	SCCB_SDA=0;	
	delay_us(2);
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		delay_us(2);
		SCCB_SCL=1;	
		delay_us(2);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();		//����SDAΪ���� 
	delay_us(2);
	SCCB_SCL=1;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(2);
	if(SCCB_READ_SDA)
	{
		res=1;  //SDA=1����ʧ�ܣ�����1
	}
	else
	{
		res=0;         //SDA=0���ͳɹ�������0
	}		
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		delay_us(2);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(2);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 SCCB_WR_Reg(u8 reg, u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	delay_us(100);	 
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
	delay_us(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}

