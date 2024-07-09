#include "sys.h"
#include "uart5.h"

//����5 PC12 PD2 ,APB1ʱ��
void uart5_init(u32 bound) {
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);    //ʹ��UART5��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);    //ʹ��UART5��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    //ʹ��UART5��GPIOCʱ��

	//����5��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD2����ΪUSART5
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOC12����ΪUSART5
	
	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD2

	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC12

	//Usart5 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ

	USART_Init(UART5, &USART_InitStructure); //��ʼ������5
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���5
}

void u5_printf(char *fmt, ...) {
    int i;
    int len;
    char buffer[50]; //�㹻���ɲſ��ԣ����Ը���
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    len = strlen(buffer);

    for (i = 0; i < len; i++) {
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET); //�ȴ��������
        USART_SendData(UART5, buffer[i]); //����һ���ַ�
    }
}

void UART5_IRQHandler(void)                    //����5�жϷ������
{
    u8 Res;
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(UART5);    //��ȡ���յ�������

        u5_printf("�յ���һ���ַ�: %c\r\n", Res);
    }

}
