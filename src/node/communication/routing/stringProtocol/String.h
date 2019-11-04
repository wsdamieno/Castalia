#ifndef _STRING_H_
#define _STRING_H_

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
#include "StringPacket_m.h"
#include "ApplicationPacket_m.h"
#include "NoMobilityManager.h"

#define THRESHOLD_ENERGY 4680 // Corresponding to 25% remaining energy 
#define MIN_RSSI -150 // The minimum value of the Rssi (node sensibility)
#define MAX_IMAGINARY_NODE 5000 // An imaginary node iD

using namespace std;
using namespace std::experimental;

const int stringNP = 300;//The number of colony size (employed bees+onlooker bees)
const int stringFoodNumber = stringNP / 2;//The number of food sources equals the half of the colony size
const int stringLimit = 20;//A food source which could not be improved through limit trials is abandoned by its employed bee
const int stringMaxCycle = 50;//The number of cycles for search

const int stringD = 50;//The number of parameters of the problem to be optimized
int stringLb ;//lower bounds of the parameters
int stringUb ;//upper bound of the parameters
double stringResult[stringMaxCycle] = { 0 };

//struct of Bees
struct StringBeeGroup
{
	int code[stringD];//the number of weights and bias is D
	double trueFit;//objective function value
	double fitness;//fitness is a vector holding fitness (quality) values associated with food sources
	double rfitness;//a vector holding probabilities of food sources (solutions) to be chosen
	int trail;//trial is a vector holding trial numbers through which solutions can not be improved
}StringBee[stringFoodNumber];

enum StringTimers {
	HELLO_WORD = 0,
	DISCOVERY_ROUND = 1,
	DISCOVERY_ACK = 7,
	SEND_DATA_TO_SINK = 2,	
	ABC_COMPUTATION = 3,	
	JOIN_RING = 4,	
	ACK_RING = 5,
	TDMA_RING = 6,			
	START_SLOT = 11,	
	END_SLOT = 12,
};

struct StringNeighborInfo
{
	int id;
	double rssi;
	double residual;
	double location;
};

class String : public VirtualRouting {

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
	int followersLength;
	int initialSender;
	double percentage;
	double probability;
	double roundLength;
	double sinkRssi; //The RSSI of the BS in order to have an idea of the distance between node and the BS 
	int ringFollowing; // the Id of the selected CH base on the max RSSI
	double maxRssiCH;
	int roundNumber;
	
	bool isRingMember; //the ID of a node is found inside the ring table !
	int sinkPosition;  // The position of the sink inside the generated ring
	int nodePosition;  // The position of the node inside the generated ring
	int kValue;
	int frwdCount;
	bool isSink;
	bool isCt;
	bool endFormClus;
	//bool endRound;
	
	// ABC parameters		
	vector<RoutingPacket> stringBufferAggregate;
	
	vector <int> stringPowers;
	vector <int> ringMembers;
	vector <int> stringMemberFollowers;
	
	queue <cPacket *> stringTempTXBuffer;
	
	list <StringNeighborInfo> stringNeighborsTable;
	
	// *****************

	
protected:
	void startup();
	
	void fromApplicationLayer(cPacket *, const char *);
	void timerFiredCallback(int);
	void processBufferedPacket();
	
	void fromMacLayer(cPacket *, int, double, double);
	void Aggregate();
	void setPowerLevel(double);
	void setStateSleep();
	void setStateRx();
	void levelTxPower(int);	
	void readXMLparams();
	
};

struct stringNodeInfo
{
	int id;
	double sinkRssi;
	double residual;
	double location;
	list <StringNeighborInfo> NeighborsTable ;
};

list <stringNodeInfo> stringNodesInfo;
list <stringNodeInfo> stringCHcandidates;
list <stringNodeInfo> stringClusterHeadSet;

bool stringCmpResidual(stringNodeInfo a, stringNodeInfo b);
bool stringCmpId(stringNodeInfo a, stringNodeInfo b);
bool stringLessThanThresh(stringNodeInfo a);
bool stringCmpRssi(StringNeighborInfo a, StringNeighborInfo b);
bool stringCmpNeighborNumb(stringNodeInfo a, stringNodeInfo b);
bool stringIsNeighbor(stringNodeInfo node, stringNodeInfo next);
bool stringAlreadyExist(int T[],int valeurATrouver, int lenght);
bool stringExistCandidateSet(int valeurATrouver);
bool stringExistPath(int T[], int valeurATrouver, int id, int lenght);
int getKValue(int nodePosition, int sinkPosition);

// ABC funtions
double stringRandom(double, double);
void stringInitilize();

void EmployedBees();
void OnlookerBees();
void ScoutBees();
void StringMemorizeBestSource();
double stringCalculationTruefit(StringBeeGroup);
double stringCalculationFitness(double);
void stringCalculateProbabilities();



#endif			
