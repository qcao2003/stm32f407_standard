#include "stm32f4xx.h"                
#include "NRF24L01.h"
#include "LCD.h"
#include "delay.h"
#include "SPI.h"
 
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //�����ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //�����ַ  ʹ��P0ͨ���Ļ��������ַ�ͽ��յ�ַ��һ����
 
//nRF24L01��SPI��ʼ��
void NRF24L01_SPI_Init(void)
{
	SPI_Cmd(SPI1,DISABLE); //ʧ��SPI����  ������SPI����ʱ��������ʧ����ʹ��
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CRCPolynomial=7;	//CRCֵ����Ķ���ʽ
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI
}
//��ʼ��nRF24L01��IO��
void NRF24L01_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOG��GPIOBʱ��
	//֮����ʹ��GPIOBʱ������ΪSPI1��nRF24L01��W25Q128�����õģ�������ʹ��nRF24L01��ʱ�򣬽�W25Q128��Ƭѡ�ź����øߣ���ֹӰ��ͨѶ
	
	//PG6 PG7
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//PG8
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//PB14
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_14); //PB14Ƭѡ�ź�������ߵ�ƽ����ֹ����
	
	SPI1_Init();      		//��ʼ��SPI1
	NRF24L01_SPI_Init();	//���nRF24L01�޸�SPI������
	
	NRF24L01_CE=0;			//ʹ��nRF24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��
}
//���nRF24L01�Ƿ����
//����ֵ��0���ɹ���1��ʧ��
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	u8 i=0;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //SPI�ٶ�Ϊ10.5Mhz(nRF24L01�����SPIʱ��Ϊ10Mhz)
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);  //д��5���ֽڵĵ�ַ��buf��  �ú�����һ�������ǼĴ�����λ�ã�ͷ�ļ��ж���ĵ�ַ+����λ��Ҫд��ĵ�ַ
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  ��һ�������ǼĴ�����λ��
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0xA5)
		{
			break;  //����յ������ݲ���0xA5����ֹͣ��ȡ����
		}
	}
	if(i!=5)  //���������ݸ����ʹ洢���ݵ�buf����С����
	{
		return 1; //��⵽nRF24L01����
	}
	return 0;
}
//SPIд�Ĵ���
//Register��ָ���ļĴ�����ַ
//Value��д���ֵ
u8 NRF24L01_Write_Reg(u8 Register,u8 Value)
{
	u8 Status;
	NRF24L01_CSN=0;  //ʹ��nRF24L01��Ƭѡ�ź���
	Status=SPI1_ReadWriteByte(Register); //���ͼĴ����ĵ�ַ,SPI1_ReadWriteByte�ķ���ֵ��SPI1�������ֽڣ����﷢�͵��ǼĴ����ĵ�ַ�����Զ�ȡ���ľ��ǼĴ����ĵ�ַ
	SPI1_ReadWriteByte(Value); 			 //д��Ĵ�����ֵ
	NRF24L01_CSN=1;  //ȡ��nRF24L01��Ƭѡ�ź�
	return(Status);  //���ض����ļĴ�����ַ
}
//��ȡSPI�Ĵ���ֵ
//Register��Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 Register)
{
	u8 Register_Value; //����һ��������¼��ȡ����ֵ�����ڷ���
	NRF24L01_CSN=0;  //ʹ��nRF24L01��Ƭѡ�ź���
	SPI1_ReadWriteByte(Register); //��Ҫ���ļĴ�����д��
	Register_Value=SPI1_ReadWriteByte(0XFF); //��ȡ�Ĵ���������  
	//0XFF��ͷ�ļ����Ѿ�����
	//#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���
	
	NRF24L01_CSN=1;  //ʧ��nRF24L01��Ƭѡ�ź���
	return(Register_Value);
}
//��ָ��λ���϶���ָ�����ȵ�����
//Register���Ĵ�����λ��
//*pBuffer������ָ��
//Length�����ݳ���
//����ֵ���˴ζ�����״ֵ̬
u8 NRF24L01_Read_Buf(u8 Register,u8 *pBuffer,u8 Length)
{
	u8 Status,i;
	NRF24L01_CSN=0;
	Status=SPI1_ReadWriteByte(Register);  //���ͼĴ����ĵ�ַ���õ���״ֵ̬
	for(i=0;i<Length;i++)
	{
		pBuffer[i]=SPI1_ReadWriteByte(0XFF); //��������
	}
	NRF24L01_CSN=1;
	return Status;  //���ض�����״ֵ̬
}
//��ָ��λ����дָ�����ȵ�����
//Register���Ĵ�����λ��
//*pBuffer������ָ��
//Length�����ݳ���
//����ֵ���˴ζ�����״̬�Ĵ�����ֵ
u8 NRF24L01_Write_Buf(u8 Register,u8 *pBuffer,u8 Length)
{
	u8 Status,i;
	NRF24L01_CSN=0;
	Status=SPI1_ReadWriteByte(Register);  //���ͼĴ����ĵ�ַ���õ���״ֵ̬
	for(i=0;i<Length;i++)
	{
		SPI1_ReadWriteByte(*pBuffer++); //д������  ����++  ÿд��һ������ָ��ָ��ĵ�ַ����һλ
	}
	NRF24L01_CSN=1;
	return Status;
}
//����NRF24L01����һ������
//TX_Buffer�����������ݵ��׵�ַ
//����ֵ���������״��
u8 NRF24L01_TxPacket(u8 *TX_Buffer)
{
	u8 Status;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��
	NRF24L01_CE=0; //��CE�õ�����Ϊֻ�д��ڵ���ģʽ���ߴ���ģʽ�ſ���д�Ĵ���������CE�õͱ�ʾ�������ģʽ1
	NRF24L01_Write_Buf(WR_TX_PLOAD,TX_Buffer,TX_PLOAD_WIDTH); //д���ݵ�����Ļ������У�WR_TX_PLOAD��ͷ�ļ������дTX��Ч����
	NRF24L01_CE=1;  //��������  ����ǿ��ShockBurstTMģʽ�£���CEΪ�ߣ�����10us����ʹ�ܷ��͹���
	//Ҳ����˵��ֻ�ǽ�CE��1���������������뷢�͹��̣���Ҳ��Ϊʲô������Ҫ�����ж��Ƿ������
	
	while(NRF24L01_IRQ!=0); //�ȴ��������   IRQ����������±�Ϊ�͵�ƽ������֮һ��Tx FIFO�����겢���յ�Ӧ��ACK  ����NRF24L01_IRQ=0ʱ�����������
	
	Status=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ��STATUS��ͷ�ļ����Ѿ�����
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,Status); //���TX_DS��MAX_RT�жϱ�־
	if(Status&MAX_TX) //�ﵽ����ط����� MAX_TX��0x10 ���Ե�״̬�Ĵ�����λ4������1��Ҳ���Ǵﵽ������
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xFF); //���FIFO�Ĵ���
		return FLUSH_TX;
	}
	if(Status&TX_OK)  //������� TX_OK��OX20����״̬�Ĵ����ĵ�6λ��1����ʾ��������ж�
	{
		return TX_OK;
	}
	return 0xff; //����ԭ�򣬷���ʧ��
}
//����NRF24L01����һ������
//TX_Buffer�����������ݵ��׵�ַ
//����ֵ��0��������ɣ�����������
u8 NRF24L01_RxPacket(u8 *TX_Buffer)
{
	u8 Status;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz�� 
	Status=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,Status); // ���TX_DS��MAX_RT�жϱ�־
	if(Status&RX_OK) //���ܵ�����  RX_OK��0x40��״̬�Ĵ�����6λ��1��Ҳ���ǽ��������ж�
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,TX_Buffer,RX_PLOAD_WIDTH);  //��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xFF); //���RX FIFO�Ĵ���
		return 0;
	}
	return 1; //û�յ��κ�����
}
//��ʼ��NRF24L01��Rxģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void NRF24L01_Rx_Mode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_READ_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //дRX�ڵ��ַ
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //ʹ��ͨ��0���Զ�Ӧ��,  ֻ��ͨ��0�����Զ�Ӧ����
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0���յ�ַ
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);  //����RFͨ��Ƶ��   
	
	//��ƵRF���㹫ʽ��2400+RF_CH(Mhz) ����40��Ҳ����2440Mhz
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH); //ѡ��ͨ��0��Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	
	NRF24L01_CE=1;  //CEΪ�ߣ��������ģʽ
}
//��ʼ��NRF24L01��Txģʽ
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,���������ͣ�������ģʽ
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH); //дTX�ڵ��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����RX�ڵ��ַ����Ҫ��Ϊ��ʹ��ACK
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ 
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE=1; //CEΪ�ߣ�10us����������
}


