/************************************************************************************

*************************************************************************************/
#include "bsp_hx711_2.h"
#include "delay.h"

u32 HX711_Buffer2;
u32 Weight_Maopi2;
float Weight_Shiwu2;
u8 Flag_Error2 = 0;
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue2 106.5
void Init_2_HX711pin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PF�˿�ʱ��

    //HX711_SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOB

    //HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	//HX711_SCK
    GPIO_SetBits(GPIOA,GPIO_Pin_5);					//��ʼ������Ϊ0
}
//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_2_Read(void)	//����128
{
    unsigned long count;
    unsigned char i;
    HX711_2_DOUT=1;
    delay_us(5);
    HX711_2_SCK=0;
    count=0;
    while(HX711_2_DOUT);
    for(i=0; i<24; i++)
        {
            HX711_2_SCK=1;
            count=count<<1;
            delay_us(5);
            HX711_2_SCK=0;
            if(HX711_2_DOUT)
                count++;
            delay_us(5);
        }
    HX711_2_SCK=1;
    count=count^0x800000;//��25�������½�����ʱ��ת������
    delay_us(5);
    HX711_2_SCK=0;
    return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi2(void)
{
    Weight_Maopi2 = HX711_2_Read();
}

//****************************************************
//����
//****************************************************
void Get_Weight2(void)
{
    HX711_Buffer2 = HX711_2_Read();
    if(HX711_Buffer2 > Weight_Maopi2)
        {
            Weight_Shiwu2 = HX711_Buffer2 - Weight_Maopi2;		//��ȡʵ���AD������ֵ��
            Weight_Shiwu2 = ((float)Weight_Shiwu2/GapValue2); 	//����ʵ���ʵ������
            //��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
            //�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
            //������Գ���������ƫСʱ����С����ֵ��
        }
}
