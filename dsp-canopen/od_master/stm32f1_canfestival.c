

#include "stm32f1_canfestival.h"
//#include "motion.h"
/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

extern void CAN_SDO_Update(unsigned char* data, unsigned long size ,unsigned long error);
MASTER_PARA Master;


unsigned int TimeCNT=0;             //时间计数
unsigned int NextTime=0;            //下一次触发时间计数
unsigned int TIMER_MAX_COUNT=70000; //最大的时间计数


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

/* SDO报文读取成功回调函数 */
void SDOCallback_t_Index1800_Subindex0(CO_Data* d, UNS8 nodeId)
{
	UNS8 data[8];
	UNS32 size;
	UNS32 abortCode;

	getReadResultNetworkDict(d, nodeId, data, &size, &abortCode);

	CAN_SDO_Update(data, size, abortCode);
	//printf("abortCode = %ld, size = %ld, data = %d\r\n", abortCode, size, data);
}

void SDO_Write_Test()
{
	UNS32 tt[1]={10};
	writeNetworkDictCallBack(&PDO_Master_Data,0x1f,0x2000,0x00,4,int32,tt,SDOCallback_t_Index1800_Subindex0,0);
	//writeNetworkDictCallBack(&PDO_Master_Data,0x01,0x2000,0x00,4,int32,tt,SDOCallback_t_Index1800_Subindex0,0);
}

