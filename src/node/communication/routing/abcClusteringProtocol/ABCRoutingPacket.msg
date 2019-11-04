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


enum ABCRoutingPacket_Type
{
	ABC_ROUTING_ADV_PACKET 	= 1;		
	ABC_ROUTING_JOIN_PACKET = 2;		
	ABC_ROUTING_TDMA_PACKET = 3;		
	ABC_ROUTING_DATA_PACKET = 4;
};

packet ABCRoutingPacket extends RoutingPacket
{	
	int type_packet;	
	int hop_numb;
	int sum_energy;
	int residual_energy;
	int min_energy;	
	int ABCRoutingPacketKind enum (ABCRoutingPacket_Type);   
	int schedule[];		
	int path[];	
};
