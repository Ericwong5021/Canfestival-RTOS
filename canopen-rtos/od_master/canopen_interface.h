#ifndef __INTERFACE__
#define __INTERFACE__

void CANSendSyc(void);
void CanOpen_Start();
void CanOpenDeviceBootUp(unsigned char NodeId);
void CanOpenChangeNMT(char NodeID);
void CanOpen_Stop();
void Change_PDO_Time(long time);
unsigned char CanOpen_SDO_Read(char id, short index, char sub, char type);
unsigned char Canopen_SDO_Write(char id, short index, char sub, char type, long data);


#endif /* __INTERFACE__ */
