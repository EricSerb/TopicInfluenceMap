#include "graph.hpp"

#define DEBUG
#define COUNT


/******************************
 * Graph functions 
 ******************************/


void Graph::init(string file) 
{
	ifstream ifile(file);
	
	string line;
	vector<string> tokens;
	
	do 
	{
		getline(ifile, line);
		tokens = GenParse(line);
	}while(tokens[ID].compare("*Vertices"));
	

	int numVerts = std::stoi(tokens[1]);

	for (int i = 0; i < numVerts; i++)
	{
		//Insert all of the node info in this loop
		getline(ifile, line);
		tokens.clear();
		tokens = VertParse(line);

		try
		{
			shared_ptr<Node> node = shared_ptr<Node>(new Node(std::stoi(tokens[ID]), tokens[NAME_ID2], std::stoi(tokens[WEIGHT])));
			int nid = node->getId();
			if(node->getWeight() <= 0)
			{
				node->correctWeight();
			}
// 			this->total += node->getWeight();
			this->_graph.insert(make_pair(nid, node));
			node.reset();
			
#ifdef COUNT
			if((nid%10000) == 0)
				cout << "Finished reading in " << nid << "/" << numVerts << " nodes."<< endl;
#endif
		}
		
		catch (int e)
		{
			cout << "An exception occured: EX NUM: " << e << endl;
			cout << "Node info: ID= " << tokens[ID] << " name= " << tokens[NAME_ID2] 
			<< " weight= " << tokens[WEIGHT] << endl;
			cout << "Skipping node: " << tokens[ID] << " already exists..." << endl;
		}
	}
	
#ifdef COUNT
	cout << "A total of " << numVerts << " nodes were read in." << endl;
#endif

	do 
	{
		getline(ifile, line);
		tokens = GenParse(line);
	}while(tokens.empty() || tokens[ID].compare("*Edges"));
	
	
	
	//Parsing edges
#ifdef COUNT
	unsigned int ecount = 0;
#endif
	while (getline(ifile, line))
	{
		tokens.clear();
		tokens = GenParse(line);

		//Making edges undirected since alg needs them to be
		
		shared_ptr<Node> n = this->_graph.at(std::stoi(tokens[ID]));
		shared_ptr<Node> n2 = this->_graph.at(std::stoi(tokens[NAME_ID2]));
		
		n->addEdge(std::stoi(tokens[ID]), std::stoi(tokens[NAME_ID2]), std::stoi(tokens[WEIGHT]));
		n2->addEdge(std::stoi(tokens[NAME_ID2]), std::stoi(tokens[ID]), std::stoi(tokens[WEIGHT]));
		
#ifdef DEBUG
// 		cout << "Node: " << n->getId() << endl;
// 		n->printEdges();
// 		cout << "Node: " << n2->getId() << endl;
// 		n2->printEdges();
#endif
		
		n.reset();
		n2.reset();

#ifdef COUNT
		++ecount;
		
		if((ecount%10000) == 0)
			cout << ecount << " edges have been added..." << endl;
#endif
	}
	
#ifdef COUNT
	cout << "A total of " << ecount << " edges were read in." << endl;
#endif

	ifile.close();
	
// 	normalizeWeights();
	
	//Find all leaf nodes to start sum prod alg
	
	findLeafs();
}


vector<string> Graph::GenParse(string line) 
{
	vector<string> tokens;
	
	stringstream ss = stringstream(line);
	string temp;
	
	while (ss >> temp) 
	{
		tokens.push_back(temp);
	}
	
	return tokens;
}


vector<string> Graph::VertParse(string line) 
{
	string delim = " ";
	vector<string> tokens;
	
	int id_pos =  line.find_first_of(" \t");
	string sub = line.substr(0, id_pos);
	tokens.push_back(sub.data());
	line.erase(0, id_pos + delim.length());
	
	id_pos = line.rfind("\"") + 1;
	sub.assign(line.substr(0, id_pos));
	tokens.push_back(sub.data());
	//Can change to 1 subtract 1 from delim.length() to remove quotes
	line.erase(0, id_pos + delim.length());
	
	stringstream ss = stringstream(line);
	string temp;
	
	while (ss >> temp) 
	{
		tokens.push_back(temp);
	}
	
	return tokens;
}


void Graph::findLeafs()
{
	for(auto it : this->_graph)
	{
		if(it.second->getEdgeSize() == 1)
			this->leafs.push_back(it.second->getId());
	}
}


unordered_map<int, shared_ptr<Node>>::iterator Graph::graphBegin()
{
	return this->_graph.begin();
}


shared_ptr<Node> Graph::getNode(int index)
{
	return this->_graph.at(index);
}


bool Graph::checkConverge()
{
	/*
	 * Create a loop to check if converged by product nodes new messages
	 * being the same as product of previous iteration messages
	 */
	bool check = true;
	
	for(auto it : this->_graph)
	{
		//check if rcv'd messages on all edges and sent msgs on all edges
		double p = it.second->calcG();
#ifdef DEBUG
		cout <<"Node " << it.first << " Old p: " << it.second->getOldProd() << " New p: " << p << endl;
#endif
		if(p != it.second->getOldProd())
		{
			check = false;
			it.second->setOldProd(p);
		}
	}
	return check;;
}


void Node::prepMessage()
{
	//check to see if node has received all messages
	/*Prod of all messages at nodes
	//Sum of all messages times edge weight at factor nodes
	//Since all factor nodes only receive and send between 2 nodes and 
	//all edge weight is 1 then factor node just mult message from node by 1 and send it on
	* So this implementation does not use factor nodes
	*
	* Using flooding method to propogate messages since graph contains cycles
	*/
	
	for(auto it : *(this->_edges))
	{
		//for each edge, build a message to send to it
		double prod = 1;
		for(auto msgs : this->rcv_msg)
		{
			if(msgs.first != it.second.end_node)
				prod *= msgs.second;
			else
				prod *= it.second.g;
		}
		//msgs normalized so sum of all 
		this->to_send.insert(make_pair(it.second.end_node, prod));
	}
	
// 	//normalize msgs to sum to 1
// 	double tot = getMsgTotal();
// 	for(auto it : this->to_send)
// 	{
// // 		cout << "msg: " << it.second << " total: " << tot << endl;
// // 		cout << "num should be " << it.second / tot << endl;
// 		it.second = it.second / tot;
// // 		cout << "num is " << it.second << endl;
// // 		cin.get();
// 	}
}


void Graph::sumProd()
{
	/*
	 * Since triangles of nodes only exist in the author data which
	 * is too large for this alg to process, all factor nodes will
	 * only receive and send messages between 2 nodes
	 */
	//Factor node part
	//Sum of all incoming messages to factor node times weight of edge
	//Since all factor nodes only receive and send between 2 nodes and 
	//all edge weight is 1 then factor node just mult message from node by 1 and send it on
	//So this implementation does not use factor nodes
		
		
	//Var Node Part
	//Product of all incoming messages
	
	initG();
	
	int maxsteps = 1000000, timestep = 0;
#ifdef COUNT
	int cnt_snt;
#endif
	
	//Pass messages until max steps reached or converged
	while((timestep < maxsteps) && !this->converged)
	{
#ifdef COUNT
		cnt_snt = 0;
#endif
		++timestep;
		cout << "On step " << timestep << endl;
		
		for(auto it : this->_graph)
		{
			//Prep Message
			it.second->prepMessage();
		}
		
		for(auto it : this->_graph)
		{
			for(auto msgs = it.second->toSendBegin(); msgs != it.second->toSendEnd(); ++msgs)
			{
				//msgs list of <receiver, msg>
				//it node currently sending
				//Send/receive Message
				this->_graph[msgs->first]->rcvMessage(msgs->second, it.first);
				//update messages sent
				it.second->updateSntMsg(msgs->first);
#ifdef DEBUG
				cout << "Message " << msgs->second << " sent to " << this->_graph[msgs->first]->getId() << " from " << it.first << endl;
				cout << msgs->first << " got message " << this->_graph[msgs->first]->msgAt(it.first) << endl;
#endif
#ifdef COUNT
				++cnt_snt;
#endif
			}
			//reset messages to send
			it.second->resetToSnd();
		} 
		
		
		this->converged = checkConverge();
		
#ifdef DEBUG
		cin.get();
#endif
		
		
#ifdef COUNT
		//This should not happen since there are no triangles
		//but there are cycles.....
		if(!this->converged && !cnt_snt)
			cout << "Stuck!" << endl;
#endif
	}
	
	if(!this->converged)
	{
		cout << "No convergence!" << endl;
		for(auto it : this->_graph)
		{
			cout <<"Node: " << it.first << " msg count: " << it.second->msgCount() << " rcv size: " << it.second->rcvMsgSize() <<
				" snt msgs: " << it.second->sntAllMsgs() << endl;
		}
// 		cin.get();
		return;
	}
	
// 	for(auto it : this->_graph)
// 		it.second->calcG();
}


bool Graph::tapConverge()
{
	bool check = true;
	
	for(auto it : this->_graph)
	{
		for(auto edge = it.second->getEdgesBegin(),  old = it.second->getOldBegin(); edge != it.second->getEdgesEnd() || old != it.second->getOldEnd(); ++edge, ++old)
		{
			if(edge->second.r != old->second.r)
			{
				check = false;
				old->second.r = edge->second.r; 
			}
			
			if(edge->second.a != old->second.a)
			{
				check = false;
				old->second.a = edge->second.a; 
			}
		}
	}
	
	return check;
}



void Graph::TAP()
{
// 	initG();
// 	initB();
	
	int maxsteps = 1000000, timestep = 0;
	do
	{
		++timestep;
		cout << "On step " << timestep << endl;
		
		updateR();
		
		updateASelf();
		
		updateA();
		
		this->converged = tapConverge();
	}while((timestep < maxsteps) && !this->converged);
	
	if(this->converged)
	{
		cout << "converged" << endl;
		calcM();
	}
	//can do influence stuff
	
	cout << "no convergence" << endl;
}


/******************************
 * Global functions 
 ******************************/

unordered_map<int, shared_ptr<Graph>> MultiFileRead(string path)
{
	DIR* dir;
	struct dirent* ent;
	unordered_map<int, shared_ptr<Graph>> graphs;
	
// 	cout << "line 159" << endl;
	
	if((dir = opendir(path.c_str())) != NULL)
	{
		shared_ptr<Graph> graph_ptr;
		while((ent = readdir(dir)) != NULL)
		{
			if(ent->d_type != DT_REG)
				continue;
			
			string temp;
			temp.assign(path);
			if(temp.rfind("/") == (temp.length()-1))
				temp+= string(ent->d_name);
			else
				temp+= "/" + string(ent->d_name);
			
			cout << "Starting read in of "<< temp <<  "..." << endl;
			
			graph_ptr = shared_ptr<Graph>(new Graph());
			
			graph_ptr->setName(ent->d_name);
			
			graph_ptr->init(temp);
			
			cout << "Finished read in of "<< temp <<  "." << endl;
			
			
// 			cout << "on insert" << endl;
			
			//Will need an else if for author read in if ever added
			//but that requires hadoop
			if(path.find("movies") < path.length())
			{
				graphs.insert(make_pair(movies[string(ent->d_name)], graph_ptr));
			}
			
			else if(path.find("pubs") < path.length())
			{
				graphs.insert(make_pair(cit[string(ent->d_name)], graph_ptr));
			}
			else //for testing only atm
			{
				graphs.insert(make_pair(test[string(ent->d_name)], graph_ptr));
			}

			graph_ptr.reset();
		}
		closedir(dir);
		free(ent);
	}
	
	else
	{
		cout << "Path: " << path << " is not a directory and could not be opened." << endl;
		cout << "Exit due to data read failure." << endl;
		graphs.clear();
		exit(1);
	}
	
	
	return graphs;
}


void MultiGraphSumProd(unordered_map<int, shared_ptr<Graph>> graphs)
{
	//Could make this parallel by popping each on off into a thread
	//would make testing faster
	
	for(auto it : graphs)
	{
		cout << "Started sumprod on " << it.second->getName() << "..." << endl;
		it.second->sumProd();
		cout << "Finished sumprod on " << it.second->getName() << endl;
	}
}

void MultiGraphTap(unordered_map<int, shared_ptr<Graph>> graphs)
{
	//Could make this parallel by popping each on off into a thread
	//would make testing faster
	
	for(auto it : graphs)
	{
		cout << "Started tap on " << it.second->getName() << "..." << endl;
		it.second->TAP();
		cout << "Finished tap on " << it.second->getName() << endl;
	}
}

































