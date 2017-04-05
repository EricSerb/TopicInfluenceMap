#include "graph.h"
#include <string>
#include <iostream>


//TODO: Move all of this to the .hpp file for graph since that is where it should be and not in .cpp
//TODO: Will need multiple types of nodes since nodes will contain different information depending on dataset
using namespace std;
typedef struct edge {
	int start_node;
	int end_node;
	int weight;
}Edge;

class Edges {
public:
	Edges() {
		_edges = new std::vector<Edge>;
	}
	~Edges() {
		delete[] _edges;
	}

	void add_edge(int start, int end, int weight);
	int get_edge(int index);

private:
	//Only need id of node that edge goes to since edges contained in 
	//node class and starting node id stored in there
	std::vector<Edge> _edges;
};

//Maybe use this for node base class and then inherit from this for other node classes
class Node {
public:
	Node(int id, string name, int weight){
		this->id = id;
		this->name.assign(name);
		this->weight = weight;
		this->_edges = new Edges;
	}

	~Node() {
		delete _edges;
	}

private:
	int id;
	string name;
	int weight;
	Edges _edges;
};
class Graph {
public:

private:

};