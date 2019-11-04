#include "TestPhases.h"

BeeGroup NectarSource[FoodNumber];
BeeGroup EmployedBee[FoodNumber];
BeeGroup OnLooker[FoodNumber];
BeeGroup BestSource;//The best food source is memorized		


Define_Module(TestPhases);

void TestPhases::startup()
{	
	/*--- The .ned file's parameters ---*/
	percentage = par("percentage");
	roundLength = par("roundLength");
	isSink = par("isSink");
	slotLength = par("slotLength");
	advPacketSize = par("advPacketSize");
	joinPacketSize = par("joinPacketSize");
	tdmaPacketSize = par("tdmaPacketSize");
	dataPacketSize = par("dataPacketSize");
	applicationID = par("applicationID").stringValue(); 
	
	/*--- Class parameters ---*/
	//CHcandidates.clear();
	clusterMembers.clear();
	NeighborsTable.clear();
	clusterHeadMembers.clear();
	tdmaMembers.clear();
	roundNumber=0;
	probability = 0;
	clusterHeadId = 0;
	sinkRssi = 0; 
	maxRssiCH = -150; // Value lower than the sensibility of the node actually fixed at -100dBm
	residualEnergy = resMgrModule->getRemainingEnergy();
	isCH = false;
	isCt = false;
	//endRound = false;
	endFormClus = false;
	int rand();
	readXMLparams();
	
	if (isSink) {
		NodesInfo.clear();
		setTimer(HELLO_WORD, 10.0); // sink Node sends hello word
	} else 
		setTimer(DISCOVERY_ROUND, 1.0);
}


void TestPhases::fromApplicationLayer(cPacket *pkt, const char *destination)
{	
	if(!isSink)
	{
		string dst(destination);
		TestPhasesPacket *netPacket = new TestPhasesPacket("Test phase routing data packet", NETWORK_LAYER_PACKET);
		netPacket->setTestPhasesPacketKind(DATA_CH_PACKET);
		netPacket->setByteLength(dataPacketSize);
		netPacket->setSource(SELF_NETWORK_ADDRESS);
		encapsulatePacket(netPacket, pkt);
		if (!isCH && endFormClus)
		{
			stringstream buffer;
			buffer << clusterHeadId;
			string dst = buffer.str();
			netPacket->setDestination(dst.c_str());	
			bufferPacket(netPacket);
		}	
		else if (!isCH && !endFormClus) 
		{
			tempTXBuffer.push(netPacket);
		}
		else if (isCH) 
		{
			bufferAggregate.push_back(*netPacket);
		}
	}		
}

void TestPhases::fromMacLayer(cPacket *pkt, int macAddress, double rssi, double lqi){
	TestPhasesPacket *netPacket = dynamic_cast <TestPhasesPacket*>(pkt);
	
	if (!netPacket)
		return;
	
	switch (netPacket->getTestPhasesPacketKind()) {
	
		case DATA_CH_PACKET:{
			//Cominfos()<<SELF_NETWORK_ADDRESS<<" "<<macAddress<<" "<<"LEACH_ROUTING_DATA_PACKET "<<netPacket ->getSource() ;
			string dst(netPacket->getDestination());
			if (isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0){
				trace() << "Node " << self << " Aggregate Data Frame ";
				bufferAggregate.push_back(*netPacket);	
			}else if (dst.compare(SINK_NETWORK_ADDRESS) == 0 && isSink) {
				trace() << "Node " << self << " Processing Data Packet ";
				toApplicationLayer(decapsulatePacket(netPacket));
			}
			
			if (isSink)
				trace() << "Iam the sink node " << self << " and i received packet from : " << netPacket->getSource() ;
			break;	
		}

		case HELLO_WORD_PACKET:{ 
			// Wake up and start the Neighbor discovery 
			sinkRssi = rssi ;
			trace() << "Node : " << self << " receives hello packt from : " << macAddress << "with the RSSI :  " << rssi;
			break;	
		}
		
		case BROADCAST_PACKET:{ 
			// Build the neighbor table "Neighbors"
			NeighborInfo neighbor;
			neighbor.id = atoi(netPacket->getSource());
			neighbor.rssi = rssi;
			neighbor.residual = netPacket->getResidual();
			neighbor.location = netPacket->getLocation();
			NeighborsTable.push_back(neighbor);
			double timer = uniform (10.0, 15.0);
			setTimer(SEND_DATA_TO_SINK, timer); // Wait few seconds in order to receive the overall request
			break;	
		}
		
		case TO_SINK_PACKET:{
			// reception of a packet to be sent up to the Sink (BS)
			string dst(netPacket->getDestination());
			string src(netPacket->getSource());
						
			if (src.compare(SELF_NETWORK_ADDRESS) != 0 && isSink){
				// The Sink node which is the final destination of packet receives from each node their information
				NodeInfo node;
				node.id = atoi(netPacket->getSource());
				node.sinkRssi = netPacket->getSinkRssi();
				node.residual = netPacket->getResidual();
				node.location = netPacket->getLocation();
				
				neighbors.clear();
				
				int j = 0;
				while (netPacket->getNeighbors(j).id != NULL){
					NeighborInfo neighbor;
					neighbor.id = netPacket->getNeighbors(j).id;
					neighbor.rssi = netPacket->getNeighbors(j).rssi;
					neighbor.residual = netPacket->getNeighbors(j).residual;
					neighbor.location = netPacket->getNeighbors(j).location;
					neighbors.push_back(neighbor);
					j++ ;
				}
				node.NeighborsTable = neighbors; 
				NodesInfo.push_back(node);
				double timer = uniform(60.0, 70.0);
				setTimer(ABC_COMPUTATION, timer); // Wait few seconds in order to receive the overall request			
			}
			break;	
		}
		
		case END_SETUP_PACKET: {
			if (!isSink){
				int j=0; 
				clusterHeadMembers.clear();
		 		while (netPacket->getNeighbors(j).id != NULL){
		 			if (netPacket->getNeighbors(j).id == atoi(SELF_NETWORK_ADDRESS)){
		 				isCH = true; 
		 				int i = 0;
		 				roundNumber = netPacket->getCurrentRound();
		 				double timer = uniform(2.0, 3.0);
		 				setTimer(JOIN_CH, timer);
		 			}
		 			clusterHeadMembers.push_back(netPacket->getNeighbors(j).id);
		 			j++ ;
		 		}
		 		tdmaMembers.clear();
		 		clusterMembers.clear();
			
		 		trace() << "At the current round : " << roundNumber << ", the Node (CH): " << self << " is Cluster Head, he has the ID of other Cluster Head with :  " << clusterHeadMembers.size() ;
 				for (int k=0; k<clusterHeadMembers.size(); k++)
 					trace() << clusterHeadMembers[k];
	 		}
			break;
		}
		
		// 
		case JOIN_CH_PACKET: {
			if (!isCH && !isSink){
				// Select the CH with the highest rssi or which is close to the CH
				if (abs(rssi) < abs(maxRssiCH)){
					clusterHeadId = atoi(netPacket->getSource());
					maxRssiCH = rssi;
				}
				roundNumber = netPacket->getCurrentRound();
				double timer = uniform (15.0, 20.0);
				setTimer(ACK_CH, timer);
				/*if (isCt) {
					trace() << "In the round : " << roundNumber << ", Node : " << self << " is already a cluster member of cluster led by : " << clusterHeadId;
 					setTimer(NODE_ACK_CH, timer+2.0); // Check if I belong to a Cluster, if yes notify its neighbors
 				}*/
 			}
			break;
		}		
		
		case ACK_CH_PACKET: {
			if (atoi(SELF_NETWORK_ADDRESS) == atoi(netPacket->getDestination()) && isCH) {
				trace() << "For the round : " << roundNumber << ", the Cluster Head : " << self << " receives ack from node " << netPacket->getSource() << " At the send round : " << netPacket->getCurrentRound() << ". However, the CH is in the round : " << roundNumber;
				
				// Before add to the cluster member, check if the node already belongs to cluster
				if (!(std::count(clusterMembers.begin(), clusterMembers.end(), atoi(netPacket->getSource()))))
					clusterMembers.push_back(atoi(netPacket->getSource()));
				
				double timer = uniform (1.0, 2.0);
				setTimer(ACK_CH_MEMBER, timer);
			}
			break;
		}
		
		case CLUSTER_MEMBER_PACKET: {
			if (atoi(SELF_NETWORK_ADDRESS) == atoi(netPacket->getDestination()) && !isCH && !isCt && !isSink) {
				//trace() << "AT For the round : " << roundNumber << ", the simple node : " << self << " receives ack confirming that it belongs to  the cluster of : " << clusterHeadId  << ", but packet was sent by : "  << netPacket->getSource();
				roundNumber = netPacket->getCurrentRound();
				isCt = true; // The node becomes a follower of the CHs
				clusterHeadId = atoi(netPacket->getSource());
				maxRssiCH = rssi;
				clusterMembers.clear(); // Wipe his set of followers
				double timer = uniform(1.0, 2.0);
				setTimer(NODE_ACK_CH, timer);
			}else if (atoi(netPacket->getSource())==clusterHeadId && atoi(SELF_NETWORK_ADDRESS)!=atoi(netPacket->getDestination()) && !isCt) {
				//trace() << "At For the roundss : " << roundNumber << ", the simple node : " << self << " has not received before" << clusterHeadId ;
				roundNumber = netPacket->getCurrentRound();
				double timer = uniform(1.0, 2.0);
				setTimer(ACK_CH, timer); // Resent an ACK packet since, it does not received th OK 
			}else if (isCt && atoi(SELF_NETWORK_ADDRESS) == atoi(netPacket->getDestination()) && !isCH) {
				if (!(std::count(clusterMembers.begin(), clusterMembers.end(), atoi(netPacket->getSource())))){
					clusterMembers.push_back(atoi(netPacket->getSource()));
					//trace() << "During the rounds : " << roundNumber << ", the simple node : " << self << " received the acknowledgment of : " << netPacket->getSource() << " Its set of follower has : " << clusterMembers.size();
					double timer = uniform(2.0, 3.0);
					setTimer(NODE_ACK_TO_CT, timer);
				}
			}else if (isCH && atoi(SELF_NETWORK_ADDRESS) == atoi(netPacket->getDestination())){
				trace() << "During this round : " << roundNumber << ", the CH node : " << self << " received the acknowledgment of : " << netPacket->getSource() << " which number of followers is : " << netPacket->getFollowerNumb();
				tdmaInfo member ; 
				member.id = atoi(netPacket->getSource());
				member.weight = netPacket->getFollowerNumb();
				
				if (!(std::count(clusterMembers.begin(), clusterMembers.end(), member.id)))
					clusterMembers.push_back(member.id);
					
				bool exist = false;
				for (list <tdmaInfo>::iterator ngh = tdmaMembers.begin(); ngh != tdmaMembers.end(); ngh++){
					if ((*ngh).id == member.id)  // Node select the nierest node to follow
						exist = true;
				}
				if (exist == false)
					tdmaMembers.push_back(member);
					
				double timer = uniform (15.0, 20.0);
				setTimer(TDMA_CH, timer);
			}
			
			break;
		}
		
		case NODE_ACK_CH_PACKET: {
			if (!isCt && !isCH && !isSink) {
				roundNumber = netPacket->getCurrentRound();
				//trace() << "Round : " << roundNumber << ", Node : " << self << " receives notification from : " << netPacket->getSource() ;
				for (list <NeighborInfo>::iterator ngh = NeighborsTable.begin(); ngh != NeighborsTable.end(); ngh++){
					if ((*ngh).id == atoi (netPacket->getSource()) && abs(rssi) < abs(maxRssiCH)) { // Node select the nierest node to follow
						clusterHeadId = (*ngh).id; 
						maxRssiCH = rssi;
					}
				}
				
				double timer = uniform(4.0, 5.0);
				setTimer(NODE_ACK_TO_CT, timer);
			}
			
			break;
		}
		
		case TDMA_CH_PACKET:{
			if(!isCH && !isSink) {
				clusterLength = netPacket->getScheduleArraySize();
				for(int i=0; i<netPacket->getScheduleArraySize(); i++) {
					if (netPacket->getSchedule(i)==atoi(SELF_NETWORK_ADDRESS)) {	
						setStateSleep(); // Node sleep if it is not his turn
						endFormClus = true;
						setTimer(START_SLOT, i*slotLength);
						trace() << "Node " << self << " Received TDMA pkt, I am: "<< i << "th";
						break;
					}			
				}
			}
			break;
		}
	}
}


void TestPhases::timerFiredCallback(int index)
{
	switch (index) {
		
		case HELLO_WORD:{	
			// Sink node send hello word to nodes in order to start the discory round
			if (getTimer(HELLO_WORD) != 0) {
					cancelTimer(HELLO_WORD);
			}			
			TestPhasesPacket *helloPkt = new TestPhasesPacket("Hello Word Packet, I'm the sink node", NETWORK_LAYER_PACKET);
			helloPkt->setByteLength(advPacketSize);
			helloPkt->setTestPhasesPacketKind(HELLO_WORD_PACKET);
			helloPkt->setSource(SELF_NETWORK_ADDRESS);
			helloPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(helloPkt, BROADCAST_MAC_ADDRESS);
			
			break;
		}		
		
		case DISCOVERY_ROUND:{	
			// Send Broadcast packet and try to get it
			trace() << "---- The discovery Start ----";
			TestPhasesPacket *brdcstPkt = new TestPhasesPacket("Broadcast initialisation Packet", NETWORK_LAYER_PACKET);
			brdcstPkt->setByteLength(advPacketSize);
			brdcstPkt->setTestPhasesPacketKind(BROADCAST_PACKET);
			brdcstPkt->setSource(SELF_NETWORK_ADDRESS);
			brdcstPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
			brdcstPkt->setResidual(resMgrModule->getRemainingEnergy()); // Residual energy of the node (To change ASAP)
			brdcstPkt->setLocation(uniform(0.0, 0.40)); // Location of the node (burial depth) randomly between the ground surface and 40cm depth
			toMacLayer(brdcstPkt, BROADCAST_MAC_ADDRESS);
			//trace() << "Node " << self << " sends the broadcast packet for the neighbor discovery ";
			trace () << "Node : " << self << " Current round " << roundNumber;
			if (getTimer(DISCOVERY_ROUND) != 0) {
					cancelTimer(DISCOVERY_ROUND);
			}
			setTimer(DISCOVERY_ROUND, roundLength);
			break;
		}		

		case SEND_DATA_TO_SINK:{	
			// Send Data to sink based on the neighbor table of each node
			if (!isSink) {
				TestPhasesPacket *dataPkt = new TestPhasesPacket("Data packet to send up to sink", NETWORK_LAYER_PACKET);
				dataPkt->setNeighborsArraySize(dataPacketSize);
				dataPkt->setByteLength(dataPacketSize);
				dataPkt->setTestPhasesPacketKind(TO_SINK_PACKET);
				dataPkt->setSource(SELF_NETWORK_ADDRESS);
				dataPkt->setDestination(SINK_NETWORK_ADDRESS);
				dataPkt->setSinkRssi(sinkRssi);
				dataPkt->setResidual(resMgrModule->getRemainingEnergy()); // Residual energy
					
				// Construction of the neighbor table
				int i = 0;
				for (list <NeighborInfo>::iterator it = NeighborsTable.begin(); it != NeighborsTable.end(); it++){
						Neighbor Neghborpkt;
						Neghborpkt.id = (*it).id;
						Neghborpkt.rssi = (*it).rssi;
						Neghborpkt.residual = (*it).residual;
						Neghborpkt.location = (*it).location;
						dataPkt->setNeighbors(i, Neghborpkt);
						i++;
				}
				
				// Send data packet to the SINK NODE
				toMacLayer(dataPkt, atoi(SINK_NETWORK_ADDRESS));
				NeighborsTable.clear();
				trace() << "Node " << self << " send data to sink " ;
			}
			if (isSink) {
				//trace() << "Sink node : " << self << " has : "<< NeighborsTable.size() << " Neighbors " ;
				//for (list <NeighborInfo>::iterator it = NeighborsTable.begin(); it != NeighborsTable.end(); it++)
					//trace() << "Sink neighbor : " << (*it).id ;
			}
			if (getTimer(SEND_DATA_TO_SINK) != 0) {
					cancelTimer(SEND_DATA_TO_SINK);
			}
			break;
		}
		
		case ABC_COMPUTATION:{
			CHcandidates.clear();	
			ClusterHeadSet.clear();
			NodesInfo.sort(cmpId); // Classer les noeuds en fonction de leur id
			CHcandidates = NodesInfo;
			CHcandidates.remove_if (lessThanThresh);
			
			trace() << "Nodes set has : " << NodesInfo.size() ;
			trace() << "Candidates set has : " << CHcandidates.size() ;
			for (list <NodeInfo>::iterator it = CHcandidates.begin(); it != CHcandidates.end(); it++)
				trace() << "candida " << (*it).id << " residual energy : " << (*it).residual ;
		
			lb = 0; 	// The candidate set is sort thus the lowest Id is the first element
			ub = CHcandidates.size() - 1;		// The higher element is the last of the vector list
			srand((unsigned)time(NULL));
			// ABC start
			initilize();
			MemorizeBestSource();
			int gen = 0;
			while (gen<maxCycle){
				sendEmployedBees();
				CalculateProbabilities();
				sendOnlookerBees();		
				MemorizeBestSource();
				sendScoutBees();
				MemorizeBestSource();
				/*
				trace() << "Iteration : " << gen << ", BestSource.trueFit = " << BestSource.trueFit ;
				trace() << "Solutions : ";
				for (int i=0 ; i<D; i++)
					trace() << BestSource.code[i];
				*/
				gen++;
			}
			
			//  Creation of the Cluster head set
			for (int j=0 ; j<D; j++) {
				for (list <NodeInfo>::iterator ch = CHcandidates.begin(); ch != CHcandidates.end(); ch++){
					if ((*ch).id == BestSource.code[j])
						ClusterHeadSet.push_back(*ch);
				}
			}
			
			/*trace() << "Round : " << roundNumber <<  ", The cluster Head set has " << ClusterHeadSet.size() << " nodes : ";
			for (list <NodeInfo>::iterator it = ClusterHeadSet.begin(); it != ClusterHeadSet.end(); it++)
					trace() << (*it).id ;	
			*/
			TestPhasesPacket *steadyPkt = new TestPhasesPacket("End of the setup phase", NETWORK_LAYER_PACKET);
			steadyPkt->setNeighborsArraySize(dataPacketSize);
			steadyPkt->setByteLength(advPacketSize);
			steadyPkt->setTestPhasesPacketKind(END_SETUP_PACKET);
			steadyPkt->setCurrentRound(roundNumber);
				
			int in = 0;
			for (list <NodeInfo>::iterator it = ClusterHeadSet.begin(); it != ClusterHeadSet.end(); it++){
				//trace() << "Cluster Head " << (*it).id << " has (residual,location) : (" << (*it).residual << "," << (*it).location << ")";
				Neighbor Neghborpkt;
				Neghborpkt.id = (*it).id;
				Neghborpkt.residual = (*it).residual;
				Neghborpkt.location = (*it).location;
				steadyPkt->setNeighbors(in, Neghborpkt);
				in++;
			}
			toMacLayer(steadyPkt, BROADCAST_MAC_ADDRESS);
			NodesInfo.clear();
			if (getTimer(ABC_COMPUTATION) != 0) {
					cancelTimer(ABC_COMPUTATION);
			}
			roundNumber++;
			setStateRx();
			break;
		}
		
		case JOIN_CH:{	
			// The selected CHs notify their nodes through a JOIN_CH_PACKET
			TestPhasesPacket *joinPkt = new TestPhasesPacket("Cluster Head send joined packet", NETWORK_LAYER_PACKET);
			joinPkt->setByteLength(advPacketSize);
			joinPkt->setTestPhasesPacketKind(JOIN_CH_PACKET);
			joinPkt->setSource(SELF_NETWORK_ADDRESS);
			joinPkt->setCurrentRound(roundNumber);
			joinPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(joinPkt, BROADCAST_MAC_ADDRESS);
			trace() << "The cluster Head with ID : " << self << " Send his joined CH ";
			
			if (getTimer(JOIN_CH) != 0) {
					cancelTimer(JOIN_CH);
			}
			break;
		}	
		
		case ACK_CH:{	
			if (clusterMembers.size() == 0 && !isCt) {
				// The node choose the CH near by comparing the RSSI through the ACK_CH_PACKET packet
				TestPhasesPacket *ackPkt = new TestPhasesPacket("Node send ACK packet to his Cluster Head ", NETWORK_LAYER_PACKET);
				ackPkt->setByteLength(advPacketSize);
				ackPkt->setTestPhasesPacketKind(ACK_CH_PACKET);
				ackPkt->setSource(SELF_NETWORK_ADDRESS);
				ackPkt->setCurrentRound(roundNumber);
				/*
				if (isCH){
					isCH = false; // Because its clusterMembers set is null it is not a CH anymore !
					trace() << "Node : " << self << " is not a CH anymore ";
					for (int i = 0; i < clusterHeadMembers.size(); i++) {
						for (list <NeighborInfo>::iterator it = NeighborsTable.begin(); it != NeighborsTable.end(); it++){
							if ((*it).id == clusterHeadMembers[i]){ // select the Its clusterHead according the clusterHead sets it has
								if (abs((*it).rssi) < abs(maxRssiCH)){
									clusterHeadId = (*it).id;
									maxRssiCH = (*it).rssi;
								}
							}
						}
					}
				}
				*/
				//trace() << "Round : " << roundNumber << " ACK packet, Node : " << self << " sends his ACK packet to its CH : " << clusterHeadId;
				string destination = to_string(clusterHeadId);
				ackPkt->setDestination(destination.c_str());
				toMacLayer(ackPkt, clusterHeadId);
			}
			
			if (getTimer(ACK_CH) != 0) {
					cancelTimer(ACK_CH);
			}
			break;
		}	
		
		case ACK_CH_MEMBER:{	
			
			TestPhasesPacket *ackPkt = new TestPhasesPacket("CH notification to node to be member ", NETWORK_LAYER_PACKET);
			ackPkt->setByteLength(advPacketSize);
			ackPkt->setTestPhasesPacketKind(CLUSTER_MEMBER_PACKET);
			ackPkt->setSource(SELF_NETWORK_ADDRESS);
			string destination = to_string(clusterMembers.back());
			ackPkt->setDestination(destination.c_str());
			ackPkt->setCurrentRound(roundNumber); 
			toMacLayer(ackPkt, clusterMembers.back()); 
			break;
		}
		
		case NODE_ACK_CH:{	
			
			// The node choose the CH near by comparing the RSSI through the ACK_CH_PACKET packet
			if (isCt){
				TestPhasesPacket *ackPkt = new TestPhasesPacket("Node that receive CH join from the CH ", NETWORK_LAYER_PACKET);
				ackPkt->setByteLength(advPacketSize);
				ackPkt->setTestPhasesPacketKind(NODE_ACK_CH_PACKET);
				ackPkt->setSource(SELF_NETWORK_ADDRESS);
				ackPkt->setCurrentRound(roundNumber);
				ackPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				toMacLayer(ackPkt, BROADCAST_MAC_ADDRESS);
				trace() << "Node : " << self << " sends acknowledgment to other node that can be follower : ";
			}
			
			break;
		}	
		
		case NODE_ACK_TO_CT:{	
			
			// The node choose the node he would follow within its cluster
			if (!isCt && !isCH && !isSink){
				TestPhasesPacket *ackPkt = new TestPhasesPacket("Nodes notification to Ct node to know him as follower ", NETWORK_LAYER_PACKET);
				ackPkt->setByteLength(advPacketSize);
				ackPkt->setTestPhasesPacketKind(CLUSTER_MEMBER_PACKET);
				ackPkt->setSource(SELF_NETWORK_ADDRESS);
				string destination = to_string(clusterHeadId);
				ackPkt->setDestination(destination.c_str());
				ackPkt->setCurrentRound(roundNumber); 
				toMacLayer(ackPkt, clusterHeadId);
				
				trace() << "Round : " << roundNumber << ", Node : " << self << " Considers as its next step : " << clusterHeadId ;
			}else if (isCt){
				
				trace() << "Round : " << roundNumber << ", Node : " << self << " has at this stage : " << clusterMembers.size()  << " Followers:";
				for (int i = 0; i<clusterMembers.size(); i++)
					trace() << "Follower : " << clusterMembers[i];
				
				double timer = uniform(5.0, 10.0);
				setTimer(ACK_WEIGHT_CH, timer);
			}
			
			break;
		}	
		
		case ACK_WEIGHT_CH:
		{
			if (isCt) {
				TestPhasesPacket *fllwPkt = new TestPhasesPacket("Nodes notification to Ct node to know him as follower ", NETWORK_LAYER_PACKET);
				fllwPkt->setByteLength(advPacketSize);
				fllwPkt->setTestPhasesPacketKind(CLUSTER_MEMBER_PACKET);
				fllwPkt->setSource(SELF_NETWORK_ADDRESS);
				string destination = to_string(clusterHeadId);
				fllwPkt->setDestination(destination.c_str());
				fllwPkt->setCurrentRound(roundNumber);
				fllwPkt->setFollowerNumb(clusterMembers.size() + 1); // the number of follower + the node itself in order to synchronize the TDMA
				toMacLayer(fllwPkt, clusterHeadId);					
			}
			if (getTimer(ACK_WEIGHT_CH) != 0) {
					cancelTimer(ACK_WEIGHT_CH);
			}
			break;
		}
		
		case TDMA_CH:{
			double energy = resMgrModule->getSpentEnergy();
			double remainEnergy = resMgrModule->getRemainingEnergy();
						
			if (clusterMembers.size()!=0 && isCH){
				endFormClus = true ;
				//sort(clusterMembers.begin(), clusterMembers.end());
				//sort(clusterMembers.begin(), clusterMembers.end());
				trace() <<  "Round " << roundNumber << " End of Cluster head : " << self << " has node : " << clusterMembers.size();
				for (int i = 0; i<clusterMembers.size(); i++)
					trace() << "cluster member : " << clusterMembers[i];
					
				trace() <<  "Round " << roundNumber << " End of Cluster head : " << self << " has node : " << tdmaMembers.size();
				for (list <tdmaInfo>::iterator ch = tdmaMembers.begin(); ch != tdmaMembers.end(); ch++)
					trace() << "cluster member : " << (*ch).id << ", weight : " << (*ch).weight ;
				
				TestPhasesPacket *crtlPkt = new TestPhasesPacket("ClusterHead TDMA Packet", NETWORK_LAYER_PACKET);
				crtlPkt->setByteLength(tdmaPacketSize);
				crtlPkt->setTestPhasesPacketKind(TDMA_CH_PACKET);
				crtlPkt->setSource(SELF_NETWORK_ADDRESS);
				crtlPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				clusterLength = clusterMembers.size();	
				crtlPkt->setScheduleArraySize(clusterMembers.size());	
				for(int i=0; i<clusterLength; i++) 
					crtlPkt->setSchedule(i,clusterMembers[i]);
				
				toMacLayer(crtlPkt, BROADCAST_MAC_ADDRESS);
				trace() << "Node " << self << " Sent TDMA pkt";
				setTimer(START_SLOT, clusterLength*slotLength);
			} 
			
			else setTimer(START_SLOT, slotLength);
								
			if (getTimer(TDMA_CH) != 0) {
					cancelTimer(TDMA_CH);
			}
			break;
		}
		
		case START_SLOT:{
			double energy = resMgrModule->getSpentEnergy();
			double remainEnergy = resMgrModule->getRemainingEnergy();
			
			if (isCH){
				sendAggregate(); 
				processBufferedPacket();
				trace() << "Cluster Head : " << self << " Sent Pkt Aggr"  << "\n";
				setTimer(END_SLOT, 1);
			}
			if (!isCH){
				//CHInfo info = *CHcandidates.begin();
				//int power = maxPower - ((maxRssiCH)-(sensibility));
				//levelTxPower(power);
				trace() << "Node " << self << " Sent Data Packet"  << "\n";
				processBufferedPacket();
				setTimer(END_SLOT, slotLength);
			}
				
			if (getTimer(START_SLOT) != 0) {
					cancelTimer(START_SLOT);
			}
			break;
		}
		
		case END_SLOT:
		{
			if (!isSink) {
				isCH = false;
				isCt = false;		
				clusterMembers.clear();
				NeighborsTable.clear();
				setStateSleep();				
			}
			if (getTimer(END_SLOT) != 0) {
					cancelTimer(END_SLOT);
			}
			break;
		}
	}
}

void TestPhases::sendAggregate()
{
	if(bufferAggregate.size()!=0)
	{	
		double bitsLength = bufferAggregate.size() * dataPacketSize * 1000;
		double energyBit = (aggrConsumption / pow(10,9)) * bitsLength;
		powerDrawn(energyBit);
		TestPhasesPacket *aggrPacket = new TestPhasesPacket("ClusterHead Aggredated Packet", NETWORK_LAYER_PACKET);
		aggrPacket->setByteLength(dataPacketSize+4);
		aggrPacket->setTestPhasesPacketKind(DATA_CH_PACKET);
		aggrPacket->setSource(SELF_NETWORK_ADDRESS);
		aggrPacket->setDestination(SINK_NETWORK_ADDRESS);
		
		ApplicationPacket *newPacket = new ApplicationPacket("App generic packet", APPLICATION_PACKET);
		newPacket->setData(0);
		newPacket->getAppNetInfoExchange().destination = string("0");
		newPacket->getAppNetInfoExchange().source = selfAddress;
		newPacket->getAppNetInfoExchange().timestamp = simTime();
		newPacket->setApplicationID(applicationID.c_str());
		encapsulatePacket(aggrPacket, newPacket);

		bufferPacket(aggrPacket);
		bufferAggregate.clear();
	}
}

void TestPhases::processBufferedPacket()
{
	while (!tempTXBuffer.empty())
	{
		cPacket *pkt = tempTXBuffer.front();	
		TestPhasesPacket *netPacket = dynamic_cast <TestPhasesPacket*>(pkt);
		stringstream buffer;
		buffer << clusterHeadId;
		string dst = buffer.str();
		netPacket->setDestination(dst.c_str());
		bufferPacket(netPacket); 
		tempTXBuffer.pop();
	}

	while (!TXBuffer.empty()) {
		toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
		TXBuffer.pop();
	}	
}

void TestPhases::setStateRx()
{
	send(createRadioCommand(SET_STATE, RX), "toMacModule");	
}

void TestPhases::setPowerLevel(double powerLevel)
{
	send(createRadioCommand(SET_TX_OUTPUT, powerLevel), "toMacModule");
}

void TestPhases::setStateSleep()
{
	send(createRadioCommand(SET_STATE, SLEEP), "toMacModule");
}

void TestPhases::levelTxPower(int linkBudget)
{
	vector<int>::iterator constIterator;
	for (constIterator = powers.begin();
		constIterator != powers.end();
		constIterator++ ) {
		if(*constIterator > (linkBudget))
		{
			setPowerLevel(*constIterator);
			break;
		}
	}
}

void TestPhases::readXMLparams()
{
	cXMLElement *rootelement = par("powersConfig").xmlValue();
	if (!rootelement) endSimulation();
	cXMLElement* data = rootelement->getFirstChildWithTag("maxPower");
	maxPower = atoi(data->getNodeValue());
	data = rootelement->getFirstChildWithTag("sensibility");
	sensibility = atoi(data->getNodeValue());
	data = rootelement->getFirstChildWithTag("aggrConsumption");
	aggrConsumption = atoi(data->getNodeValue());
	cXMLElementList sources = rootelement->getChildrenByTagName("power");
	for (int s = 0; s < sources.size(); s++)
		powers.push_back(atoi(sources[s]->getNodeValue()));
	if (powers.size() < 1) endSimulation();
	sort(powers.begin(), powers.end());
}

bool cmpResidual(NodeInfo a, NodeInfo b){
	return (a.residual > b.residual);
}

bool cmpId(NodeInfo a, NodeInfo b){
	return (a.id < b.id);
}

bool lessThanThresh(NodeInfo a){
	return (a.residual < THRESHOLD_ENERGY);
}

bool cmpRssi(NeighborInfo a, NeighborInfo b){
	return (a.rssi > b.rssi);
}

// ABC functions implementation 

double random(double start, double end){
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

void initilize() {
	int i, j, temp;
	for (i = 0;i<FoodNumber;i++){
		for (j = 0;j<D;j++){
			bool exist = true;
			while (exist){
			//initialized food matrix FoodNumber*D and fulled with random values
				int id = randint(lb, ub);
				list <NodeInfo>::iterator it = CHcandidates.begin();
				advance(it, id);
				temp = 	(*it).id;		
				exist = alreadyExist(NectarSource[i].code, temp, D);
			}
			NectarSource[i].code[j] = temp ; //random(lb, ub);
			
			EmployedBee[i].code[j] = NectarSource[i].code[j];
			OnLooker[i].code[j] = NectarSource[i].code[j];
			BestSource.code[j] = NectarSource[0].code[j];
		}
			
		//initialized the food
		NectarSource[i].trueFit = calculationTruefit(NectarSource[i]);
		NectarSource[i].fitness = calculationFitness(NectarSource[i].trueFit);
		NectarSource[i].rfitness = 0;
		NectarSource[i].trail = 0;
		//initialized the employed
		EmployedBee[i].trueFit = NectarSource[i].trueFit;
		EmployedBee[i].fitness = NectarSource[i].fitness;
		EmployedBee[i].rfitness = NectarSource[i].rfitness;
		EmployedBee[i].trail = NectarSource[i].trail;
		//initialized the onlooker
		OnLooker[i].trueFit = NectarSource[i].trueFit;
		OnLooker[i].fitness = NectarSource[i].fitness;
		OnLooker[i].rfitness = NectarSource[i].rfitness;
		OnLooker[i].trail = NectarSource[i].trail;
	}
	
	//initialized the best food
	BestSource.trueFit = NectarSource[0].trueFit;
	BestSource.fitness = NectarSource[0].fitness;
	BestSource.rfitness = NectarSource[0].rfitness;
	BestSource.trail = NectarSource[0].trail;
}


double calculationTruefit(BeeGroup bee)//calculate the value of the function
{
	double f1 = 0;
	double f2 = 0;
	
	for (int j=0; j<D; j++) {
		for (list <NodeInfo>::iterator ch = CHcandidates.begin(); ch != CHcandidates.end(); ch++) {
			if ((*ch).id == bee.code[j]) {
				f1 += abs((*ch).residual); /// (*ch).sinkRssi ;
				(*ch).NeighborsTable.sort(cmpRssi);
				double max = (*ch).NeighborsTable.front().rssi;
				double min = (*ch).NeighborsTable.back().rssi;
				f2 += abs(max/(max-min));
			}		
  	}  	
  }
  f1 = 1/f1;
  
	return ALHPA*f1 + (1-ALHPA)*f2 ;
}


// Computation of the fitness value
double calculationFitness(double truefit)
{
	//double fitnessResult = 0;
	/*
	if (truefit >= 0){
		fitnessResult = 1 / (truefit + 1);
	}
	else{
		fitnessResult = 1 + abs(truefit);
	}
	return fitnessResult;
	*/
	return truefit;
}


/***********************************************************************************/
//															EMPLOYED BEE PHASE
/***********************************************************************************/
void sendEmployedBees() {
	int i, j, k, temp;
	bool exist, candidate;
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	for (i = 0;i<FoodNumber;i++){
		//The parameter to be changed is determined randomly
		exist = true;
		candidate = false;
		for (j = 0;j<D;j++){
			EmployedBee[i].code[j] = NectarSource[i].code[j];
		}		
		while (exist == true || candidate == false) {
			param2change = randint(0, D-1);
			//A randomly chosen solution is used in producing a mutant solution of the solution i
			
			while (1){
				k = randint(0, FoodNumber-1);
				if (k != i){
					break;
				}
			}
			
			//sol(Param2Change)=Foods(i,Param2Change)+(Foods(i,Param2Change)-Foods(neighbour,Param2Change))*(rand-0.5)*2;
			//v_{ij}=x_{ij}+\phi_{ij}*(x_{ij}-x_{kj})
			Rij = random(-1, 1);
			temp = NectarSource[i].code[param2change] + Rij*(NectarSource[i].code[param2change] - NectarSource[k].code[param2change]);
			
			if (temp>CHcandidates.back().id){
				temp = CHcandidates.back().id;
			}
			if (temp<CHcandidates.front().id){
				temp = CHcandidates.front().id;
			}
			exist = alreadyExist(EmployedBee[i].code, temp, D);
			candidate = existCandidateSet(temp);
		}
		
		EmployedBee[i].code[param2change] = temp;
		//evaluate new solution
		EmployedBee[i].trueFit = calculationTruefit(EmployedBee[i]);
		EmployedBee[i].fitness = calculationFitness(EmployedBee[i].trueFit);
		//a greedy selection is applied between the current solution i and its mutant
		//If the mutant solution is better than the current solution i,
		//replace the solution with the mutant and reset the trial counter of solution i
		if (EmployedBee[i].trueFit<NectarSource[i].trueFit){
			for (j = 0;j<D;j++){
				NectarSource[i].code[j] = EmployedBee[i].code[j];
			}
			NectarSource[i].trail = 0; // The employed bee is still active
			NectarSource[i].trueFit = EmployedBee[i].trueFit;
			NectarSource[i].fitness = EmployedBee[i].fitness;
		}
		else{
			//if the solution i can not be improved, increase its trial counter
			NectarSource[i].trail++;
		}
	}
}

/***********************************************************************************/
//  Calculate Probabilities
//  A food source is chosen with the probability which is proportioal to its quality
/***********************************************************************************/

void CalculateProbabilities() {
	int i;
	double somFit = 0;
	
	for (i = 0; i<FoodNumber;i++) {
		somFit += NectarSource[i].fitness;
	}
	
	for (i = 0; i<FoodNumber;i++) {
		NectarSource[i].rfitness = (NectarSource[i].fitness)/somFit;
	}
	/*
	int i;
	double maxfit;
	maxfit = NectarSource[0].fitness;
	for (i = 1;i<FoodNumber;i++){
		if (NectarSource[i].fitness>maxfit)
			maxfit = NectarSource[i].fitness;
	}

	for (i = 0;i<FoodNumber;i++){
		NectarSource[i].rfitness = (0.9*(NectarSource[i].fitness / maxfit)) + 0.1;
	}
	*/
}

/***********************************************************************************/
//         ONLOOKER BEE PHASE
/***********************************************************************************/

void sendOnlookerBees() {
	int i, j, t, k, temp;
	bool exist, candidate;
	double R_choosed;//the rate to be choosed
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	i = 0;
	t = 0;
	while (t<FoodNumber) {
		R_choosed = random(0, 1);
		//A food source is chosen with the probability which is proportioal to its quality
		if (R_choosed<NectarSource[i].rfitness) {
			t++;
			exist = true;
			candidate = false;
			for (j = 0;j<D;j++){
				OnLooker[i].code[j] = NectarSource[i].code[j];
			}
			while (exist == true || candidate == false) {
				//The parameter to be changed is determined randomly
				param2change = randint(0, D);
				//A randomly chosen solution is used in producing a mutant solution of the solution i
				//Randomly selected solution must be different from the solution i
				while (1) {
					k = randint(0, FoodNumber);
					if (k != i) {
						break;
					}
				}

				//sol(Param2Change)=Foods(i,Param2Change)+(Foods(i,Param2Change)-Foods(neighbour,Param2Change))*(rand-0.5)*2;
				Rij = random(-1, 1);
				temp = NectarSource[i].code[param2change] + Rij*(NectarSource[i].code[param2change] - NectarSource[k].code[param2change]);

				//shifted onto the boundaries if generated parameter value is out of boundaries
				if (temp<CHcandidates.front().id){
					temp = CHcandidates.front().id;
				}
				if (temp>CHcandidates.back().id){
					temp = CHcandidates.back().id;
				}
				
				exist = alreadyExist(NectarSource[i].code, temp, D);
				candidate = existCandidateSet(temp);
			}
			
			OnLooker[i].code[param2change] = temp; // replace the value of the onlooker bee			
			
			OnLooker[i].trueFit = calculationTruefit(OnLooker[i]);
			OnLooker[i].fitness = calculationFitness(OnLooker[i].trueFit);

			//a greedy selection is applied between the current solution i and its mutant
			//If the mutant solution is better than the current solution i,
			//replace the solution with the mutant and reset the trial counter of solution i
			if (OnLooker[i].trueFit<NectarSource[i].trueFit){
				for (j = 0;j<D;j++){
					NectarSource[i].code[j] = OnLooker[i].code[j];
				}
				NectarSource[i].trail = 0;
				NectarSource[i].trueFit = OnLooker[i].trueFit;
				NectarSource[i].fitness = OnLooker[i].fitness;
			}
			else{
				NectarSource[i].trail++;
			}
		}
		i++;
		if (i == FoodNumber){
			i = 0;
		}
	}
}

/*****************************************************************************/
//SCOUT BEE PHASE
//determine the food sources whose trial counter exceeds the "limit" value
//only one scout is allowed to occur in each cycle
/*****************************************************************************/

void sendScoutBees()
{
	int maxtrialindex, i, j, temp;
	double R;//random number between [0,1]
	bool exist, candidate;
	maxtrialindex = 0;
	for (i = 0;i<FoodNumber;i++){
		if (NectarSource[i].trail>NectarSource[maxtrialindex].trail){
			maxtrialindex = i;
		}
	}

	//determine the food sources whose trial counter exceeds the "limit" value
	if (NectarSource[maxtrialindex].trail >= limit){
		//reinitailized food
		for (j = 0;j<D;j++){
			exist = true;
			candidate = false;
			while (exist == true || candidate == false) {
				R = random(0, 1);
				temp = int(lb + R*(CHcandidates.back().id - CHcandidates.front().id));
				
				exist = alreadyExist(NectarSource[maxtrialindex].code, temp, D);
				candidate = existCandidateSet(temp);
			}
			NectarSource[maxtrialindex].code[j] = temp;
		}
		NectarSource[maxtrialindex].trail = 0;
		NectarSource[maxtrialindex].trueFit = calculationTruefit(NectarSource[maxtrialindex]);
		NectarSource[maxtrialindex].fitness = calculationFitness(NectarSource[maxtrialindex].trueFit);
	}
}

void MemorizeBestSource() // The best food source is memorized
{
	int i, j;
	for (i = 0;i<FoodNumber;i++){
		if (NectarSource[i].trueFit < BestSource.trueFit){
			for (j = 0;j<D;j++){			
				BestSource.code[j] = NectarSource[i].code[j];
			}
			BestSource.trueFit = NectarSource[i].trueFit;
		}
	}
}

bool alreadyExist(int T[],int valeurATrouver, int lenght)
{
	for (int i=0;i<lenght;i++) {
		if (T[i]==valeurATrouver) {
	  	return true;
		}
	}	
	return false;
}

bool existCandidateSet(int valeurATrouver)
{
	for (list <NodeInfo>::iterator it = CHcandidates.begin(); it != CHcandidates.end(); it++) {
		if ((*it).id == valeurATrouver) {
	  	return true;
		}
	}
	return false;
}

