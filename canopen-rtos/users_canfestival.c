/*
 * @Author: Ericwong
 * @Date: 2023-02-15 18:33:06
 * @Last Modified by: Ericwong
 * @Last Modified time: 2023-02-15 18:33:42
 */
#include "users_canfestival.h"
/************************** Modul variables **********************************/
// TODO canopen移植: 改为主站节点的字典名称,所有主站的接口函数都会引用这个指针
CO_Data *master_node = &PDO_Master_Data;

// TODO canopen移植函数: canopen报文解析函数,放在CAN接收中断里
void Can_Recv_Msg(Message *msg)
{
    canDispatch(master_node, msg);
}

// TODO canopen移植: canopen协议的CAN发送函数,这里需要调用CAN外设的发送函数
unsigned char Can_Send_Msg(Message *msg)
{
    char rtn = 0;

    return rtn;
    // 返回0, 发送成功
    // 其他 , 发送失败
}

UNS8 canSend(CAN_PORT notused, Message *m)
{
    return Can_Send_Msg(m);
}


unsigned int TimeCNT = 0;             // 时间计数
unsigned int NextTime = 0;            // 下一次触发时间计数
unsigned int TIMER_MAX_COUNT = 70000; // 最大的时间计数

// TODO canopen移植: 在目标芯片上开一个1毫秒的定时器，每隔1ms调用一次这个函数
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
        TimeDispatch(); // 定时时间到，执行时间相关的分发任务
    }
}

void setTimer(UNS32 value)
{
    NextTime = (TimeCNT + value) % TIMER_MAX_COUNT;
}

UNS32 getElapsedTime(void)
{
    // Store the last timer value to calculate the elapsed time
    static TIMEVAL last_time_set = TIMEVAL_MAX;
    int ret = 0;

    ret = TimeCNT > last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
    last_time_set = TimeCNT;
    return ret;
}

// TODO canopen移植: 初始化CANOpen主站,同时复位总线上所有其他从站设备
void canopen_start()
{
    setNodeId(master_node, 0x00);
    setState(master_node, Initialisation);
    setState(master_node, Pre_operational);
    setState(master_node, Operational);
}

// TODO canopen移植: PDO同步帧调用这个函数,根据需要的同步周期调用 (硬件手动发送比协议栈发送稳定)
void CANSendSyc(void)
{
    Message SycMesg;
    SycMesg.cob_id = 0x80;
    SycMesg.len = 0;
    SycMesg.rtr = 0;
    Can_Send_Msg(&SycMesg);
}

// TODO canopen移植: 从站上线会调用这个函数
void CanOpenDeviceBootUp(unsigned char NodeId)
{
    // CAN_NewSlaveBootup(NodeId); // put your operate function here
    CanOpenChangeNMT(NodeId, NMT_Start_Node);
}

// TODO canopen移植: 切换从站状态
void CanOpenChangeNMT(char NodeID, int NodeState)
{
    masterSendNMTstateChange(master_node, NodeID, NodeState); // 发PDO启动指令
}

/* SDO报文读写回调函数 */
void SDOCallback_t_Index1800_Subindex0(CO_Data *d, UNS8 nodeId)
{
    UNS8 data[8];
    UNS32 size;
    UNS32 abortCode;

    getReadResultNetworkDict(d, nodeId, data, &size, &abortCode);

    // printf("abortCode = %ld, size = %ld, data = %d\r\n", abortCode, size, data);
}

// TODO canopen移植: canopen字典读取函数,读取的数据由回调函数返回
unsigned char CanOpen_SDO_Read(char id, short index, char sub, char type)
{
    return readNetworkDictCallback(master_node, id, index, sub, type, SDOCallback_t_Index1800_Subindex0, 0);
}

// TODO canopen移植: canopen字典写入函数,写入的结果由回调函数返回
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
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_bool, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x02:
        size = 1;
        data_s8 = (char)data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_s8, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x03:
        size = 2;
        data_s16 = (short)data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_s16, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x04:
        size = 4;
        data_s32 = data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_s32, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x05:
        size = 1;
        data_u8 = (unsigned char)data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_u8, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x06:
        size = 2;
        data_u16 = (unsigned short)data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_u16, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    case 0x07:
        size = 4;
        data_u32 = (unsigned long)data;
        err = writeNetworkDictCallBack(master_node, id, index, sub, size, type, &data_u32, SDOCallback_t_Index1800_Subindex0, 0);
        break;
    default:
        break;
    }
    return err;
}

void SDO_Write_Test()
{
    UNS32 tt[1] = {10};
    writeNetworkDictCallBack(master_node, 0x1f, 0x2000, 0x00, 4, int32, tt, SDOCallback_t_Index1800_Subindex0, 0);
    // writeNetworkDictCallBack(master_node,0x01,0x2000,0x00,4,int32,tt,SDOCallback_t_Index1800_Subindex0,0);
}
