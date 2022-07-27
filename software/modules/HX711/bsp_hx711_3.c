/************************************************************************************

*************************************************************************************/
#include "bsp_hx711_3.h"
#include "delay.h"

u32 HX711_Buffer3;
u32 Weight_Maopi3;
float Weight_Shiwu3;
u8 Flag_Error3 = 0;
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue3 106.5
void Init_3_HX711pin(void)
{
//	#define HX711_3_SCK  PBout(3) // PB3
//	#define HX711_3_DOUT PAin(15) // PA15
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PF�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PF�˿�ʱ��
	
    //HX711_SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		 // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOB

    //HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	//HX711_SCK
    GPIO_SetBits(GPIOB,GPIO_Pin_3);					//��ʼ������Ϊ0
}
//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_3_Read(void)	//����128
{
    unsigned long count;
    unsigned char i;
    HX711_3_DOUT=1;
    delay_us(5);
    HX711_3_SCK=0;
    count=0;
    while(HX711_3_DOUT);
    for(i=0; i<24; i++)
        {
            HX711_3_SCK=1;
            count=count<<1;
            delay_us(5);
            HX711_3_SCK=0;
            if(HX711_3_DOUT)
                count++;
            delay_us(5);
        }
    HX711_3_SCK=1;
    count=count^0x800000;//��25�������½�����ʱ��ת������
    delay_us(5);
    HX711_3_SCK=0;
    return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi3(void)
{
    Weight_Maopi3 = HX711_3_Read();
}

//****************************************************
//����
//****************************************************
void Get_Weight3(void)
{
    HX711_Buffer3 = HX711_3_Read();
    if(HX711_Buffer3 > Weight_Maopi3)
        {
            Weight_Shiwu3 = HX711_Buffer3 - Weight_Maopi3;		//��ȡʵ���AD������ֵ��
            Weight_Shiwu3 = ((float)Weight_Shiwu3/GapValue3); 	//����ʵ���ʵ������
            //��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
            //�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
            //������Գ���������ƫСʱ����С����ֵ��
        }
}
