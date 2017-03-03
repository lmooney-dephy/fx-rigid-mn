/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-manage' Mid-level computing, and networking
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] fm_slave_comm: Slave R/W
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-23 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef INC_MAST_SLAVE_COMM_H
#define INC_MAST_SLAVE_COMM_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "flexsea_comm.h"

//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

void initSlaveComm(void);
void init_master_slave_comm(void);
void parseMasterCommands(uint8_t *new_cmd);
void parseSlaveCommands(uint8_t *new_cmd);
void slaveTransmit(uint8_t port);
void initCommPeriph(CommPeriph *cp, Port port, PortType pt, uint8_t *input, \
					uint8_t *unpacked, uint8_t *packed, \
					PacketWrapper *inbound, PacketWrapper *outbound);
void linkCommPeriphPacketWrappers(CommPeriph *cp, PacketWrapper *inbound, \
					PacketWrapper *outbound);

//****************************************************************************
// Definition(s):
//****************************************************************************

//****************************************************************************
// Shared Variable(s):
//****************************************************************************

#endif // INC_MAST_SLAVE_COMM_H

