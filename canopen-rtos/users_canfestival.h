/*
 * @Author: Ericwong
 * @Date: 2023-02-15 18:33:12
 * @Last Modified by:   Ericwong
 * @Last Modified time: 2023-02-15 18:33:12
 */
#ifndef __STM32F1_CANFESTIVAL_H__
#define __STM32F1_CANFESTIVAL_H__

#include "PDO_Master.h"
#include "canfestival.h"
#include "timer.h" // TODO: is this necessary?

// core functions
void CanOpenStart();
void TimerForCanOpen(void);
void CanOpenRecvMsg(Message *msg);
unsigned char CanOpenSendMsg(Message *msg);
unsigned char CanOpenSdoRead(char id, short index, char sub, char type);
unsigned char CanopenSdoWrite(char id, short index, char sub, char type, long data);

// optional functions
void SdoWriteTest();
void CanOpenPdoSyc(void);
void CanOpenDeviceBootUp(unsigned char NodeId);
void CanOpenChangeNMT(char NodeID, int NodeState);

#endif
