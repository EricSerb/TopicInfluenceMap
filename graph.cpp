#include "graph.hpp"

#define DEBUG

void Graph::init(string file) 
{
	ifstream ifile;
	ifile.open(file, ifstream::in);

	string line = ifile.getline();
	
	vector<string> tokens = parse(line);

	//Tokens should hold Node title line with # of nodes
	//If not loop until it is
	while (!tokens[0].compare("*Vertices"))
	{
		line.assign(ifile.getline());
		tokens.clear();
		tokens = parse(line);
	}

	int numVerts = atoi(tokens[1]);

	for (int i = 0; i < numVerts; i++)
	{
		//Insert all of the node info in this loop
		line.assign(ifile.getline());
		tokens.clear();
		tokens = parse(line);

		/*
		Token contains:
		node id, name, weight
		*/
		Node* node = new Node(atoi(tokens[ID]), tokens[NAME], atoi(tokens[WEIGHT]));
		this->_graph->insert(node->getId, *node);
	}


	while (!tokens[0].compare("*Edges"))
	{
		line.assign(ifile.getline());
		tokens.clear();
		tokens = parse(line);
	}

	while (!ifile.eof())
	{
		line.assign(ifile.getline());
		tokens.clear();
		tokens = parse(line);

		Edges edges = this->_graph->at(edge->start_node).getEdges();

		edges.addEdge(atoi(tokens[0]), atoi(tokens[1]), atoi(tokens[2]));
	}

	ifile.close()
}

vector<string> Graph::parse(string line) 
{
	string delim = " ";
	size_t pos = 0;
	vector<string> tokens = new vector<string>;
	while ((pos = line.find) != string::npos) 
	{
		tokens.push_back(line.substr(0, pos));
#ifdef DEBUG
		//Debug message
		cout << tokens.back() << endl;
#endif
		line.erase(0, pos + delim.length());
	}
	return tokens;
}

unordered_map<int, Node>::iterator Graph::graphBegin()
{
	return this->_graph->begin();
}

Node Graph::getNode(int index)
{
	return this->_graph->at(index);
}