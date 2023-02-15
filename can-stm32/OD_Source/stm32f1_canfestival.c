

#include "stm32f1_canfestival.h"
#include "sys.h"
#include "mycan.h"
//#include "motion.h"
/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

MASTER_PARA Master;


unsigned int TimeCNT=0;             //ʱ�����
unsigned int NextTime=0;            //��һ�δ���ʱ�����
unsigned int TIMER_MAX_COUNT=70000; //����ʱ�����

//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
static void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM5�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}

void initTimer(void)
{
    TIM4_Int_Init(1000-1, 72-1);     //1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1ms
}

//���⻹Ҫ��һ��1����Ķ�ʱ����ÿ��1ms����һ���������
void TimerForCan(void)
{
    TimeCNT++;
    
    if (TimeCNT >= TIMER_MAX_COUNT)
    {
        TimeCNT=0;
    }
    if (TimeCNT == NextTime)
    {
        TimeDispatch();     //��ʱʱ�䵽��ִ��ʱ����صķַ�����
    }
}

//��ʱ��5ÿ��1ms�ж�1��
void TIM4_IRQHandler(void)
{	 
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
    {		 
			   TimerForCan();     
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
                                   //���ʱ���������
	} 		    
}
 
UNS8 canSend(CAN_PORT notused, Message *m)
{ 
    return Can_Send_Msg(m);   //����CAN��Ϣ
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
    return 0;
}

void setTimer(UNS32 value)
{ 
    NextTime = (TimeCNT+value)%TIMER_MAX_COUNT;
}

UNS32 getElapsedTime(void)
{
    int ret=0;
    
    ret = TimeCNT> last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
    last_time_set = TimeCNT;
    return ret;
}

Message my_message;

void Sys_ParaInit(void)
{
	int i=0;
	os_time2_run.Inch_flag=0;
	os_time2_run.Move_delay=0x000000;
	os_time2_run.Inch_reset=0;
	os_time2_run.Led_timer=0x0000;
	for(i=0;i<8;i++)my_message.data[i]=i+0x20;
		my_message.len=8;
		my_message.rtr=CAN_RTR_DATA;
	
	Master.TransmitData[0]=0xa1;
	Master.TransmitData[1]=0xa2;
	Master.TransmitData[2]=0xa3;
	Master.TransmitData[3]=0xa4;
	Master.TransmitData[4]=0xa5;
	Master.TransmitData[5]=0xb1;
	Master.TransmitData[6]=0xb2;
	Master.TransmitData[7]=0xb3;
	Master.TransmitData[8]=0xb4;
	Master.TransmitData[9]=0xb5;
	Master.TransmitData[10]=0xc1;
	Master.TransmitData[11]=0xc2;
	Master.TransmitData[12]=0xc3;
	Master.TransmitData[13]=0xc4;
	Master.TransmitData[14]=0xc5;
	Master.TransmitData[15]=0xd1;
	Master.TransmitData[16]=0xd2;
	Master.TransmitData[17]=0xd3;
	Master.TransmitData[18]=0xd4;
	Master.TransmitData[19]=0xd5;
	
	
	
	
	
}

void OD_Save_TO_EE(UNS16 wIndex)
{
	 switch(wIndex)
		 {	
				case 0x1000: break;// EEPROMwrite_U16(EE_Steplen_Addr,FeiDa_Para_StepLen);  
				case 0x1001: break; //EEPROMwrite_U32(EE_Distance_Addr,FeiDa_Para_Distance);
				case 0x1018: break;
				case 0x1200: break;
				case 0x2000: break;
				case 0x2001: break;
				case 0x2002: break;
				case 0x2006: break;
				case 0x2010: break;
				case 0x2011: break;
				case 0x2012: break;
				case 0x2013: break;
				case 0x2020: break;
				case 0x2021: break;
				case 0x2022: break;
				case 0x2023: break;
				case 0x2024: break;
				case 0x2030: break;
				case 0x2031: break;
				case 0x2032: break;
				case 0x2033: break;
				case 0x2034: break;
				case 0x2300: break;
				case 0x2301: break;
				case 0x4000: break;
				default:
				break;
	}
}
 


