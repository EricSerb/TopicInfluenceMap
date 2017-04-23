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
		this->_edges = new vector<Edge>();
	}

	~Node() 
	{
		this->name.clear();
		this->_edges->clear();
		delete this->_edges;
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

	//Edge Functionality
	void addEdge(int start, int end, int weight)
	{
		Edge *newEdge = new Edge;
		newEdge->start_node = start;
		newEdge->end_node = end;
		newEdge->weight = weight;
		this->_edges->push_back(*newEdge);
		delete newEdge;
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
	

private:
	int id;
	string name;
	int weight;
	vector<Edge>* _edges;
	//old
	//vector<Edges>* _edges;
};

class Graph 
{
public:

	Graph() 
	{
		
	}

	~Graph() 
	{
		this->_graph.clear();
	}

	void init(string file);
	vector<string> GenParse(string line);
	vector<string> VertParse(string line);

	unordered_map<int, shared_ptr<Node>>::iterator graphBegin();
	
	shared_ptr<Node> getNode(int index);
private:
	unordered_map<int, shared_ptr<Node>> _graph;
	//may try vector for speed as well since read in will be in order
	//so lookup with be linear since lookup is just int where node is in vector
	//vector<Node> _graph;
};



//Global prototypes
unordered_map<int, Graph> MultiFileRead(string path);


#endif