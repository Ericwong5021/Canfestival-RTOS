

#include "stm32f1_canfestival.h"
#include "sys.h"
#include "mycan.h"
//#include "motion.h"
/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

MASTER_PARA Master;


unsigned int TimeCNT=0;             //时间计数
unsigned int NextTime=0;            //下一次触发时间计数
unsigned int TIMER_MAX_COUNT=70000; //最大的时间计数

//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
static void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}

void initTimer(void)
{
    TIM4_Int_Init(1000-1, 72-1);     //1Mhz的计数频率，计数到1000为1ms
}

//另外还要开一个1毫秒的定时器，每隔1ms调用一次这个函数
void TimerForCan(void)
{
    TimeCNT++;
    
    if (TimeCNT >= TIMER_MAX_COUNT)
    {
        TimeCNT=0;
    }
    if (TimeCNT == NextTime)
    {
        TimeDispatch();     //定时时间到，执行时间相关的分发任务
    }
}

//定时器5每隔1ms中断1次
void TIM4_IRQHandler(void)
{	 
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
    {		 
			   TimerForCan();     
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
                                   //检查时间相关任务
	} 		    
}
 
UNS8 canSend(CAN_PORT notused, Message *m)
{ 
    return Can_Send_Msg(m);   //发送CAN消息
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
 


