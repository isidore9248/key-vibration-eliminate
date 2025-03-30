#include "stm32f10x.h"
#include "sys.h"
#include "nRF24L01_API.h"
GPIO_InitTypeDef GPIO_InitStructure;

#define LED 			PFout(6)
#define NRF_IRQ   PGin(15)  //IRQ������������,��������


uchar rece_buf[32];

void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void SPI_GPIO_Init(void)
{
	NVIC_Configuration();
  /* ��������I��LED��ʹ�õ�GPIO�ܽ�ģʽ*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF, ENABLE); /*ʹ��LED��ʹ�õ�GPIOʱ��*/
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_5;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);  /*GPIO�ڳ�ʼ��*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*GPIO�ڳ�ʼ��*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);  /*GPIO�ڳ�ʼ��*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  /*GPIO�ڳ�ʼ��*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  /*GPIO�ڳ�ʼ��*/
}

int main(void)
{	
	SPI_GPIO_Init();	
	GPIO_SetBits(GPIOF,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOG,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	LED=0;
	while(NRF24L01_Check()); // �ȴ���⵽NRF24L01������Ż�����ִ��
	NRF24L01_RT_Init();	
	rece_buf[1]=0xc9;					//��
	rece_buf[2]=0xcf;					//��
	rece_buf[3]=0xba;					//��
	rece_buf[4]=0xa3;					//��
	rece_buf[5]=0xb1;					//��
	rece_buf[6]=0xa6;					//��
	rece_buf[7]=0xc7;					//Ƕ
	rece_buf[8]=0xb6;					//Ƕ
	rece_buf[9]=0xb5;					//��
	rece_buf[10]=0xe7;				//��
	rece_buf[11]=0xd7;				//��
	rece_buf[12]=0xd3;				//��
	rece_buf[0]=12;						//һ��Ҫ����12���ֽڣ�rece_buf[0]������12������������
	SEND_BUF(rece_buf);
	while(1)
	{
		if(NRF_IRQ==0)	 	// �������ģ����յ�����
		{		
			if(NRF24L01_RxPacket(rece_buf)==0)
			{			   
				if(	rece_buf[1]=='1')		 //��1λ�Ժ����յ����������ݣ�rece_buf[0]������λ������
					LED=~LED;
				if(	rece_buf[1]=='2')	 	 //��1λ�Ժ����յ����������ݣ�rece_buf[0]������λ������
						;
			}
		}
		
	}
}

