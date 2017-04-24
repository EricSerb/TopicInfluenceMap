#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ratio>
#include <ctime>
#include <memory>


#include <dirent.h>

#define ID 0
#define NAME_ID2 1
#define WEIGHT 2

using namespace std;

//Globals to match topics with index numbers
//movies
static unordered_map<string, int> movies = {{"newmovies.txt", 0}};

//citation
static unordered_map<string, int> cit = {{"graph-16.net", 0}, {"graph-24.net", 1}, {"graph-75.net", 2}, {"graph-107.net", 3}, {"graph-131.net", 4},
								  {"graph-144.net", 5},{"graph-145.net", 6}, {"graph-162.net", 7}, {"graph-182.net", 8}, {"graph-199.net", 9}};

//test
static unordered_map<string, int> test = {{"testgraph.txt", 0}, {"testgraph2.txt", 1}};


//Graph/Node/Edge class/struct implementation

typedef struct edge 
{
	int start_node;
	int end_node;
	int weight;
}Edge;

//Probably abstracted more than it needs to be since struct edges could just be containted as a list in Node
// class Edges 
// {
// public:
// 	Edges() 
// 	{
// 		this->_edges = new vector<Edge>();
// 	}
// 	~Edges() 
// 	{
// 		delete[] this->_edges;
// 	}
// 
// 	void addEdge(int start, int end, int weight)
// 	{
// 		Edge *newEdge = new Edge;
// 		newEdge->start_node = start;
// 		newEdge->end_node = end;
// 		newEdge->weight = weight;
// 		this->_edges->push_back(*newEdge);
// 	}
// 
// 	Edge getEdge(int index)
// 	{
// 		return this->_edges->at(index);
// 	}
// 
// 	vector<Edge>::iterator getEdgesBegin()
// 	{
// 		return this->_edges->begin();
// 	}
// 
// 	vector<Edge>::iterator getEdgesEnd()
// 	{
// 		return this->_edges->end();
// 	}
// 
// private:
// 	//Only need id of node that edge goes to since edges contained in 
// 	//node class and starting node id stored in there
// 	// Storing all of them just for uniformity with paper
// 	vector<Edge>* _edges;
// };

class Node {
public:
	Node(int id, string name, int weight) 
	{
		this->id = id;
		this->name.assign(name);
		this->weight = weight;
		this->g_node = 1;
		this->old_Prod = 0;
		this->_edges = new vector<Edge>();
	}

	~Node() 
	{
		this->name.clear();
		this->_edges->clear();
		delete this->_edges;
		this->rcv_msg.clear();
		this->snt_msg.clear();
		this->to_send.clear();
// 		this->_edges->~Edges();
	}

	int getId() 
	{
		return this->id;
	}

	string getName()
	{
		return this->name;
	}

	int getWeight()
	{
		return this->weight;
	}
	
	void correctWeight()
	{
		this->weight = 1;
	}
	
	void prepMessage();
	
	unordered_map<int, unsigned int>::iterator toSendBegin()
	{
		return this->to_send.begin();
	}
	
	unordered_map<int, unsigned int>::iterator toSendEnd()
	{
		return this->to_send.end();
	}
	
	void rcvMessage(unsigned int m, int n)
	{
		this->rcv_msg[n] = m;
	}
	
	void updateSntMsg(int n)
	{
		this->snt_msg[n] = true;
	}
	
	void resetToSnd()
	{
		this->to_send.clear();
	}
	
	int msgCount()
	{
		int count = 0;
		for(auto it : this->rcv_msg)
		{
			if(it.second > 0)
				count++;
		}
		return count;
	}
	
	bool sntAllMsgs()
	{
		for(auto it : this->snt_msg)
		{
			if(!it.second)
				return false;
		}
		return true;
	}
	
	unsigned int msgAt(int idx)
	{
		return this->rcv_msg[idx];
	}
	
	int rcvMsgSize()
	{
		return this->rcv_msg.size();
	}
	
	unsigned int calcG()
	{
		this->g_node = 1;
		for(auto it : this->rcv_msg)
		{
			this->g_node *= it.second;
		}
		return this->g_node;
	}
	
	unsigned int getG()
	{
		return this->g_node;
	}
	
	unsigned int getOldProd()
	{
		return this->old_Prod;
	}
	
	void setOldProd(unsigned int p)
	{
		this->old_Prod = p;
	}
	

	//Edge Functionality
	void addEdge(int start, int end, int weight)
	{
		Edge *newEdge = new Edge;
		newEdge->start_node = start;
		newEdge->end_node = end;
		newEdge->weight = weight;
		this->_edges->push_back(*newEdge);
		delete newEdge;
		
		this->rcv_msg.insert(make_pair(end, 1));
		this->snt_msg.insert(make_pair(end, false));
	}

	Edge getEdge(int index)
	{
		return this->_edges->at(index);
	}

	vector<Edge>::iterator getEdgesBegin()
	{
		return this->_edges->begin();
	}

	vector<Edge>::iterator getEdgesEnd()
	{
		return this->_edges->end();
	}
	
	int getEdgeSize()
	{
		return this->_edges->size();
	}
	
	void printEdges()
	{
		for(auto it : *_edges)
			cout << it.start_node << "<-->" << it.end_node << endl;
	}
	

private:
	//Private Funcs
	
	//Private Vars
	int id;
	string name;
	int weight;
	unsigned int g_node;
	vector<Edge>* _edges;
	unsigned int old_Prod;
	//<msg from, contents>
	unordered_map<int, unsigned int> rcv_msg;
	//<sent to, yes/no>
	unordered_map<int, bool> snt_msg;
	//<msg to, content>
	unordered_map<int, unsigned int> to_send;
	//old
	//vector<Edges>* _edges;
};

class Graph 
{
public:

	Graph() 
	{
		this->converged = false;
		this->gname.assign("");
	}

	~Graph() 
	{
		this->gname.clear();
		this->leafs.clear();
		this->msgs_to_snd.clear();
		this->_graph.clear();
	}

	void init(string file);
	
	void setName(string name)
	{
		this->gname.assign(name);
	}
	
	string getName()
	{
		return this->gname;
	}
	
	int getLeafsSize()
	{
		return this->leafs.size();
	}
	
	int getLeaf(int idx)
	{
		return this->leafs[idx];
	}
	
	void printLeafs()
	{
		for(auto it : leafs)
			cout << it << endl;
	}

	unordered_map<int, shared_ptr<Node>>::iterator graphBegin();
	
	shared_ptr<Node> getNode(int index);
	
	vector<string> GenParse(string line);
	vector<string> VertParse(string line);
	
	void sumProd();
	
	
private:
	
	//Private Funcs
	void findLeafs();
	bool checkConverge();
	
	//Private Vars
	
	//may try vector for speed as well since read in will be in order
	//so lookup with be linear since lookup is just int where node is in vector
	//vector<Node> _graph;
	unordered_map<int, shared_ptr<Node>> _graph;
	string gname;
	bool converged;
	vector<int> leafs;
	unordered_map<int, int> msgs_to_snd;
};



//Global prototypes
unordered_map<int, shared_ptr<Graph>> MultiFileRead(string path);

void MultiGraphSumProd(unordered_map<int, shared_ptr<Graph>> graphs);


#endif