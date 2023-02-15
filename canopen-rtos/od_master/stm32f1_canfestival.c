/*
 * @Author: Ericwong 
 * @Date: 2023-02-15 18:33:06 
 * @Last Modified by: Ericwong
 * @Last Modified time: 2023-02-15 18:33:42
 */
#include "stm32f1_canfestival.h"
/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

extern void CAN_SDO_Update(unsigned char *data, unsigned long size, unsigned long error);

unsigned int TimeCNT = 0;			  // ʱ�����
unsigned int NextTime = 0;			  // ��һ�δ���ʱ�����
unsigned int TIMER_MAX_COUNT = 70000; // ����ʱ�����

// TODO canopen��ֲ����: ����CAN����ķ��ͺ���,����һ������,��׼֡��ʽ����
unsigned char Can_Send_Msg(Message *msg)
{
	char rtn = 0;

	return rtn;
	// ����0, ���ͳɹ�
	// ���� , ����ʧ��
}

// TODO canopen��ֲ: ��Ŀ��оƬ�Ͽ�һ��1����Ķ�ʱ����ÿ��1ms����һ���������
void TimerForCan(void)
{
	static int TimeCNT = 0;
	TimeCNT++;
	if (TimeCNT >= TIMER_MAX_COUNT)
	{
		TimeCNT = 0;
	}
	if (TimeCNT == NextTime)
	{
		TimeDispatch(); // ��ʱʱ�䵽��ִ��ʱ����صķַ�����
	}
}

// TODO canopen��ֲ: PDOͬ��֡�����������,������Ҫ��ͬ�����ڵ��� (Ӳ���ֶ����ͱ�Э��ջ�����ȶ�)
void CANSendSyc(void)
{
	Message SycMesg;
	SycMesg.cob_id = 0x80;
	SycMesg.len = 0;
	SycMesg.rtr = 0;
	Can_Send_Msg(&SycMesg);
}
// TODO canopen��ֲ: ��Ŀ��оƬ�Ͽ�һ��1����Ķ�ʱ����ÿ��1ms����һ���������
void TimerForCan(void)
{
	TimeCNT++;

	if (TimeCNT >= TIMER_MAX_COUNT)
	{
		TimeCNT = 0;
	}
	if (TimeCNT == NextTime)
	{
		TimeDispatch(); // ��ʱʱ�䵽��ִ��ʱ����صķַ�����
	}
}

UNS8 canSend(CAN_PORT notused, Message *m)
{
	return Can_Send_Msg(m);
}

void setTimer(UNS32 value)
{
	NextTime = (TimeCNT + value) % TIMER_MAX_COUNT;
}

UNS32 getElapsedTime(void)
{
	int ret = 0;

	ret = TimeCNT > last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
	last_time_set = TimeCNT;
	return ret;
}

Message my_message;

/* SDO���Ķ�ȡ�ɹ��ص����� */
void SDOCallback_t_Index1800_Subindex0(CO_Data *d, UNS8 nodeId)
{
	UNS8 data[8];
	UNS32 size;
	UNS32 abortCode;

	getReadResultNetworkDict(d, nodeId, data, &size, &abortCode);

	CAN_SDO_Update(data, size, abortCode);
	// printf("abortCode = %ld, size = %ld, data = %d\r\n", abortCode, size, data);
}

void SDO_Write_Test()
{
	UNS32 tt[1] = {10};
	writeNetworkDictCallBack(&PDO_Master_Data, 0x1f, 0x2000, 0x00, 4, int32, tt, SDOCallback_t_Index1800_Subindex0, 0);
	// writeNetworkDictCallBack(&PDO_Master_Data,0x01,0x2000,0x00,4,int32,tt,SDOCallback_t_Index1800_Subindex0,0);
}

extern unsigned long PDO_Master_obj1006;


// TODO canopen��ֲ: ��ʼ��CANOpen��վ,ͬʱ��λ����������������վ�豸
void CanOpen_Start()
{
    setNodeId(&PDO_Master_Data, 0x00);
    setState(&PDO_Master_Data, Initialisation);
    setState(&PDO_Master_Data, Pre_operational);
    setState(&PDO_Master_Data, Operational);
}

void CanOpenDeviceBootUp(unsigned char NodeId)
{
    CAN_NewSlaveBootup(NodeId); // TODO(eric): call this function when node device booting up
}
void CanOpenChangeNMT(char NodeID)
{
    masterSendNMTstateChange(&PDO_Master_Data, NodeID, NMT_Start_Node); // ��PDO����ָ��
}
void CanOpen_Stop()
{
    setNodeId(&PDO_Master_Data, 0x00);
    setState(&PDO_Master_Data, Stopped);
}
void Change_PDO_Time(long time)
{
    PDO_Master_obj1006 = time;
}
unsigned char CanOpen_SDO_Read(char id, short index, char sub, char type)
{
    return readNetworkDictCallback(&PDO_Master_Data, id, index, sub, type, SDOCallback_t_Index1800_Subindex0, 0);
}
unsigned char Canopen_SDO_Write(char id, short index, char sub, char type, long data)
{
    char size;
    unsigned char err;
    char data_bool;
    char data_s8;
    short data_s16;
    long data_s32;
    unsigned char data_u8;
    unsigned short data_u16;
    unsigned long data_u32;
    switch (type)
    {
    case 0x01:
        size = 1;
        data_bool = (char)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_bool, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x02:
        size = 1;
        data_s8 = (char)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_s8, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x03:
        size = 2;
        data_s16 = (short)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_s16, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x04:
        size = 4;
        data_s32 = data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_s32, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x05:
        size = 1;
        data_u8 = (unsigned char)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_u8, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x06:
        size = 2;
        data_u16 = (unsigned short)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_u16, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x07:
        size = 4;
        data_u32 = (unsigned long)data;
        err = writeNetworkDictCallBack(&PDO_Master_Data, id, index, sub, size, type, &data_u32, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    default:
        break;
    }
    return err;
}
