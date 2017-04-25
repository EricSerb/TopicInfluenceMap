#ifndef GRAPH_HPP
#define GRAPH_HPP

#define _USE_MATH_DEFINES

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
#include <cmath>
#include <algorithm>


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
	double g;
	double b;
	double r;
	double a;
	double m;
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
		this->weight = (double)weight;
		this->g_node = 1.0;
		this->old_Prod = 0.0;
		this->_edges = new unordered_map<int, Edge>();
		this->_old_edges = new unordered_map<int, Edge>();
		addEdge(this->id, this->id, this->weight);
	}

	~Node() 
	{
		this->name.clear();
		this->_edges->clear();
		this->_old_edges->clear();
		delete this->_edges;
		this->rcv_msg.clear();
		this->snt_msg.clear();
		this->to_send.clear();
// 		this->self_edge.reset();
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

	double getWeight()
	{
		return this->weight;
	}
	
	void setWeight(double w)
	{
		this->weight = w;
	}
	
	double getTmpWeight()
	{
		return this->tmp_wt;
	}
	
	void setTmpWeight(double w)
	{
		this->tmp_wt = w;
	}
	
	void correctWeight()
	{
		this->weight = 1.0;
	}
	
	void prepMessage();
	
	unordered_map<int, double>::iterator toSendBegin()
	{
		return this->to_send.begin();
	}
	
	unordered_map<int, double>::iterator toSendEnd()
	{
		return this->to_send.end();
	}
	
	void rcvMessage(double m, int n)
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
	
	double msgAt(int idx)
	{
		return this->rcv_msg[idx];
	}
	
	int rcvMsgSize()
	{
		return this->rcv_msg.size();
	}
	
	double calcG()
	{
		this->g_node = 1;
		for(auto it : this->rcv_msg)
		{
			this->g_node *= it.second;
		}
		return this->g_node;
	}
	
	double getG()
	{
		return this->g_node;
	}
	
	double getOldProd()
	{
		return this->old_Prod;
	}
	
	void setOldProd(double p)
	{
		this->old_Prod = p;
	}
	
	double getMsgTotal()
	{
		double tot = 1;
		for(auto it : this->to_send)
			tot*= it.second;
		return tot;
	}
	
// 	//DEPRECATED since switching _edges to unordered_map
// 	shared_ptr<Edge> getSelfEdge()
// 	{
// 		return this->self_edge;
// 	}
	

	//Edge Functionality
	void addEdge(int start, int end, int weight)
	{
		for(auto it : *(this->_edges))
		{
			if((it.second.start_node == start) && (it.second.end_node == end))
				return;
		}
		Edge *newEdge = new Edge;
		newEdge->start_node = start;
		newEdge->end_node = end;
		newEdge->weight = weight;
		newEdge->g = 0.0;
		newEdge->b = 0.0;
		newEdge->r = 0.0;
		newEdge->a = 0.0;
		newEdge->m = 0.0;
		this->_edges->insert(make_pair(end, *newEdge));
		this->_old_edges->insert(make_pair(end, *newEdge));
		delete newEdge;
		
		this->rcv_msg.insert(make_pair(end, 1));
		this->snt_msg.insert(make_pair(end, false));
	}

	//idx is end node
	Edge getEdge(int index)
	{
		return this->_edges->at(index);
	}
	
	unordered_map<int, Edge>::const_iterator findEdge(int idx)
	{
		return this->_edges->find(idx);
	}
	
	//DEPRECATED since switching to unordered_map
	Edge getEdgeTo(int idx)
	{
		for(auto it : *(this->_edges))
		{
			if(it.second.end_node == idx)
				return it.second;
		}
	}

	unordered_map<int, Edge>::iterator getEdgesBegin()
	{
		return this->_edges->begin();
	}

	unordered_map<int, Edge>::iterator getEdgesEnd()
	{
		return this->_edges->end();
	}
	
	unordered_map<int, Edge>::iterator getOldBegin()
	{
		return this->_old_edges->begin();
	}

	unordered_map<int, Edge>::iterator getOldEnd()
	{
		return this->_old_edges->end();
	}
	
	int getEdgeSize()
	{
		return this->_edges->size();
	}
	
	void printEdges()
	{
		for(auto it : *_edges)
			cout << it.second.start_node << "<-->" << it.second.end_node << endl;
	}
	
	//TAP
	
	void setAEdge(int i, double a)
	{
		this->_edges->at(i).a = a;
	}
	

private:
	//Private Funcs
	
	
	//Private Vars
	int id;
	string name;
	double weight;
	double tmp_wt;
	double g_node;
	unordered_map<int, Edge>* _edges;
	unordered_map<int, Edge>* _old_edges;
	double old_Prod;
// 	shared_ptr<Edge> self_edge;
	//<msg from, contents>
	unordered_map<int, double> rcv_msg;
	//<sent to, yes/no>
	unordered_map<int, bool> snt_msg;
	//<msg to, content>
	unordered_map<int, double> to_send;
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
		this->total = 0;
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
	
	void normalizeWeights()
	{
		for(auto it : this->_graph)
		{
			double nbr_wt = 0;
			for(auto eg = it.second->getEdgesBegin(); eg != it.second->getEdgesEnd(); ++eg)
			{
				nbr_wt += (this->_graph[eg->second.end_node]->getWeight() + it.second->getWeight());
			}
// 			cout << "node\tnbr\tcurrent\tnew" << endl;
// 			cout << it.first << "\t" << nbr_wt << "\t" << it.second->getWeight() << "\t" << it.second->getWeight() / nbr_wt <<endl;
			it.second->setTmpWeight(it.second->getWeight() / nbr_wt);
		}
		
		for(auto it : this->_graph)
		{
			it.second->setWeight(it.second->getTmpWeight());
		}
	}

	unordered_map<int, shared_ptr<Node>>::iterator graphBegin();
	
	shared_ptr<Node> getNode(int index);
	
	vector<string> GenParse(string line);
	vector<string> VertParse(string line);
	
	void sumProd();
	
	//TAP stuff
	//only making this work for a single graph atm
	void TAP();
	bool tapConverge();
	
	void initG()
	{
		for(auto it : this->_graph)
		{
			double g_bot = sumNbrInfluence(it.second);
			
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
				if(edge->first == it.first)
					edge->second.g = (selfSum(it.second)/g_bot);
				else
					edge->second.g = (this->_graph[edge->second.end_node]->getWeight() * edge->second.weight) / g_bot;
			}
		}
	}
	
	void initB()
	{
		for(auto it : this->_graph)
		{
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
					edge->second.b = log((edge->second.g)/(sumNbrG(it.second)));
			}
		}
	}
	
	//rij
	void updateR()
	{
		for(auto it : this->_graph)
		{
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
				//r gets b - max(b'+a') b'a' on edge ik where k nbr of j
				edge->second.r = edge->second.b - maxJNbrBA(edge->second.start_node, edge->second.end_node);
			}
		}
	}
	
	//ajj
	void updateASelf()
	{
		//updates node TmpWeight
		//max value between all neighbor min (r values and 0)
		
		vector<double> mins;
		
		for(auto it : this->_graph)
		{
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
				mins.push_back((this->_graph[edge->second.end_node]->getEdge(it.first).r, 0.0));
			}
			it.second->setAEdge(it.first, (*max_element(mins.begin(), mins.end())));
			mins.clear();
		}
		
	}
	
	//aij
	void updateA()
	{
		for(auto it : this->_graph)
		{
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
				if(edge->first != it.first)
				{
					edge->second.a = min(max(this->_graph[edge->first]->getEdge(edge->first).r, 0.0), 
										 (-min(this->_graph[edge->first]->getEdge(edge->first).r, 0.0) - 
											maxMinNbrs(edge->first, it.first)));
				}
			}
		}
	}
	
	void calcM()
	{
		//m_st is influnce of v_s on v_t
		for(auto it : this->_graph)
		{
			for(auto edge = it.second->getEdgesBegin(); edge != it.second->getEdgesEnd(); ++edge)
			{
				edge->second.m = 1 / (1 + pow(M_E, -(this->_graph[edge->first]->getEdge(it.first).r + this->_graph[edge->first]->getEdge(it.first).a)));
			}
		}
	}
	
	
	
// 	Edge getEdge(int s_node, int n_node)
// 	{
// 		for(auto it = this->_graph[s_node]->getEdgesBegin(); it != )
// 	}
	
	
private:
	
	//Private Funcs
	void findLeafs();
	bool checkConverge();
	
	/*The summation of neighbors weight * edge wt +  curr node weight * edge wt for all neighbors */
	double sumNbrInfluence(shared_ptr<Node> curr)
	{
		double tot = 0;
		for(auto it = curr->getEdgesBegin(); it != curr->getEdgesEnd(); ++it)
		{
			tot += ((this->_graph[it->second.end_node]->getWeight() * it->second.weight) + (curr->getWeight() * it->second.weight));
		}
		return tot;
	}
	
	double selfSum(shared_ptr<Node> curr)
	{
		double tot = 0;
		for(auto it = curr->getEdgesBegin(); it != curr->getEdgesEnd(); ++it)
		{
			tot += (curr->getWeight() * it->second.weight);
		}
		return tot;
	}
	
	double sumNbrG(shared_ptr<Node> curr)
	{
		double tot = 0;
		for(auto it = curr->getEdgesBegin(); it != curr->getEdgesEnd(); ++it)
		{
			tot += (it->second.g);
		}
		return tot;
	}
	
	double maxJNbrBA(int i, int j)
	{
		vector<double> mx = {0};
		for(auto edge = this->_graph[j]->getEdgesBegin(); edge != this->_graph[j]->getEdgesEnd(); ++edge)
		{
			unordered_map<int, Edge>::const_iterator test = this->_graph[edge->first]->findEdge(i);
			if(test != this->_graph[edge->first]->getEdgesEnd())
			{
				mx.push_back(this->_graph[i]->getEdge(test->second.start_node).b 
				+ this->_graph[i]->getEdge(test->second.start_node).a);
			}
		}
		
		return *max_element(mx.begin(), mx.end());
	}
	
	double maxMinNbrs(int j, int i)
	{
		vector<double> mins;
		for(auto edge = this->_graph[j]->getEdgesBegin(); edge != this->_graph[j]->getEdgesEnd(); ++edge)
		{
			if(edge->first != i)
			{
				mins.push_back(min(this->_graph[edge->first]->getEdge(j).r, 0.0));
			}
		}
		
		return *max_element(mins.begin(), mins.end());
	}
	
	//Private Vars
	
	//may try vector for speed as well since read in will be in order
	//so lookup with be linear since lookup is just int where node is in vector
	//vector<Node> _graph;
	unordered_map<int, shared_ptr<Node>> _graph;
	string gname;
	bool converged;
	vector<int> leafs;
	unordered_map<int, int> msgs_to_snd;
	unsigned long long int total;
};



//Global prototypes
unordered_map<int, shared_ptr<Graph>> MultiFileRead(string path);

void MultiGraphSumProd(unordered_map<int, shared_ptr<Graph>> graphs);

void MultiGraphTap(unordered_map<int, shared_ptr<Graph>> graphs);


#endif