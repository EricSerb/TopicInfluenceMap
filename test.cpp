#include "test.hpp"

void testParse()
{
	Graph test_graph;

	string test_string = "0 \"Eric Serbousek\" 10";

	vector<string> tokens = test_graph.VertParse(test_string);

	vector<string> test_tokens = {"0", "\"Eric Serbousek\"", "10"};

	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		if ((i < test_tokens.size()) && !test_tokens[i].compare(tokens[i]))
			continue;
		cout << "token: " << tokens[i] << " != test token: " << test_tokens[i] << endl;
		cout << "token count: " << tokens[i].length() << " " << test_tokens[i].length() << endl;
		exit(1);
	}
}


vector<vector<Edge>> testEdges()
{
	//edge info
	vector<vector<Edge>> test_edges;
	test_edges.resize(4);

	//Node 0
	Edge *temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 1;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;
	temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 2;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;
	temp = new Edge;
	temp->start_node = 0;
	temp->end_node = 3;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;

	//Node 1
	temp = new Edge;
	temp->start_node = 1;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;

	//Node 2
	temp = new Edge;
	temp->start_node = 2;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;
	temp = new Edge;
	temp->start_node = 2;
	temp->end_node = 1;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;

	//Node 3
	temp = new Edge;
	temp->start_node = 3;
	temp->end_node = 0;
	temp->weight = 1;
	test_edges[temp->start_node].push_back(*temp);
	delete temp;
	
	return test_edges;
}


void testRead()
{
	Graph* test_graph = new Graph();

	test_graph->init("test_data/testgraph.txt");

	//need to test all the nodes are correct and edges
	//node info
	vector<int> node_ids = {0, 1, 2, 3};
	vector<string> node_names = {"Eric Serbousek", "Bree Lavalle", "Adam Stallard", "Brandon Stephens"};
	vector<int> node_weights = {10, 5, 8, 3};

	auto test_edges = testEdges();
	

	for (unsigned int i = 0; i < node_ids.size(); i++)
	{
		shared_ptr<Node> graph_node = test_graph->getNode(i);
// 		cout << "node_id " << graph_node->getId() << " i " << i << endl;
		if ((graph_node->getId() == node_ids[i]) && 
			(graph_node->getName().compare(node_names[i])) && 
			(graph_node->getWeight() == node_weights[i]))
		{
			//Node passed, now test Edges
			for (pair<vector<Edge>::iterator, vector<Edge>::iterator> it(graph_node->getEdgesBegin(), test_edges[i].begin()); 
				 it.first < graph_node->getEdgesEnd(); 
				 ++it.first, ++it.second)
			{
// 				cout << it.first->start_node << it.second->start_node << endl;
// 				cout << it.first->end_node << it.second->end_node << endl;
// 				cout << it.first->weight << it.second->weight << endl;
				if ((it.first->start_node == it.second->start_node) &&
					(it.first->end_node == it.second->end_node) &&
					(it.first->weight == it.second->weight))
					continue;

				cout << "Edge: " << it.first->start_node << " " << it.first->end_node << " " << it.first->weight <<
					"\nnot equal to\n"
					<< "Test Edge: " << it.second->start_node << " " << it.second->end_node << " " << it.second->weight << endl;
				exit(1);
			}
			graph_node.reset();
			continue;
		}
		cout << "Node: " << graph_node->getId() << " " << graph_node->getName() << " " << graph_node->getWeight() <<
			"\nnot equal to\n"
			<< "Test Node: " << node_ids[i] << " " << node_names[i] << " " << node_weights[i] << endl;
		test_edges.clear();
		delete test_graph;
		exit(1);
	}
	test_edges.clear();
	delete test_graph;
}


void testMultiRead()
{
	string path = "test_data";
	unordered_map<int, Graph> graph = MultiFileRead(path);
	
	//need to test all the nodes are correct and edges
	//node info
	vector<int> node_ids = {0, 1, 2, 3};
	vector<string> node_names = {"Eric Serbousek", "Bree Lavalle", "Adam Stallard", "Brandon Stephens"};
	vector<int> node_weights = {10, 5, 8, 3};

	auto test_edges = testEdges();
	
	for (auto test_graph : graph)
	{
		for (unsigned int i = 0; i < node_ids.size(); i++)
		{
			shared_ptr<Node> graph_node = test_graph.second.getNode(i);
	// 		cout << "node_id " << graph_node->getId() << " i " << i << endl;
			if ((graph_node->getId() == node_ids[i]) && 
				(graph_node->getName().compare(node_names[i])) && 
				(graph_node->getWeight() == node_weights[i]))
			{
				//Node passed, now test Edges
				for (pair<vector<Edge>::iterator, vector<Edge>::iterator> it(graph_node->getEdgesBegin(), test_edges[i].begin()); 
					it.first < graph_node->getEdgesEnd(); 
					++it.first, ++it.second)
				{
	// 				cout << it.first->start_node << it.second->start_node << endl;
	// 				cout << it.first->end_node << it.second->end_node << endl;
	// 				cout << it.first->weight << it.second->weight << endl;
					if ((it.first->start_node == it.second->start_node) &&
						(it.first->end_node == it.second->end_node) &&
						(it.first->weight == it.second->weight))
						continue;

					cout << "Edge: " << it.first->start_node << " " << it.first->end_node << " " << it.first->weight <<
						"\nnot equal to\n"
						<< "Test Edge: " << it.second->start_node << " " << it.second->end_node << " " << it.second->weight << endl;
					exit(1);
				}
				graph_node.reset();
				continue;
			}
			cout << "Node: " << graph_node->getId() << " " << graph_node->getName() << " " << graph_node->getWeight() <<
				"\nnot equal to\n"
				<< "Test Node: " << node_ids[i] << " " << node_names[i] << " " << node_weights[i] << endl;
			graph.clear();
			test_edges.clear();
			exit(1);
		}
	}
	test_edges.clear();
	graph.clear();
}


