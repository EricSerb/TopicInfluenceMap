#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#define ID 0
#define NAME 1
#define WEIGHT 2

using namespace std;

typedef struct edge 
{
	int start_node;
	int end_node;
	int weight;
}Edge;

class Edges 
{
public:
	Edges() 
	{
		_edges = new vector<Edge>;
	}
	~Edges() 
	{
		delete[] _edges;
	}

	void addEdge(int start, int end, int weight)
	{
		Edge *newEdge = new Edge;
		newEdge->start_node = start;
		newEdge->end_node = end;
		newEdge->weight = weight;
		this->_edges.push_back(*newEdge);
	}

	int getEdge(int index)
	{
		return this->_edges[index];
	}

	vector<Edge>::iterator getEdgesBegin()
	{
		return this->_edges.begin();
	}

	vector<Edge>::iterator getEdgesEnd()
	{
		return this->_edges.end();
	}

private:
	//Only need id of node that edge goes to since edges contained in 
	//node class and starting node id stored in there
	// Storing all of them just for uniformity with paper
	vector<Edge> _edges;
};

class Node {
public:
	Node(int id, string name, int weight) 
	{
		this->id = id;
		this->name.assign(name);
		this->weight = weight;
		this->_edges = new Edges;
	}

	~Node() 
	{
		delete _edges;
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

	vector<Edge> getEdges()
	{
		return this->_edges;
	}

private:
	int id;
	string name;
	int weight;
	Edges _edges;
};

class Graph 
{
public:

	Graph() 
	{
		this->_graph = new unordered_map<int, Node>;
	}

	~Graph() 
	{
		this->_graph.erase(this->_graph.begin());
		// delete[] _graph;
	}

	void init(string file);
	vector<string> parse(string line);

	unordered_map<int, Node>::iterator graphBegin();
	Node getNode(int index);
private:
	unordered_map<int, Node>* _graph;
	//may try vector for speed as well since read in will be in order
	//so lookup with be linear since lookup is just int where node is in vector
	//vector<Node> _graph;
};

#endif