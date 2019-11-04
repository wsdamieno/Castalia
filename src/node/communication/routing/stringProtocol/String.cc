#include "String.h"

StringBeeGroup StringNectarSource[stringFoodNumber];
StringBeeGroup StringEmployedBee[stringFoodNumber];
StringBeeGroup StringOnLooker[stringFoodNumber];
StringBeeGroup StringBestSource;//The best food source is memorized		


Define_Module(String);

void String::startup()
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
	stringNeighborsTable.clear();
	ringMembers.clear();
	stringMemberFollowers.clear();
	kValue = 0;
	ringFollowing = MAX_IMAGINARY_NODE;
	sinkRssi = 0; 
	maxRssiCH = MIN_RSSI; // Value lower than the sensibility of the node actually fixed at -100dBm
	residualEnergy = resMgrModule->getRemainingEnergy();
	isRingMember = false;
	isCt = false;
	//endRound = false;
	endFormClus = false;
	int rand();
	readXMLparams();
	
	if (isSink) {
		stringNodesInfo.clear();
		setTimer(HELLO_WORD, 5.0); // sink Node sends hello word
	} else 
		setTimer(DISCOVERY_ROUND, 1.0);
}


void String::fromApplicationLayer(cPacket *pkt, const char *destination)
{	
	if(!isSink) {
		string dst(destination);
		StringPacket *netPacket = new StringPacket("Test phase routing data packet", NETWORK_LAYER_PACKET);
		netPacket->setStringPacketKind(DATA_RING_PACKET);
		netPacket->setByteLength(dataPacketSize);
		netPacket->setSource(SELF_NETWORK_ADDRESS);
		encapsulatePacket(netPacket, pkt);
		if (!isRingMember && endFormClus) {
			stringstream buffer;
			buffer << ringFollowing;
			string dst = buffer.str();
			netPacket->setDestination(dst.c_str());	
			bufferPacket(netPacket);
		}	
		else if (!isRingMember && !endFormClus) {
			stringTempTXBuffer.push(netPacket);
		}
		else if (isRingMember) {
			stringBufferAggregate.push_back(*netPacket);
		}
	}		
}

void String::fromMacLayer(cPacket *pkt, int macAddress, double rssi, double lqi){
	StringPacket *netPacket = dynamic_cast <StringPacket*>(pkt);
	
	if (!netPacket)
		return;
	
	switch (netPacket->getStringPacketKind()) {
	
		case HELLO_WORD_PACKET:{ 
			// Wake up and start the Neighbor discovery 
			sinkRssi = rssi ;
			trace() << "Node : " << self << " receives hello packt from : " << macAddress << "with the RSSI :  " << rssi;
			break;	
		}
		
		case BROADCAST_PACKET:{ 
			// Build the neighbor table "Neighbors"
			StringNeighborInfo neighbor;
			neighbor.id = atoi(netPacket->getSource());
			neighbor.rssi = rssi;
			neighbor.residual = netPacket->getResidual();
			neighbor.location = netPacket->getLocation();
			stringNeighborsTable.push_back(neighbor);
			double timer = uniform (15.0, 18.0);
			setTimer(SEND_DATA_TO_SINK, timer); // Wait few seconds in order to receive the overall request
			break;	
		}
		
		case TO_SINK_PACKET:{
			// reception of a packet to be sent up to the Sink (BS)
			string dst(netPacket->getDestination());
			string src(netPacket->getSource());
						
			if (src.compare(SELF_NETWORK_ADDRESS) != 0 && isSink){
				// The Sink node which is the final destination of packet receives from each node their information
				stringNodeInfo node;
				node.id = atoi(netPacket->getSource());
				node.sinkRssi = netPacket->getSinkRssi();
				node.residual = netPacket->getResidual();
				node.location = netPacket->getLocation();
				
				list <StringNeighborInfo> neighbors;
				
				int j = 0;
				while (netPacket->getNeighbors(j).id != NULL){
					StringNeighborInfo neighbor;
					neighbor.id = netPacket->getNeighbors(j).id;
					neighbor.rssi = netPacket->getNeighbors(j).rssi;
					neighbor.residual = netPacket->getNeighbors(j).residual;
					neighbor.location = netPacket->getNeighbors(j).location;
					neighbors.push_back(neighbor);
					j++ ;
				}
				node.NeighborsTable = neighbors; 
				stringNodesInfo.push_back(node);
				trace() << "During the round : " << roundNumber << ", the Sink node (CH): " << self << " receives data from : " <<  macAddress ;
				double timer = uniform(30.0, 35.0);
				setTimer(ABC_COMPUTATION, timer); // Wait few seconds in order to receive the overall request			
			}
			break;	
		}
		
		case END_SETUP_PACKET: {
			if (!isSink){
				ringMembers.clear();
		 		for (int j=0; j<=stringD; j++){
		 			if (netPacket->getNeighbors(j).id == atoi(SELF_NETWORK_ADDRESS)){
		 				isRingMember = true; 
		 				nodePosition = j;
		 				sinkPosition = netPacket->getSinkPosition();
		 				roundNumber = netPacket->getCurrentRound();
		 				kValue = netPacket->getNeighbors(j).kVal;
		 				double timer = uniform(5.0, 7.0);
		 				setTimer(JOIN_RING, timer);
		 			}
		 			ringMembers.push_back(netPacket->getNeighbors(j).id);
		 		}
		 				
		 		/*trace() << "At the current round : " << roundNumber << ", the Node (CH): " << self << " is Cluster Head, he has the ID of other Cluster Head with :  " << ringMembers.size() ;
 				for (int k=0; k<ringMembers.size(); k++)
 					trace() << ringMembers[k];
 				*/	
				trace() << "At the current round : " << roundNumber << ", the Node (CH): " << self << " is in the position :  " <<  nodePosition << " and the sink node has the position :  " <<  sinkPosition << ", And its K_neighbor is : " << kValue ;
 			//setTimer(DISCOVERY_ROUND, roundLength);
			}
 				
			break;
		}
		
		// 
		case JOIN_RING_PACKET: {
			if (!isRingMember && !isSink){
				// Select the CH with the highest rssi or which is close to the CH
				if (abs(rssi) < abs(maxRssiCH)){
					ringFollowing = atoi(netPacket->getSource());
					maxRssiCH = rssi;
				}
				roundNumber = netPacket->getCurrentRound();
				double timer = uniform (10.0, 12.0);
				setTimer(ACK_RING, timer);
			}
			break;
		}		
		
		case ACK_RING_PACKET: {
			if (isRingMember && !isSink && atoi(SELF_NETWORK_ADDRESS) == atoi(netPacket->getDestination()))
				stringMemberFollowers.push_back(atoi(netPacket->getSource()));
			break;
		}
		
		case TDMA_RING_PACKET:{
		//Cominfos()<<SELF_NETWORK_ADDRESS<<" "<<macAddress<<" "<<"LEACH_ROUTING_TDMA_PACKET "<<netPacket ->getSource() ;
			if(!isRingMember && !isSink) {
				followersLength = netPacket->getScheduleArraySize();
				for(int i=0; i<netPacket->getScheduleArraySize(); i++) {
					if (netPacket->getSchedule(i) == atoi(SELF_NETWORK_ADDRESS)) {	
						setStateSleep();
						setTimer(START_SLOT, i*slotLength);
						trace() << "Node " << self << " Received TDMA pkt, I am: "<< i << "th \n";
						break;
					}			
				}
			} else if (isSink) stringNodesInfo.clear();
			break;
		}	
		
		case DATA_RING_PACKET:{
			if (atoi(netPacket->getDestination()) == atoi(SELF_NETWORK_ADDRESS) && !isSink) {
				trace() << "Node :" << self << " Processing Data Packet from initial node : " << netPacket->getSource() ; // A received a data packet but i have to forward it up to the Sink
				initialSender = atoi(netPacket->getIsource());
				frwdCount++;
				setTimer(START_SLOT, 0);
			}	
			
			if (atoi(netPacket->getDestination()) == atoi(SELF_NETWORK_ADDRESS) && isSink ) {
				trace() << "Sink node :" << self << " Processing Data Packet from initial node : " << netPacket->getIsource() << ", and fordwarded by the node : " << netPacket->getSource() << " in hop : " << netPacket->getHopNumber();
			}
			/* string dst(netPacket->getDestination());
			if (isRingMember && dst.compare(SELF_NETWORK_ADDRESS) == 0){
				trace() << "Node " << self << " Aggregate Data Frame ";
				stringBufferAggregate.push_back(*netPacket);	
			}else if (dst.compare(SINK_NETWORK_ADDRESS) == 0 && isSink) {
				trace() << "Node " << self << " Processing Data Packet ";
				toApplicationLayer(decapsulatePacket(netPacket));
			}
			
			if (isSink)
				trace() << "Iam the sink node " << self << " and i received packet from : " << netPacket->getSource() ;
			*/
			break;	
		}
	}
}


void String::timerFiredCallback(int index)
{
	switch (index) {
		
		case HELLO_WORD:{	
			// Sink node send hello word to nodes in order to start the discory round
					
			StringPacket *helloPkt = new StringPacket("Hello Word Packet, I'm the sink node", NETWORK_LAYER_PACKET);
			helloPkt->setByteLength(advPacketSize);
			helloPkt->setStringPacketKind(HELLO_WORD_PACKET);
			helloPkt->setSource(SELF_NETWORK_ADDRESS);
			helloPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(helloPkt, BROADCAST_MAC_ADDRESS);
			
			//setTimer(HELLO_WORD, 2.0);
			if (getTimer(HELLO_WORD) != 0) {
					cancelTimer(HELLO_WORD);
			}	
			break;
		}		
		
		case DISCOVERY_ROUND:{	
			// Send Broadcast packet and try to get it
			trace() << "---- The discovery Start ----";
			stringClusterHeadSet.clear();
			stringMemberFollowers.clear();
			ringMembers.clear();
			StringPacket *brdcstPkt = new StringPacket("Broadcast initialisation Packet", NETWORK_LAYER_PACKET);
			brdcstPkt->setByteLength(advPacketSize);
			brdcstPkt->setStringPacketKind(BROADCAST_PACKET);
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
			break;
		}		

		case SEND_DATA_TO_SINK:{	
			// Send Data to sink based on the neighbor table of each node
			if (getTimer(DISCOVERY_ROUND) != 0) {
					cancelTimer(DISCOVERY_ROUND);
			}
			
			if (!isSink) {
				StringPacket *dataPkt = new StringPacket("Data packet to send up to sink", NETWORK_LAYER_PACKET);
				dataPkt->setNeighborsArraySize(dataPacketSize);
				dataPkt->setByteLength(dataPacketSize);
				dataPkt->setStringPacketKind(TO_SINK_PACKET);
				dataPkt->setSource(SELF_NETWORK_ADDRESS);
				dataPkt->setDestination(SINK_NETWORK_ADDRESS);
				dataPkt->setSinkRssi(sinkRssi);
				dataPkt->setResidual(resMgrModule->getRemainingEnergy()); // Residual energy
					
				// Construction of the neighbor table
				int i = 0;
				for (list <StringNeighborInfo>::iterator it = stringNeighborsTable.begin(); it != stringNeighborsTable.end(); it++){
						stringNeighbor Neghborpkt;
						Neghborpkt.id = (*it).id;
						Neghborpkt.rssi = (*it).rssi;
						Neghborpkt.residual = (*it).residual;
						Neghborpkt.location = (*it).location;
						dataPkt->setNeighbors(i, Neghborpkt);
						i++;
				}
				
				// Send data packet to the SINK NODE
				toMacLayer(dataPkt, atoi(SINK_NETWORK_ADDRESS));
				stringNeighborsTable.clear();
				trace() << "Node " << self << " send data to sink " ;
			}
			if (isSink) {
				trace() << "Sink node : " << self << " has : "<< stringNeighborsTable.size() << " Neighbors " ;
				for (list <StringNeighborInfo>::iterator it = stringNeighborsTable.begin(); it != stringNeighborsTable.end(); it++)
					trace() << "Sink neighbor : " << (*it).id ;
			}
			
			//setTimer(DISCOVERY_ROUND, 300.0);	
			if (getTimer(SEND_DATA_TO_SINK) != 0) {
					cancelTimer(SEND_DATA_TO_SINK);
			}
			break;
		}
		
		case ABC_COMPUTATION:{
			/*
			stringCHcandidates.clear();	
			*/
			stringClusterHeadSet.clear();
			stringCHcandidates.clear();
			stringNodesInfo.sort(stringCmpId); // Classer les noeuds en fonction de leur id
			stringCHcandidates = stringNodesInfo;
			stringCHcandidates.remove_if (stringLessThanThresh);
			list <stringNodeInfo> checkNeighborNumber;
			checkNeighborNumber = stringCHcandidates;
			checkNeighborNumber.sort(stringCmpNeighborNumb);
			int k = checkNeighborNumber.front().NeighborsTable.size();
			trace() << "The K is : " << k ;
			trace() << "Candidates set has : " << stringCHcandidates.size() ;
			//stringD = stringCHcandidates.size(); 
			for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++)
				trace() << "candida " << (*it).id << " residual energy : " << (*it).residual ;
		
			stringLb = 0; 	// The candidate set is sort thus the lowest Id is the first element
			stringUb = stringCHcandidates.size() - 1;		// The higher element is the last of the vector list
			srand((unsigned)time(NULL));
			// ABC start
			stringInitilize();
			StringMemorizeBestSource();
			int gen = 0;
			while (gen<stringMaxCycle){
				EmployedBees();
				stringCalculateProbabilities();
				OnlookerBees();		
				StringMemorizeBestSource();
				ScoutBees();
				StringMemorizeBestSource();
				trace() << "Iteration : " << gen << ", BestSource.trueFit = " << StringBestSource.trueFit ;
				trace() << "Solutions : ";
				for (int i=0 ; i<stringD; i++)
					trace() << StringBestSource.code[i];
				gen++;
			}
			// Strucuture of the Sink node 
			stringNodeInfo sinkNode; 
			sinkNode.id = 0;
			sinkNode.residual = resMgrModule->getRemainingEnergy();
			sinkNode.location = 0.0;
			sinkNode.NeighborsTable = stringNeighborsTable;
			
			stringNeighborsTable.sort(stringCmpRssi); // Sort the neighbor table of the Sink so that the first element is the closest node
			stringNeighborsTable.reverse();
			int id, position; 
			int nodeRssi = MIN_RSSI;
		
			for (list <StringNeighborInfo>::iterator ngh = stringNeighborsTable.begin(); ngh != stringNeighborsTable.end(); ngh++){
				for (int j=0 ; j<stringD; j++) {
					if (StringBestSource.code[j] == (*ngh).id && (*ngh).rssi > nodeRssi){
						position = j; // position + 1 is the ID of the Sink node within the clusterHeadSet  
						id = StringBestSource.code[j];
						nodeRssi = (*ngh).rssi;
					}
				}
			}
			
			//  Creation of the Cluster head set		
			for (int j=0 ; j<stringD; j++) {
				for (list <stringNodeInfo>::iterator ch = stringCHcandidates.begin(); ch != stringCHcandidates.end(); ch++){
					if ((*ch).id == StringBestSource.code[j] && id != (*ch).id)
						stringClusterHeadSet.push_back(*ch);
					else if ((*ch).id == StringBestSource.code[j] && id == (*ch).id){
						stringClusterHeadSet.push_back(*ch);
						stringClusterHeadSet.push_back(sinkNode);
					}
				}
			}
			
			trace() << "Round : " << roundNumber <<  ", The cluster Head set has " << stringClusterHeadSet.size() << " nodes : ";
			for (list <stringNodeInfo>::iterator it = stringClusterHeadSet.begin(); it != stringClusterHeadSet.end(); it++)
					trace() << (*it).id ;	
			
			// Computation of the value of k 
			/*
			kValue = k;
			list <stringNodeInfo>::iterator it2; 
			for (list <stringNodeInfo>::iterator it = stringClusterHeadSet.begin(); it != stringClusterHeadSet.end(); it++) {
				if ((*it).id != 0) {
					int kInt = 0;
					int id = 1;
					list <stringNodeInfo>::iterator itera = it;
					for (int i = 1; i <= stringClusterHeadSet.size(); i++) {
						if (id == 0) 
							it2 == stringClusterHeadSet.begin();
						else
							it2 = next(itera, id); 
						if (it2 != stringClusterHeadSet.end() && (*it2).id != (*it).id) {
							if (stringIsNeighbor((*it), (*it2)))
								kInt++;	
							id ++;
						}else if (it2 == stringClusterHeadSet.end()) {
							id = 0;
							itera = stringClusterHeadSet.begin();
							it2 = stringClusterHeadSet.begin();
						}
					}
					if (kInt < kValue) {
						kValue = kInt;
					}
				}
			} 
			*/
			kValue = int(stringClusterHeadSet.size()/6);
			trace() << "The value of k neighbor is : "<< kValue ;
					
			StringPacket *steadyPkt = new StringPacket("End of the setup phase", NETWORK_LAYER_PACKET);
			steadyPkt->setNeighborsArraySize(dataPacketSize);
			steadyPkt->setByteLength(dataPacketSize);
			steadyPkt->setStringPacketKind(END_SETUP_PACKET);
			steadyPkt->setCurrentRound(roundNumber);
			steadyPkt->setSinkPosition(position + 1);
			//steadyPkt->setKValue(kValue);
			steadyPkt->setSource(SELF_NETWORK_ADDRESS);
			steadyPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				
			int in = 0;
			for (list <stringNodeInfo>::iterator it = stringClusterHeadSet.begin(); it != stringClusterHeadSet.end(); it++){
				//trace() << "Cluster Head " << (*it).id << " has (residual,location) : (" << (*it).residual << "," << (*it).location << ")";
				stringNeighbor Neghborpkt;
				Neghborpkt.id = (*it).id;
				Neghborpkt.residual = (*it).residual;
				Neghborpkt.kVal = getKValue(in, steadyPkt->getSinkPosition());
				Neghborpkt.location = (*it).location;
				steadyPkt->setNeighbors(in, Neghborpkt);
				in++;
			}
			trace() << "Round : " << roundNumber <<  ", packet to send has- :" ;
			
			for  (int j = 0; j<=stringD; j++){
				trace() << "Node : " << steadyPkt->getNeighbors(j).id ;
			}
			
			toMacLayer(steadyPkt, BROADCAST_MAC_ADDRESS);
			stringNeighborsTable.clear();
			stringNodesInfo.clear();
			
			if (getTimer(ABC_COMPUTATION) != 0) {
					cancelTimer(ABC_COMPUTATION);
			}
			roundNumber++;
			setStateRx();
			break;
		}
		
		case JOIN_RING:{
			if (isRingMember && !isSink)	{
				StringPacket *joinPkt = new StringPacket("Eeach Ring Member sends joined packet", NETWORK_LAYER_PACKET);
				joinPkt->setByteLength(advPacketSize);
				joinPkt->setStringPacketKind(JOIN_RING_PACKET);
				joinPkt->setSource(SELF_NETWORK_ADDRESS);
				joinPkt->setCurrentRound(roundNumber);
				joinPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				toMacLayer(joinPkt, BROADCAST_MAC_ADDRESS);
				trace() << "The cluster Head with ID : " << self << " Send his joined CH ";
				
				double timer = uniform (15.0, 20.0);
				setTimer(TDMA_RING, timer);	
				
				if (getTimer(JOIN_RING) != 0) {
						cancelTimer(JOIN_RING);
				}				
			}
			break;
		}	
		
		case ACK_RING:{
			if (!isRingMember && !isSink)	{
				StringPacket *ackPkt = new StringPacket("Each Ring Member sends ack packet", NETWORK_LAYER_PACKET);
				ackPkt->setByteLength(advPacketSize);
				ackPkt->setStringPacketKind(ACK_RING_PACKET);
				ackPkt->setSource(SELF_NETWORK_ADDRESS);
				ackPkt->setCurrentRound(roundNumber);
				string destination = to_string(ringFollowing);
				ackPkt->setDestination(destination.c_str());
				toMacLayer(ackPkt, ringFollowing);
				trace() << "Round : " << roundNumber <<  ", node :" << self << " sends acknowledgement to ringFollowing : " << ringFollowing;
				
				if (getTimer(ACK_RING) != 0) {
						cancelTimer(ACK_RING);
				}
			}
			break;
		}
		
		case TDMA_RING:{
			frwdCount = 0;	
			if (stringMemberFollowers.size()!=0){
				StringPacket *crtlPkt = new StringPacket("Ring member builds TDMA Packet", NETWORK_LAYER_PACKET);
				crtlPkt->setByteLength(tdmaPacketSize);
				crtlPkt->setStringPacketKind(TDMA_RING_PACKET);
				crtlPkt->setSource(SELF_NETWORK_ADDRESS);
				crtlPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				followersLength = stringMemberFollowers.size();	
				crtlPkt->setScheduleArraySize(stringMemberFollowers.size());	
				for(int i=0; i<followersLength; i++) 
					crtlPkt->setSchedule(i,stringMemberFollowers[i]);
				
				toMacLayer(crtlPkt, BROADCAST_MAC_ADDRESS);
				trace() << "Node " << self << " Sent TDMA pkt";
				setTimer(START_SLOT, followersLength*slotLength);
			}
			else setTimer(START_SLOT, slotLength);					
			break;
		}
		
		case START_SLOT:{
			if (isRingMember && !isSink){
				int hopNumber = 0; //represents the number of node need to reach the Sink
				int ringLenght = ringMembers.size();
				StringPacket *dataPkt = new StringPacket("Ring member builds TDMA Packet", NETWORK_LAYER_PACKET);
				dataPkt->setByteLength(tdmaPacketSize);
				dataPkt->setStringPacketKind(DATA_RING_PACKET);
				if (frwdCount == 0)	{
					dataPkt->setSource(SELF_NETWORK_ADDRESS);
					dataPkt->setIsource(SELF_NETWORK_ADDRESS);
				}
				else {
					dataPkt->setSource(SELF_NETWORK_ADDRESS);
					string src = to_string(initialSender);
					dataPkt->setIsource(src.c_str());					
				}
				
				if (sinkPosition > nodePosition) { // The sink is located after the node position according the ringMembers Vector
					hopNumber = sinkPosition - nodePosition ;
					if (hopNumber <= kValue){ // I can send my data directly to the node
						dataPkt->setDestination(SINK_NETWORK_ADDRESS);
						dataPkt->setHopNumber(frwdCount+1);
						toMacLayer(dataPkt, atoi(SINK_NETWORK_ADDRESS));
						trace() << "Send data, the node : " << self << " (1) which is in the position : " << nodePosition << " sends Data to : " << atoi(SINK_NETWORK_ADDRESS) << ", the Initial sender is node :" << dataPkt->getIsource();
						break;
					}else { // The node is not able to send data directly to the Sink 
				 		string destination = to_string(ringMembers[nodePosition+kValue]); // Send to the kth neighbor of the ring 
				 		dataPkt->setDestination(destination.c_str());
				 		dataPkt->setHopNumber(frwdCount+1);
						toMacLayer(dataPkt, ringMembers[nodePosition+kValue]);
						trace() << "Send data, the node : " << self << " (11)  in the position : " << nodePosition << " sends Data to : " << ringMembers[nodePosition+kValue] << ", the Initial sender is node :" << dataPkt->getIsource();
						break;
					}	
				}else { // The sink is located before the node position according the ringMembers Vector
					hopNumber = ringLenght-nodePosition+sinkPosition;// The hopNUmber from the node to the Sink
					if (hopNumber <= kValue){ // I can send my data directly to the node
						dataPkt->setDestination(SINK_NETWORK_ADDRESS);
						dataPkt->setHopNumber(frwdCount+1);
						toMacLayer(dataPkt, atoi(SINK_NETWORK_ADDRESS));
						trace() << "Send data, the node : " << self << " (2) which is in the position : " << nodePosition << " sends Data to : " << atoi(SINK_NETWORK_ADDRESS) << ", the Initial sender is node :" << dataPkt->getIsource();
						break;
					}else { // The node is not able to send data directly to the Sink 
						string destination = to_string(ringMembers[kValue+nodePosition-ringLenght]); // Send to the kth neighbor of the ring 
				 		dataPkt->setDestination(destination.c_str());
				 		dataPkt->setHopNumber(frwdCount+1);
				 		int receiverId;
				 		if (kValue+nodePosition < ringLenght) receiverId = kValue+nodePosition;
				 		else receiverId = kValue+nodePosition-ringLenght;
						toMacLayer(dataPkt, ringMembers[receiverId]);
						trace() << "Send data, the node : " << self << " (21) in the position : " << nodePosition << " sends Data to : " << ringMembers[receiverId] << ", the Initial sender is node :" << dataPkt->getIsource();
						break;
					}
				}
			}
		 	if (!isRingMember && ringFollowing != MAX_IMAGINARY_NODE && !isSink) {
		 		StringPacket *fwrdPkt = new StringPacket("Ring member builds TDMA Packet", NETWORK_LAYER_PACKET);
				fwrdPkt->setByteLength(tdmaPacketSize);
				fwrdPkt->setStringPacketKind(DATA_RING_PACKET);
				fwrdPkt->setSource(SELF_NETWORK_ADDRESS);
				string destination = to_string(ringFollowing);
				fwrdPkt->setDestination(destination.c_str());
				trace() << "The node :" << self << " located at the position : " << nodePosition << " sends forwarded Data to : " << ringFollowing;
				toMacLayer(fwrdPkt, ringFollowing) ;			
				setTimer(END_SLOT, slotLength);		
		 	}
			break;			
		}
		
		case END_SLOT:{
			stringMemberFollowers.clear();
			ringMembers.clear();
			trace() << "Node " << self << " Sleept"  << "\n";
			if (!isSink && !isRingMember) 
				setStateSleep();
			break;
		}
		
	}
}

void String::Aggregate() {
	if(stringBufferAggregate.size()!=0)	{	
		double bitsLength = stringBufferAggregate.size() * dataPacketSize * 1000;
		double energyBit = (aggrConsumption / pow(10,9)) * bitsLength;
		powerDrawn(energyBit);
		StringPacket *aggrPacket = new StringPacket("ClusterHead Aggredated Packet", NETWORK_LAYER_PACKET);
		aggrPacket->setByteLength(dataPacketSize+4);
		aggrPacket->setStringPacketKind(DATA_RING_PACKET);
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
		stringBufferAggregate.clear();
	}
}

void String::processBufferedPacket() {
	while (!stringTempTXBuffer.empty())	{
		cPacket *pkt = stringTempTXBuffer.front();	
		StringPacket *netPacket = dynamic_cast <StringPacket*>(pkt);
		stringstream buffer;
		buffer << ringFollowing;
		string dst = buffer.str();
		netPacket->setDestination(dst.c_str());
		bufferPacket(netPacket); 
		stringTempTXBuffer.pop();
	}

	while (!TXBuffer.empty()) {
		toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
		TXBuffer.pop();
	}	
}

void String::setStateRx() {
	send(createRadioCommand(SET_STATE, RX), "toMacModule");	
}

void String::setPowerLevel(double powerLevel) {
	send(createRadioCommand(SET_TX_OUTPUT, powerLevel), "toMacModule");
}

void String::setStateSleep() {
	send(createRadioCommand(SET_STATE, SLEEP), "toMacModule");
}

void String::levelTxPower(int linkBudget) {
	vector<int>::iterator constIterator;
	for (constIterator = stringPowers.begin();
		constIterator != stringPowers.end();
		constIterator++ ) {
		if(*constIterator > (linkBudget))	{
			setPowerLevel(*constIterator);
			break;
		}
	}
}

void String::readXMLparams() {
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
		stringPowers.push_back(atoi(sources[s]->getNodeValue()));
	if (stringPowers.size() < 1) endSimulation();
	sort(stringPowers.begin(), stringPowers.end());
}

bool stringCmpResidual(stringNodeInfo a, stringNodeInfo b){
	return (a.residual > b.residual);
}

bool stringCmpId(stringNodeInfo a, stringNodeInfo b){
	return (a.id < b.id);
}

bool stringCmpNeighborNumb(stringNodeInfo a, stringNodeInfo b){
	return (a.NeighborsTable.size() < b.NeighborsTable.size());
}

bool stringLessThanThresh(stringNodeInfo a){
	return (a.residual < THRESHOLD_ENERGY);
}

bool stringCmpRssi(StringNeighborInfo a, StringNeighborInfo b){
	return (a.rssi > b.rssi);
}

// ABC functions implementation 
double stringRandom(double start, double end){
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

void stringInitilize() {
	int i, j, temp;
	for (i = 0;i<stringFoodNumber;i++){
		for (j = 0;j<stringD;j++){
			bool exist = true;
			while (exist){
			//initialized food matrix FoodNumber*D and fulled with random values
				int id = randint(stringLb, stringUb);
				list <stringNodeInfo>::iterator it = stringCHcandidates.begin();
				advance(it, id);
				temp = 	(*it).id;		
				exist = stringAlreadyExist(StringNectarSource[i].code, temp, stringD);
			}
			StringNectarSource[i].code[j] = temp ; //random(lb, ub);
			
			StringEmployedBee[i].code[j] = StringNectarSource[i].code[j];
			StringOnLooker[i].code[j] = StringNectarSource[i].code[j];
			StringBestSource.code[j] = StringNectarSource[0].code[j];
		}
			
		//initialized the food
		StringNectarSource[i].trueFit = stringCalculationTruefit(StringNectarSource[i]);
		StringNectarSource[i].fitness = stringCalculationFitness(StringNectarSource[i].trueFit);
		StringNectarSource[i].rfitness = 0;
		StringNectarSource[i].trail = 0;
		//initialized the employed
		StringEmployedBee[i].trueFit = StringNectarSource[i].trueFit;
		StringEmployedBee[i].fitness = StringNectarSource[i].fitness;
		StringEmployedBee[i].rfitness = StringNectarSource[i].rfitness;
		StringEmployedBee[i].trail = StringNectarSource[i].trail;
		//initialized the onlooker
		StringOnLooker[i].trueFit = StringNectarSource[i].trueFit;
		StringOnLooker[i].fitness = StringNectarSource[i].fitness;
		StringOnLooker[i].rfitness = StringNectarSource[i].rfitness;
		StringOnLooker[i].trail = StringNectarSource[i].trail;
	}
	
	//initialized the best food
	StringBestSource.trueFit = StringNectarSource[0].trueFit;
	StringBestSource.fitness = StringNectarSource[0].fitness;
	StringBestSource.rfitness = StringNectarSource[0].rfitness;
	StringBestSource.trail = StringNectarSource[0].trail;
}

double stringCalculationTruefit(StringBeeGroup bee)//calculate the value of the function
{
	double fitness = 0;
	stringNodeInfo node;
	stringNodeInfo next;
	for (int j=0; j<stringD ; j++) {
		for (list <stringNodeInfo>::iterator ch = stringCHcandidates.begin(); ch != stringCHcandidates.end(); ch++) {
			if ((*ch).id == bee.code[j]) {
				node = (*ch) ;
			}else if ((*ch).id == bee.code[j+1] && j != stringD-1) {
				next = (*ch); 
			}else if (j == stringD-1 && (*ch).id == bee.code[0]) {
				next = (*ch);
			} 
  	} 
  	 	
  	for (list <StringNeighborInfo>::iterator it = node.NeighborsTable.begin(); it != node.NeighborsTable.end(); it++) {
  		if ((*it).id == next.id)
  			fitness += abs((*it).rssi) ; // Som = Abs(RSSI_next)/Residual of the node of the ring
  	}
  }
    
	return fitness ;
}


// Computation of the fitness value
double stringCalculationFitness(double truefit)
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
void EmployedBees() {
	int i, j, k, temp, id;
	bool candidate, path;
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	for (i = 0;i<stringFoodNumber;i++){
		//The parameter to be changed is determined randomly
		candidate = false;
		path = false;
		for (j = 0;j<stringD;j++){
			StringEmployedBee[i].code[j] = StringNectarSource[i].code[j];
		}		
		while (candidate == false || path == false) {
			param2change = randint(0, stringD-1);
			//A randomly chosen solution is used in producing a mutant solution of the solution i
			
			while (1){
				k = randint(0, stringFoodNumber-1);
				if (k != i){
					break;
				}
			}
			
			//v_{ij}=x_{ij}+\phi_{ij}*(x_{ij}-x_{kj})
			Rij = stringRandom(-1, 1);
			temp = StringNectarSource[i].code[param2change] + Rij*(StringNectarSource[i].code[param2change] - StringNectarSource[k].code[param2change]);
			
			if (temp>stringCHcandidates.back().id){
				temp = stringCHcandidates.back().id;
			}
			if (temp<stringCHcandidates.front().id){
				temp = stringCHcandidates.front().id;
			}
			
			candidate = stringExistCandidateSet(temp);
			path = stringExistPath(StringEmployedBee[i].code, temp, param2change, stringD); // Check if there is a path between added nodes and its neighbor (previous and next)
		}
		
		
		for (int j=0 ; j<stringD; j++) {
			if (StringEmployedBee[i].code[j] == temp) {
				id = j;
				break;
			}
		}
		StringEmployedBee[i].code[id] = StringEmployedBee[i].code[param2change] ;
		StringEmployedBee[i].code[param2change] = temp;
		
		//evaluate new solution
		StringEmployedBee[i].trueFit = stringCalculationTruefit(StringEmployedBee[i]);
		StringEmployedBee[i].fitness = stringCalculationFitness(StringEmployedBee[i].trueFit);
		//a greedy selection is applied between the current solution i and its mutant
		//If the mutant solution is better than the current solution i,
		//replace the solution with the mutant and reset the trial counter of solution i
		if (StringEmployedBee[i].trueFit<StringNectarSource[i].trueFit){
			for (j = 0;j<stringD;j++){
				StringNectarSource[i].code[j] = StringEmployedBee[i].code[j];
			}
			StringNectarSource[i].trail = 0; // The employed bee is still active
			StringNectarSource[i].trueFit = StringEmployedBee[i].trueFit;
			StringNectarSource[i].fitness = StringEmployedBee[i].fitness;
		}
		else{
			//if the solution i can not be improved, increase its trial counter
			StringNectarSource[i].trail++;
		}
	}
}

/***********************************************************************************/
//  Calculate Probabilities
//  A food source is chosen with the probability which is proportioal to its quality
/***********************************************************************************/

void stringCalculateProbabilities() {
	int i;
	double somFit = 0;
	
	for (i = 0; i<stringFoodNumber;i++) {
		somFit += StringNectarSource[i].fitness;
	}
	
	for (i = 0; i<stringFoodNumber;i++) {
		StringNectarSource[i].rfitness = (StringNectarSource[i].fitness)/somFit;
	}
}

/***********************************************************************************/
//         ONLOOKER BEE PHASE
/***********************************************************************************/

void OnlookerBees() {
	int i, j, t, k, temp, id;
	bool candidate, path;
	double R_choosed;//the rate to be choosed
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	i = 0;
	t = 0;
	while (t<stringFoodNumber) {
		R_choosed = stringRandom(0, 1);
		//A food source is chosen with the probability which is proportioal to its quality
		if (R_choosed<StringNectarSource[i].rfitness) {
			t++;
			candidate = false;
			path = false;
			for (j = 0;j<stringD;j++){
				StringOnLooker[i].code[j] = StringNectarSource[i].code[j];
			}
			while (candidate == false || path == false) {
				//The parameter to be changed is determined randomly
				param2change = randint(0, stringD-1);
				//A randomly chosen solution is used in producing a mutant solution of the solution i
				//Randomly selected solution must be different from the solution i
				while (1) {
					k = randint(0, stringFoodNumber);
					if (k != i) {
						break;
					}
				}

				//sol(Param2Change)=Foods(i,Param2Change)+(Foods(i,Param2Change)-Foods(neighbour,Param2Change))*(rand-0.5)*2;
				Rij = stringRandom(-1, 1);
				temp = StringNectarSource[i].code[param2change] + Rij*(StringNectarSource[i].code[param2change] - StringNectarSource[k].code[param2change]);

				//shifted onto the boundaries if generated parameter value is out of boundaries
				if (temp<stringCHcandidates.front().id){
					temp = stringCHcandidates.front().id;
				}
				if (temp>stringCHcandidates.back().id){
					temp = stringCHcandidates.back().id;
				}
				
				candidate = stringExistCandidateSet(temp);
				path = stringExistPath(StringOnLooker[i].code, temp, param2change, stringD); 
			}
			
			for (int j=0 ; j<stringD; j++) {
				if (StringOnLooker[i].code[j] == temp) {
					id = j;
					break;
				}
			}
			
			StringOnLooker[i].code[id] = StringOnLooker[i].code[param2change] ; // Permute the value of the onlooker bees
			StringOnLooker[i].code[param2change] = temp;
						
			StringOnLooker[i].trueFit = stringCalculationTruefit(StringOnLooker[i]);
			StringOnLooker[i].fitness = stringCalculationFitness(StringOnLooker[i].trueFit);

			//a greedy selection is applied between the current solution i and its mutant
			//If the mutant solution is better than the current solution i,
			//replace the solution with the mutant and reset the trial counter of solution i
			if (StringOnLooker[i].trueFit<StringNectarSource[i].trueFit){
				for (j = 0;j<stringD;j++){
					StringNectarSource[i].code[j] = StringOnLooker[i].code[j];
				}
				StringNectarSource[i].trail = 0;
				StringNectarSource[i].trueFit = StringOnLooker[i].trueFit;
				StringNectarSource[i].fitness = StringOnLooker[i].fitness;
			}
			else{
				StringNectarSource[i].trail++;
			}
		}
		i++;
		if (i == stringFoodNumber){
			i = 0;
		}
	}
}

/*****************************************************************************/
//SCOUT BEE PHASE
//determine the food sources whose trial counter exceeds the "limit" value
//only one scout is allowed to occur in each cycle
/*****************************************************************************/

void ScoutBees() {
	int maxtrialindex, i, j, temp, id;
	double R;//random number between [0,1]
	bool candidate, path;
	maxtrialindex = 0;
	for (i = 0;i<stringFoodNumber;i++){
		if (StringNectarSource[i].trail>StringNectarSource[maxtrialindex].trail){
			maxtrialindex = i;
		}
	}

	//determine the food sources whose trial counter exceeds the "limit" value
	if (StringNectarSource[maxtrialindex].trail >= stringLimit){
		//reinitailized food
		for (j = 0;j<stringD;j++){
			candidate = false;
			path = false;
			while (candidate == false || path == false) {
				R = stringRandom(0, 1);
				temp = int(stringLb + R*(stringCHcandidates.back().id - stringCHcandidates.front().id));
				
				candidate = stringExistCandidateSet(temp);
				path = stringExistPath(StringNectarSource[maxtrialindex].code, temp, j, stringD);
			}
			
			for (int k=0 ; k<stringD; k++) {
				if (StringNectarSource[maxtrialindex].code[k] == temp) {
					id = k;
					break;
				}
			}
			
			StringNectarSource[maxtrialindex].code[id] = StringNectarSource[maxtrialindex].code[j] ; // Permute values
			StringNectarSource[maxtrialindex].code[j] = temp;
			
			//StringNectarSource[maxtrialindex].code[j] = temp;
		}
		StringNectarSource[maxtrialindex].trail = 0;
		StringNectarSource[maxtrialindex].trueFit = stringCalculationTruefit(StringNectarSource[maxtrialindex]);
		StringNectarSource[maxtrialindex].fitness = stringCalculationFitness(StringNectarSource[maxtrialindex].trueFit);
	}
}

void StringMemorizeBestSource() { // The best food source is memorized
	int i, j;
	for (i = 0;i<stringFoodNumber;i++){
		if (StringNectarSource[i].trueFit < StringBestSource.trueFit){
			for (j = 0;j<stringD;j++){			
				StringBestSource.code[j] = StringNectarSource[i].code[j];
			}
			StringBestSource.trueFit = StringNectarSource[i].trueFit;
		}
	}
}

bool stringExistCandidateSet(int valeurATrouver) {
	for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++) {
		if ((*it).id == valeurATrouver) {
	  	return true;
		}
	}
	return false;
}

bool stringAlreadyExist(int T[],int valeurATrouver, int lenght) {
	for (int i=0;i<lenght;i++) {
		if (T[i]==valeurATrouver) {
	  	return true;
		}
	}	
	return false;
}

bool stringExistPath(int T[], int valeurATrouver, int id, int lenght) {
	stringNodeInfo prev ;
	stringNodeInfo node ;
	stringNodeInfo next ;
	bool pathPrev = false ;
	bool pathNext = false ;
	
	for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++) {
		if ((*it).id == valeurATrouver) {
			node = (*it) ;
			break ;
		}
	}
	
	if (id == 0) {
		for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++) {
			if ((*it).id == T[1]) {
				next = (*it) ;
			}else if ((*it).id == T[lenght-1]) {
				prev = (*it) ;
			}
		}
	}else if (id == lenght-1){
		for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++) {
			if ((*it).id == T[lenght-2]) {
				prev = (*it) ;
			}else if ((*it).id == T[0]) {
				next = (*it) ;
			}
		}
	}else {
		for (list <stringNodeInfo>::iterator it = stringCHcandidates.begin(); it != stringCHcandidates.end(); it++) {
			if ((*it).id == T[id-1]) {
				prev = (*it) ;
			}else if ((*it).id == T[id+1]) {
				next = (*it) ;
			}
		}
	}
	
	for (list <StringNeighborInfo>::iterator it = node.NeighborsTable.begin(); it != node.NeighborsTable.end(); it++) {
			if ((*it).id == next.id)
				pathNext = true;
		}		
		for (list <StringNeighborInfo>::iterator it = prev.NeighborsTable.begin(); it != prev.NeighborsTable.end(); it++) {
			if ((*it).id == node.id)
				pathPrev = true;
		}
		
		if (pathNext && pathPrev)
			return true;
		else 
			return false;			
}

bool stringIsNeighbor(stringNodeInfo node, stringNodeInfo next) {
	for (list <StringNeighborInfo>::iterator it = next.NeighborsTable.begin(); it != next.NeighborsTable.end(); it++) {
		if ((*it).id == node.id)
			return true;
	}
	return false;
}

int getKValue(int nodePosition, int sinkPosition){
	int hopNumber = 0;
	bool isNeighbor;
	//list<stringNodeInfo>::iterator it = stringClusterHeadSet.begin();
	stringNodeInfo node = *next(stringClusterHeadSet.begin(),nodePosition) ;
	for (int j=1; j<stringClusterHeadSet.size(); j++) {
  	if (sinkPosition-j >= 0) isNeighbor = stringIsNeighbor(node, *next(stringClusterHeadSet.begin(),sinkPosition-j));
		else isNeighbor = stringIsNeighbor(node, *prev(stringClusterHeadSet.end(),j-sinkPosition));
		if (sinkPosition-j > nodePosition && isNeighbor) {
			return sinkPosition-j-nodePosition;
		}else if (sinkPosition-j == 0 && isNeighbor) {
			return stringClusterHeadSet.size()-nodePosition;  
		}else if (sinkPosition-j < 0 && nodePosition > stringClusterHeadSet.size()+sinkPosition-j) {
			return 2*stringClusterHeadSet.size()-nodePosition+sinkPosition-j ;// sinkPosition-j < nodePosition
		}else if (sinkPosition-j < 0 && nodePosition < stringClusterHeadSet.size()+sinkPosition-j) {
			return stringClusterHeadSet.size()+sinkPosition-j-nodePosition; 
		}
	}
}
