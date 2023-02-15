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

extern unsigned char Can_Send_Msg(Message *msg);



extern Message my_message;

#include "timer.h"
extern UNS8 canSend(CAN_PORT notused, Message *m);
extern void setTimer(UNS32 value);
extern UNS32 getElapsedTime(void);


void SDOCallback_t_Index1800_Subindex0(CO_Data *d, UNS8 nodeId);
void SDO_Write_Test(); // sdo 发送测试程序

#endif
