#include "test.hpp"

void testParse()
{
	Graph test_graph;

	string test_string = "Hello World testing!";

	auto tokens = test_graph.parse(test_string);

	vector<strings> test_tokens = ["Hello", "World", "testing!"];

	for (int i = 0; i < tokens.size(); i++)
	{
		if ((i < test_tokens.size()) && test_tokens[i].compare(tokens[i]))
			continue;
		cout << "token: " << token[i] << " != test token: " << test_tokens[i] << endl;
		exit(1);
	}
}

void testRead()
{
	Graph test_graph;

	test_graph.init("testgraph.txt");

	//need to test all the nodes are correct and edges
	//node info
	vector<int> node_ids = [0, 1, 2, 3];
	vector<string> node_names = ["Eric Serbousek", "Bree Lavalle", "Adam Stallard", "Brandon Stephens"];
	vector<int> node_weights = [10, 5, 8, 3];

	//edge info
	vector<vector<Edge>> test_edges;
	test_edges.resize(4);

	//Node 0
	Edge *temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 1;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 2;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 3;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);

	//Node 1
	temp = new Edge;
	temp->start_node = 1;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);

	//Node 2
	temp = new Edge;
	temp->start_node = 2;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	temp = new Edge;
	temp->start_node = 2;
	temp->end_node = 1;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);

	//Node 3
	temp = new Edge;
	temp->start_node = 3;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);

	for (int i = 0; i < node_ids.size(); i++)
	{
		Node graph_node = test_graph.getNode(i);
		if ((graph_node.getId() == node_ids[i]) && 
			(graph_node.getName().compare(node_names[i])) && 
			(graph_node.getWeight() == node_weights[i]))
		{
			//Node passed, now test Edges
			for (vector<Edge>::iterator eit = graph_node.getEdges().getEdgesBegin(), 
				vector<Edge>::iterator test_eit = test_edges[i].it;
				eit != graph_node.getEdges().getEdgesEnd(); eit++, test_eit++)
			{
				if ((eit->start_node == test_eit->start_node) &&
					(eit->end_node == test_eit->end_node) &&
					(eit->weight == test_eit->weight))
					continue;

				cout << "Edge: " << eit->start_node << " " << eit->end_node << " " << eit->weight <<
					"\nnot equal to\n"
					<< "Test Edge: " << test_eit->start_node << " " << test_eit->end_node << " " << test_eit->weight << endl;
				exit(1);
			}
			continue;
		}
		cout << "Node: " << graph_node.getId() << " " << graph_node.getName() << " " << graph_node.getWeight() <<
			"\nnot equal to\n"
			<< "Test Node: " << node_ids[i] << " " << node_names[i] << " " << node_weights[i] << endl;
		exit(1);
	}
	
}