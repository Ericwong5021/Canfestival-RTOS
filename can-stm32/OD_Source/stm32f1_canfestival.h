

#ifndef __STM32F1_CANFESTIVAL_H__
#define __STM32F1_CANFESTIVAL_H__
 
#include "canfestival.h" 
#include "PDO_Master.h" 
#include "mytimer.h" 	
#include "timer.h"



typedef struct 
{
	  u16 cmd;      //0x0000 通用指令 0x0100 电机A轴指令，0x0200 电机B轴指令         
	  u16 RunStatus;            
	  u32 ErrCode;
	  u16 RunMode;
	  u16 Meteri_Null;
	  u16 StepLen;               //调幅丝杆导程
	  u16 AxisEn;           //按位控制轴使能，bit0表示0轴，bit1表示1轴 
	  u16 DigtalOut;
	  u16 DigtalIn;
	  u16 OutMode;
	  u16 BackDefVal; 
	  u32 TimeOut;            //求物料超时报警
	UNS8 ReceiveData[20];
	UNS8 TransmitData[20];
}MASTER_PARA;

extern MASTER_PARA Master;


#define  OD_Soft_Index  0x4000

#define Master_cmd              Master.cmd            
#define Master_RunStatus        Master.RunStatus      
#define Master_ErrCode          Master.ErrCode     
#define Master_RunMode          Master.RunMode
#define Master_StepLen          Master.StepLen        
#define Master_AxisEn           Master.AxisEn         
#define Master_DigtalOut        Master.DigtalOut      
#define Master_DigtalIn         Master.DigtalIn       
#define Master_OutMode          Master.OutMode 
#define Master_Meteri_Null      Master.Meteri_Null
#define Master_BackDefVal      Master.BackDefVal
#define Master_TimeOut      Master.TimeOut

#define Master_ReceiveData_0     Master.ReceiveData[0]
#define Master_ReceiveData_1      Master.ReceiveData[1]
#define Master_ReceiveData_2      Master.ReceiveData[2]
#define Master_ReceiveData_3      Master.ReceiveData[3]
#define Master_ReceiveData_4      Master.ReceiveData[4]
#define Master_ReceiveData_5      Master.ReceiveData[5]
#define Master_ReceiveData_6      Master.ReceiveData[6]
#define Master_ReceiveData_7      Master.ReceiveData[7]
#define Master_ReceiveData_8      Master.ReceiveData[8]
#define Master_ReceiveData_9      Master.ReceiveData[9]
#define Master_ReceiveData_10      Master.ReceiveData[10]
#define Master_ReceiveData_11      Master.ReceiveData[11]
#define Master_ReceiveData_12      Master.ReceiveData[12]
#define Master_ReceiveData_13      Master.ReceiveData[13]
#define Master_ReceiveData_14      Master.ReceiveData[14]
#define Master_ReceiveData_15      Master.ReceiveData[15]
#define Master_ReceiveData_16      Master.ReceiveData[16]
#define Master_ReceiveData_17      Master.ReceiveData[17]
#define Master_ReceiveData_18      Master.ReceiveData[18]
#define Master_ReceiveData_19      Master.ReceiveData[19]

#define Master_TransmitData_0     Master.TransmitData[0]      
#define Master_TransmitData_1    	Master.TransmitData[1] 
#define Master_TransmitData_2    	Master.TransmitData[2]  
#define Master_TransmitData_3    	Master.TransmitData[3] 
#define Master_TransmitData_4    	Master.TransmitData[4] 
#define Master_TransmitData_5    	Master.TransmitData[5] 
#define Master_TransmitData_6    	Master.TransmitData[6] 
#define Master_TransmitData_7    	Master.TransmitData[7] 
#define Master_TransmitData_8    	Master.TransmitData[8] 
#define Master_TransmitData_9    	Master.TransmitData[9] 
#define Master_TransmitData_10   	Master.TransmitData[10]
#define Master_TransmitData_11   	Master.TransmitData[11]
#define Master_TransmitData_12   	Master.TransmitData[12]
#define Master_TransmitData_13   	Master.TransmitData[13]
#define Master_TransmitData_14   	Master.TransmitData[14]
#define Master_TransmitData_15   	Master.TransmitData[15]
#define Master_TransmitData_16   	Master.TransmitData[16]
#define Master_TransmitData_17   	Master.TransmitData[17]
#define Master_TransmitData_18   	Master.TransmitData[18]
#define Master_TransmitData_19   	Master.TransmitData[19]





extern Message my_message;
 
#include "timer.h" 
extern void initTimer(void);
extern UNS8 canSend(CAN_PORT notused, Message *m);
extern UNS8 canChangeBaudRate(CAN_PORT port, char* baud);
extern void setTimer(UNS32 value);
extern UNS32 getElapsedTime(void);

void OD_Save_TO_EE(UNS16 wIndex);

void Sys_ParaInit(void);


#endif

