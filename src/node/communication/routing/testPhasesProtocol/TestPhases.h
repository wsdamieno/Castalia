#ifndef _TESTPHASES_H_
#define _TESTPHASES_H_

#include <queue>
#include <vector>
#include <omnetpp.h>
#include <algorithm>
#include <string>
#include <math.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <experimental/random>
#include <stdio.h>
#include <iostream>
#include <time.h> 
#include <sstream>
#include "VirtualRouting.h"
#include "VirtualApplication.h"	
#include "TestPhasesPacket_m.h"
#include "ApplicationPacket_m.h"
#include "NoMobilityManager.h"

#define THRESHOLD_ENERGY 4680 // Corresponding to 25% remaining energy 
#define ALHPA 0.2 // Corresponding of alpha in the equation alpha*f1 +(1-alpha)*f2  

using namespace std;
using namespace std::experimental;

const int NP = 200;//The number of colony size (employed bees+onlooker bees)
const int FoodNumber = NP / 2;//The number of food sources equals the half of the colony size
const int limit = 20;//A food source which could not be improved through limit trials is abandoned by its employed bee
const int maxCycle = 50;//The number of cycles for search

const int D = 25;//The number of parameters of the problem to be optimized
int lb ;//lower bounds of the parameters
int ub ;//upper bound of the parameters
double result[maxCycle] = { 0 };

//struct of Bees
struct BeeGroup
{
	int code[D];//the number of weights and bias is D
	double trueFit;//objective function value
	double fitness;//fitness is a vector holding fitness (quality) values associated with food sources
	double rfitness;//a vector holding probabilities of food sources (solutions) to be chosen
	int trail;//trial is a vector holding trial numbers through which solutions can not be improved
}Bee[FoodNumber];

enum TestPhasesTimers {
	HELLO_WORD = 0,
	DISCOVERY_ROUND = 1,
	DISCOVERY_ACK = 7,
	SEND_DATA_TO_SINK = 2,	
	ABC_COMPUTATION = 3,	
	JOIN_CH = 4,	
	ACK_CH = 5,
	ACK_CH_MEMBER = 6,
	NODE_ACK_CH = 7,
	NODE_ACK_TO_CT = 8,	
	ACK_WEIGHT_CH = 9,
	TDMA_CH = 10,			
	START_SLOT = 11,	
	END_SLOT = 12,
};

struct NeighborInfo
{
	int id;
	double rssi;
	double residual;
	double location;
};

struct tdmaInfo
{
	int id;
	int weight;
};

class TestPhases : public VirtualRouting {

private:
	
	string applicationID;
	int advPacketSize;
	int tdmaPacketSize;
	int dataPacketSize;
	int joinPacketSize;

	double maxPower;
	double sensibility;
	double aggrConsumption;
	double residualEnergy;
	
	double slotLength;
	int clusterLength;
	double percentage;
	double probability;
	double roundLength;
	double sinkRssi; //The RSSI of the BS in order to have an idea of the distance between node and the BS 
	int clusterHeadId; // the Id of the selected CH base on the max RSSI
	double maxRssiCH;
	int roundNumber;
	
	bool isCH;
	bool isSink;
	bool isCt;
	bool endFormClus;
	//bool endRound;
	
	// ABC parameters		
	vector<RoutingPacket> bufferAggregate;
	
	vector <int> powers;
	vector <int> clusterMembers;
	vector <int> clusterHeadMembers;
	
	queue <cPacket *> tempTXBuffer;
	
	list <NeighborInfo> NeighborsTable;
	list <NeighborInfo> neighbors;
	list <tdmaInfo> tdmaMembers;
	
	// *****************

	
protected:
	void startup();
	
	void fromApplicationLayer(cPacket *, const char *);
	void timerFiredCallback(int);
	void processBufferedPacket();
	
	void fromMacLayer(cPacket *, int, double, double);
	void sendAggregate();
	void setPowerLevel(double);
	void setStateSleep();
	void setStateRx();
	void levelTxPower(int);	
	void readXMLparams();
	
};

struct NodeInfo
{
	int id;
	double sinkRssi;
	double residual;
	double location;
	list <NeighborInfo> NeighborsTable ;
};

list <NodeInfo> NodesInfo;
list <NodeInfo> CHcandidates;
list <NodeInfo> ClusterHeadSet;

bool cmpResidual(NodeInfo a, NodeInfo b);
bool cmpId(NodeInfo a, NodeInfo b);
bool lessThanThresh(NodeInfo a);
bool cmpRssi(NeighborInfo a, NeighborInfo b);
bool alreadyExist(int T[],int valeurATrouver, int lenght);
bool existCandidateSet(int valeurATrouver);

// ABC funtions
double random(double, double);
void initilize();

void sendEmployedBees();
void sendOnlookerBees();
void sendScoutBees();
void MemorizeBestSource();
double calculationTruefit(BeeGroup);
double calculationFitness(double);
void CalculateProbabilities();



#endif			
