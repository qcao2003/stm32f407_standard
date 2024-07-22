#include "stm32f4xx.h"                
#include "NRF24L01.h"
#include "LCD.h"
#include "delay.h"
#include "SPI.h"
 
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //发射地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //发射地址  使用P0通道的话，发射地址和接收地址是一样的
 
//nRF24L01的SPI初始化
void NRF24L01_SPI_Init(void)
{
	SPI_Cmd(SPI1,DISABLE); //失能SPI外设  在设置SPI外设时，必须先失能再使能
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CRCPolynomial=7;	//CRC值计算的多项式
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE); //使能SPI
}
//初始化nRF24L01的IO口
void NRF24L01_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOG和GPIOB时钟
	//之所以使能GPIOB时钟是因为SPI1是nRF24L01和W25Q128所共用的，所以在使用nRF24L01的时候，将W25Q128的片选信号线置高，防止影响通讯
	
	//PG6 PG7
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //普通输出模式
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//PB14
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_14); //PB14片选信号线输出高电平，防止干扰
	
	SPI1_Init();      		//初始化SPI1
	NRF24L01_SPI_Init();	//针对nRF24L01修改SPI的设置
	
	NRF24L01_CE=0;			//使能nRF24L01
	NRF24L01_CSN=1;			//SPI片选取消
}
//检测nRF24L01是否存在
//返回值：0，成功；1，失败
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	u8 i=0;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //SPI速度为10.5Mhz(nRF24L01的最大SPI时钟为10Mhz)
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);  //写入5个字节的地址到buf里  该函数第一个参数是寄存器的位置，头文件中定义的地址+低五位所要写入的地址
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  第一个参数是寄存器的位置
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0xA5)
		{
			break;  //如果收到的数据不是0xA5，就停止读取数据
		}
	}
	if(i!=5)  //读到的数据个数和存储数据的buf区大小不等
	{
		return 1; //检测到nRF24L01错误
	}
	return 0;
}
//SPI写寄存器
//Register：指定的寄存器地址
//Value：写入的值
u8 NRF24L01_Write_Reg(u8 Register,u8 Value)
{
	u8 Status;
	NRF24L01_CSN=0;  //使能nRF24L01的片选信号线
	Status=SPI1_ReadWriteByte(Register); //发送寄存器的地址,SPI1_ReadWriteByte的返回值是SPI1读到的字节，这里发送的是寄存器的地址，所以读取到的就是寄存器的地址
	SPI1_ReadWriteByte(Value); 			 //写入寄存器的值
	NRF24L01_CSN=1;  //取消nRF24L01的片选信号
	return(Status);  //返回读到的寄存器地址
}
//读取SPI寄存器值
//Register：要读的寄存器
u8 NRF24L01_Read_Reg(u8 Register)
{
	u8 Register_Value; //定义一个变量记录读取到的值，用于返回
	NRF24L01_CSN=0;  //使能nRF24L01的片选信号线
	SPI1_ReadWriteByte(Register); //把要读的寄存器号写入
	Register_Value=SPI1_ReadWriteByte(0XFF); //读取寄存器的内容  
	//0XFF在头文件中已经定义
	//#define NOP             0xFF  //空操作,可以用来读状态寄存器
	
	NRF24L01_CSN=1;  //失能nRF24L01的片选信号线
	return(Register_Value);
}
//在指定位置上读出指定长度的数据
//Register：寄存器的位置
//*pBuffer：数据指针
//Length：数据长度
//返回值：此次读到的状态值
u8 NRF24L01_Read_Buf(u8 Register,u8 *pBuffer,u8 Length)
{
	u8 Status,i;
	NRF24L01_CSN=0;
	Status=SPI1_ReadWriteByte(Register);  //发送寄存器的地址，得到其状态值
	for(i=0;i<Length;i++)
	{
		pBuffer[i]=SPI1_ReadWriteByte(0XFF); //读出数据
	}
	NRF24L01_CSN=1;
	return Status;  //返回读到的状态值
}
//在指定位置上写指定长度的数据
//Register：寄存器的位置
//*pBuffer：数据指针
//Length：数据长度
//返回值：此次读到的状态寄存器的值
u8 NRF24L01_Write_Buf(u8 Register,u8 *pBuffer,u8 Length)
{
	u8 Status,i;
	NRF24L01_CSN=0;
	Status=SPI1_ReadWriteByte(Register);  //发送寄存器的地址，得到其状态值
	for(i=0;i<Length;i++)
	{
		SPI1_ReadWriteByte(*pBuffer++); //写入数据  后置++  每写入一次数据指针指向的地址后移一位
	}
	NRF24L01_CSN=1;
	return Status;
}
//启动NRF24L01发送一次数据
//TX_Buffer：待发送数据的首地址
//返回值：发送完成状况
u8 NRF24L01_TxPacket(u8 *TX_Buffer)
{
	u8 Status;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）
	NRF24L01_CE=0; //把CE置低是因为只有处于掉电模式或者待机模式才可以写寄存器，这里CE置低表示进入待机模式1
	NRF24L01_Write_Buf(WR_TX_PLOAD,TX_Buffer,TX_PLOAD_WIDTH); //写数据到定义的缓存区中，WR_TX_PLOAD是头文件定义的写TX有效数据
	NRF24L01_CE=1;  //启动发送  在增强型ShockBurstTM模式下，置CE为高，至少10us，将使能发送过程
	//也就是说，只是将CE置1，并不会立即进入发送过程，这也是为什么紧接着要立刻判断是否发送完成
	
	while(NRF24L01_IRQ!=0); //等待发送完成   IRQ在三种情况下变为低电平，其中之一是Tx FIFO发送完并且收到应答ACK  所以NRF24L01_IRQ=0时，代表发送完成
	
	Status=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值，STATUS在头文件中已经定义
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,Status); //清除TX_DS或MAX_RT中断标志
	if(Status&MAX_TX) //达到最大重发次数 MAX_TX是0x10 所以的状态寄存器的位4必须是1，也就是达到最多次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xFF); //清除FIFO寄存器
		return FLUSH_TX;
	}
	if(Status&TX_OK)  //发送完成 TX_OK是OX20，将状态寄存器的第6位置1，表示发送完成中断
	{
		return TX_OK;
	}
	return 0xff; //其他原因，发送失败
}
//启动NRF24L01接收一次数据
//TX_Buffer：待接收数据的首地址
//返回值：0，接收完成；其他，错误
u8 NRF24L01_RxPacket(u8 *TX_Buffer)
{
	u8 Status;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz） 
	Status=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,Status); // 清除TX_DS或MAX_RT中断标志
	if(Status&RX_OK) //接受到数据  RX_OK是0x40，状态寄存器第6位是1，也就是接收数据中断
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,TX_Buffer,RX_PLOAD_WIDTH);  //读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xFF); //清除RX FIFO寄存器
		return 0;
	}
	return 1; //没收到任何数据
}
//初始化NRF24L01到Rx模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_Rx_Mode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_READ_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //写RX节点地址
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //使能通道0的自动应答,  只有通道0才有自动应答功能
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0接收地址
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);  //设置RF通信频率   
	
	//射频RF计算公式：2400+RF_CH(Mhz) 给的40，也就是2440Mhz
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH); //选择通道0有效数据宽度
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	
	NRF24L01_CE=1;  //CE为高，进入接收模式
}
//初始化NRF24L01到Tx模式
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送，到发送模式
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH); //写TX节点地址
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置RX节点地址，主要是为了使能ACK
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //使能通道0的自动应答
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址 
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE=1; //CE为高，10us后启动发送
}


