//****************************************************************************
//*  Copyright (c) Federal University of Para, brazil - 2011                 *
//*  Developed at the Research Group on Computer Network and Multimedia      *
//*  Communication (GERCOM)     				             *
//*  All rights reserved    				                     *
//*                                                                          *
//*  Permission to use, copy, modify, and distribute this protocol and its   *
//*  documentation for any purpose, without fee, and without written         *
//*  agreement is hereby granted, provided that the above copyright notice,  *
//*  and the author appear in all copies of this protocol.                   *
//*                                                                          *
//*  Module:   LEACH Clustering Protocol for Castalia Simulator              *
//*  Version:  0.2                                                           *
//*  Author(s): Adonias Pires <adonias@ufpa.br>                              *
//*             Claudio Silva <claudio.silva@itec.ufpa.br>                   *
//****************************************************************************/

cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

struct stringNeighbor
{
	int id;
	int kVal;
	double rssi;
	double residual;
	double location;
};


enum StringPacket_Type
{
	HELLO_WORD_PACKET = 0;
	BROADCAST_PACKET = 1;
	TO_SINK_PACKET = 2;
	SENT_TO_SINK_PACKET = 3;
	END_SETUP_PACKET = 4;
	JOIN_RING_PACKET = 5;
	ACK_RING_PACKET = 6;
	TDMA_RING_PACKET = 7;
	DATA_RING_PACKET = 8;
};

packet StringPacket extends RoutingPacket
{	
	string Isource;
	int sinkPosition;	
	//int kValue;
	int currentRound;
	int hopNumber;
	double sinkRssi;
	double location;
	double residual;
	double moisture;
	int StringPacketKind enum (StringPacket_Type);
	int schedule[];	
	stringNeighbor Neighbors[];
};
