#include "can.h"
#include "stm32f1_canfestival.h"
#include "canopen_interface.h"

extern unsigned long PDO_Master_obj1006;

typedef struct
{
    char id;
    short index;
    char sub;
    char type;
    long data;
} SDO_Message;


void CANSendSyc(void)
{
    Message SycMesg;
    SycMesg.cob_id = 0x80;
    SycMesg.len = 0;
    SycMesg.rtr = 0;
    Can_Send_Msg(&SycMesg);
}


void CanOpen_Start()
{
    setNodeId(&PDO_Master_Data, 0x00);
    setState(&PDO_Master_Data, Initialisation);
    setState(&PDO_Master_Data, Pre_operational);
    setState(&PDO_Master_Data, Operational);
}

void CanOpenDeviceBootUp(unsigned char NodeId)
{
    CAN_NewSlaveBootup(NodeId); //TODO(eric): call this function when node device booting up
}
void CanOpenChangeNMT(char NodeID)
{
    masterSendNMTstateChange(&PDO_Master_Data, NodeID, NMT_Start_Node); // 发PDO启动指令
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
