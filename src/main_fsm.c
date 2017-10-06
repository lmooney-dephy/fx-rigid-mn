/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-manage' Mid-level computing, and networking
	Copyright (C) 2017 Dephy, Inc. <http://dephy.com/>
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] main_fsm: Contains all the case() code for the main FSM
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-23 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "user-mn.h"
#include <adc.h>
#include <dio.h>
#include <i2c.h>
#include <master_slave_comm.h>
#include <ui.h>
#include "main.h"
#include "main_fsm.h"
#include "flexsea_global_structs.h"
#include "flexsea_board.h"
#include "rgb_led.h"
#include "cmd-Rigid.h"
#include "flexsea_system.h"
#include "flexsea_interface.h"
#include "spi.h"
#include "misc.h"
#include "svm.h"
#include "user-mn.h"

#include "cmd-Bilateral.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

uint8_t newMasterCmdLed = 0, newSlaveCmdLed = 0, newPacketsFlag = 0;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************

//Test code:
void sendReadRigidPacket(void);

//****************************************************************************
// Public Function(s)
//****************************************************************************

//1kHz time slots:
//================

//Case 0: slaveComm
void mainFSM0(void)
{
	//slaveTransmit(PORT_RS485_1);
}

//Case 1: I2C1 - IMU
void mainFSM1(void)
{
	i2c1_fsm();
}

//Case 2: I2C2 - Unused
void mainFSM2(void)
{
	i2c2_fsm();

	#ifdef BILATERAL_MASTER
	static uint8_t cnt = 0;
	cnt++;
	cnt %= 10;
	if(!cnt)
	{
		sendReadRigidPacket();
	}
	#endif	//BILATERAL_MASTER
}

//Case 3:
void mainFSM3(void)
{
	independentWatchdog();

	readInternalTempSensor();
}

//Case 4: User Functions
void mainFSM4(void)
{
	#if(RUNTIME_FSM1 == ENABLED)
	user_fsm_1();
	#endif //RUNTIME_FSM1 == ENABLED
}

//Case 5:
void mainFSM5(void)
{
	slaveTransmit(PORT_UART_EX);
}

//Case 6:
void mainFSM6(void)
{
	//ADC:
	startAdcConversion();
}

//Case 7:
void mainFSM7(void)
{
	autoStream();
}

//Case 8: User functions
void mainFSM8(void)
{
	#if(RUNTIME_FSM2 == ENABLED)
	user_fsm_2();
	#endif //RUNTIME_FSM2 == ENABLED
}

//Case 9: User Interface
void mainFSM9(void)
{
	//UI RGB LED
	rgbLedRefreshFade();
	rgb_led_ui(0, 0, 0, newMasterCmdLed);    //ToDo add error codes
	if(newMasterCmdLed) {newMasterCmdLed = 0;}

	//Constant LED0 flashing while the code runs
	LED0(rgbLedCenterPulse(12));
}

//10kHz time slot:
//================

void mainFSM10kHz(void)
{
	#ifdef USE_COMM_TEST

		comm_test();

	#endif	//USE_COMM_TEST

	//RGB:
	rgbLedRefresh();

	//Communication with our Master & Slave(s):
	//=========================================

	//New approach - WiP:
	flexsea_receive_from_slave();	//Only for the RS-485 transceivers
	//Master:
	receiveFlexSEAPacket(PORT_USB, &newPacketsFlag, &newMasterCmdLed);
	receiveFlexSEAPacket(PORT_SPI, &newPacketsFlag, &newMasterCmdLed);
	receiveFlexSEAPacket(PORT_WIRELESS, &newPacketsFlag, &newMasterCmdLed);
	//Slave:
	receiveFlexSEAPacket(PORT_RS485_2, &newPacketsFlag, &newSlaveCmdLed);	//Ex
	receiveFlexSEAPacket(PORT_EXP, &newPacketsFlag, &newSlaveCmdLed);
	//Note: this new function doesn't have the SPI error handling code.
	//Variable:
	#ifdef BILATERAL_MASTER
		receiveFlexSEAPacket(PORT_RS485_1, &newPacketsFlag, &newSlaveCmdLed);
	#endif	//BILATERAL_MASTER
	#ifdef BILATERAL_SLAVE

		receiveFlexSEAPacket(PORT_RS485_1, &newPacketsFlag, &newMasterCmdLed);

		//Time to reply - RS-485?
		sendMasterDelayedResponse();

	#endif	//BILATERAL_SLAVE


	//SPI, USB or Wireless reception from a Plan board:
	//flexsea_receive_from_master();

	//RS-485 or UART reception from an Execute board:
	//flexsea_receive_from_slave();

	//Did we receive new commands? Can we parse them?
	//parseMasterCommands(&newCmdLed);
	//parseSlaveCommands(&newCmdLed);

	/*
	//Test: ToDo: this trick can be integrated in the stack, with a programmable
	//number of passes.
	if(commPeriph[PORT_USB].rx.bytesReadyFlag > 0)
	{
		//We still have bytes available, let's call the functions a second time
		flexsea_receive_from_master();
		parseMasterCommands(&newCmdLed);
	}
	*/

	completeSpiTransmit();

	#ifdef USE_SVM
	svmBackgroundMath();
	#endif	//USE_SVM
}

//Test code:
void sendReadRigidPacket(void)
{
	uint8_t info[2] = {PORT_RS485_1, PORT_RS485_1};

	//Prepare and send command:
	//tx_cmd_rigid_r(TX_N_DEFAULT, 0);
	tx_cmd_bilateral_rw(TX_N_DEFAULT, 0);
	packAndSend(P_AND_S_DEFAULT, FLEXSEA_MANAGE_2, info, SEND_TO_SLAVE);
}

//Asynchronous time slots:
//========================

void mainFSMasynchronous(void)
{

}
