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

struct Neighbor
{
	int id;
	double rssi;
	double residual;
	double location;
};

struct tdma
{
	int id;
	int weight;
};


enum TestPhasesPacket_Type
{
	HELLO_WORD_PACKET = 0;
	BROADCAST_PACKET = 1;
	TO_SINK_PACKET = 2;
	SENT_TO_SINK_PACKET = 3;
	END_SETUP_PACKET = 4;
	JOIN_CH_PACKET = 5;
	ACK_CH_PACKET = 6;
	CLUSTER_MEMBER_PACKET = 7;
	NODE_ACK_CH_PACKET = 8;
	TDMA_CH_PACKET = 9;
	DATA_CH_PACKET = 10;	
};

packet TestPhasesPacket extends RoutingPacket
{	
	int hopNumb;
	int currentRound;
	double sinkRssi;
	double location;
	double sumEnergy;
	double residual;
	double moisture;
	int followerNumb;	
	int TestPhasesPacketKind enum (TestPhasesPacket_Type);
	int schedule[];	
	Neighbor Neighbors[];
};
