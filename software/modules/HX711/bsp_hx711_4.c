/************************************************************************************

*************************************************************************************/
#include "bsp_hx711_4.h"
#include "delay.h"

u32 HX711_Buffer4;
u32 Weight_Maopi4;
float Weight_Shiwu4;
u8 Flag_Error4 = 0;
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue4 106.5
void Init_4_HX711pin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PF�˿�ʱ��
//#define HX711_4_SCK  PBout(5) // PB5
//#define HX711_4_DOUT PBin(4)  // PB4
    //HX711_SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOB

    //HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//HX711_SCK
    GPIO_SetBits(GPIOB,GPIO_Pin_5);					//��ʼ������Ϊ0
}
//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_4_Read(void)	//����128
{
    unsigned long count;
    unsigned char i;
    HX711_4_DOUT=1;
    delay_us(5);
    HX711_4_SCK=0;
    count=0;
    while(HX711_4_DOUT);
    for(i=0; i<24; i++)
        {
            HX711_4_SCK=1;
            count=count<<1;
            delay_us(5);
            HX711_4_SCK=0;
            if(HX711_4_DOUT)
                count++;
            delay_us(5);
        }
    HX711_4_SCK=1;
    count=count^0x800000;//��25�������½�����ʱ��ת������
    delay_us(5);
    HX711_4_SCK=0;
    return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi4(void)
{
    Weight_Maopi4 = HX711_4_Read();
}

//****************************************************
//����
//****************************************************
void Get_Weight4(void)
{
    HX711_Buffer4 = HX711_4_Read();
    if(HX711_Buffer4 > Weight_Maopi4)
        {
            Weight_Shiwu4 = HX711_Buffer4 - Weight_Maopi4;		//��ȡʵ���AD������ֵ��
            Weight_Shiwu4 = ((float)Weight_Shiwu4/GapValue4); 	//����ʵ���ʵ������
            //��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
            //�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
            //������Գ���������ƫСʱ����С����ֵ��
        }
}
