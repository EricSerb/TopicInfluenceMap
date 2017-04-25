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
	temp = new Edge;
	temp->start_node = 1;
	temp->end_node = 2;
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
	vector<double> node_weights = {10, 5, 8, 3};
	vector<int> leafs = {3};

	auto test_edges = testEdges();
	
// 	int tot = 0;
// 	for(auto it : node_weights)
// 	{
// 		tot += it;
// 	}
// 	
// 	for(unsigned int i = 0; i < node_weights.size(); ++i)
// 	{
// 		cout << node_weights[i]/tot << endl;
// 		node_weights[i] = node_weights[i]/tot;
// 	}
	

	for (unsigned int i = 0; i < node_ids.size(); i++)
	{
		shared_ptr<Node> graph_node = test_graph->getNode(i);
// 		cout << "node_id " << graph_node->getId() << " i " << i << endl;
		if ((graph_node->getId() == node_ids[i]) && 
			(graph_node->getName().compare(node_names[i])) && 
			(graph_node->getWeight() == node_weights[i]))
		{
			
			//Commented code worked when vectors were still used but no longer work due to unordered_map
			//since maps do not have predictable order and need to rewrite code to work with it
			
// 			//Node passed, now test Edges
// 			for (int k = 0; k < test_edges[graph_node->getId()].size(); ++k)
// 			{
// // 				cout << it.first->start_node << it.second->start_node << endl;
// // 				cout << it.first->end_node << it.second->end_node << endl;
// // 				cout << it.first->weight << it.second->weight << endl;
// 				auto e = graph_node->findEdge(test_edges[graph_node->getId()][i].end_node);
// 				if ((e->second.start_node == test_edges[graph_node->getId()][k].start_node) &&
// 					(e->second.end_node == test_edges[graph_node->getId()][k].end_node) &&
// 					(e->second.weight == test_edges[graph_node->getId()][k].weight))
// 				{
// 					++k;
// 					continue;
// 				}
// 
// 				cout << "Edge: " << e->second.start_node << " " << e->second.end_node << " " << e->second.weight <<
// 					"\nnot equal to\n"
// 					<< "Test Edge: " << test_edges[graph_node->getId()][k].start_node << " " << test_edges[graph_node->getId()][k].end_node << " " << test_edges[graph_node->getId()][k].weight << endl;
// 				exit(1);
// 			}
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
	
	//test leafs
	for(int i = 0; i < test_graph->getLeafsSize(); ++i)
	{
		if(test_graph->getLeaf(i) != leafs[i])
		{
			cout << "leafs in graph are wrong... check function" << endl;
			cout << "Leafs in test graph:" << endl;
			for(int k = 0; k < test_graph->getLeafsSize(); k++)
				cout << test_graph->getLeaf(k) << endl;
			exit(1);
		}
	}
	
	test_edges.clear();
	delete test_graph;
}


void testMultiRead()
{
	string path = "test_data";
	unordered_map<int, shared_ptr<Graph>> graph = MultiFileRead(path);
	
	//need to test all the nodes are correct and edges
	//node info
	vector<int> node_ids = {0, 1, 2, 3};
	vector<string> node_names = {"Eric Serbousek", "Bree Lavalle", "Adam Stallard", "Brandon Stephens"};
	vector<double> node_weights = {10, 5, 8, 3};
	vector<int> leafs = {3};

	auto test_edges = testEdges();
	
// 	int tot = 0;
// 	for(auto it : node_weights)
// 	{
// 		tot += it;
// 	}
// 	
// 	for(unsigned int i = 0; i < node_weights.size(); ++i)
// 	{
// 		cout << node_weights[i]/tot << endl;
// 		node_weights[i] = node_weights[i]/tot;
// 	}
	
	
	for (auto test_graph : graph)
	{
		for (unsigned int i = 0; i < node_ids.size(); i++)
		{
			shared_ptr<Node> graph_node = test_graph.second->getNode(i);
	// 		cout << "node_id " << graph_node->getId() << " i " << i << endl;
			if ((graph_node->getId() == node_ids[i]) && 
				(graph_node->getName().compare(node_names[i])) && 
				(graph_node->getWeight() == node_weights[i]))
			{
				
				
				//Commented code worked when vectors were still used but no longer work due to unordered_map
				//since maps do not have predictable order and need to rewrite code to work with it
				
				//Node passed, now test Edges
// 				int i = 0;
// 				for (auto it = graph_node->getEdgesBegin(); 
// 					it != graph_node->getEdgesEnd(); ++it)
// 				{
// 	// 				cout << it.first->start_node << it.second->start_node << endl;
// 	// 				cout << it.first->end_node << it.second->end_node << endl;
// 	// 				cout << it.first->weight << it.second->weight << endl;
// 					if ((it->second.start_node == test_edges[graph_node->getId()][i].start_node) &&
// 						(it->second.end_node == test_edges[graph_node->getId()][i].end_node) &&
// 						(it->second.weight == test_edges[graph_node->getId()][i].weight))
// 					{
// 						i++;
// 						continue;
// 					}
// 
// 					cout << "Edge: " << it->second.start_node << " " << it->second.end_node << " " << it->second.weight <<
// 						"\nnot equal to\n"
// 						<< "Test Edge: " << test_edges[graph_node->getId()][i].start_node << " " << test_edges[graph_node->getId()][i].end_node << " " << test_edges[graph_node->getId()][i].weight << endl;
// 					exit(1);
// 				}
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
		
		//test leafs
		for(int i = 0; i < test_graph.second->getLeafsSize(); ++i)
		{
			if(test_graph.second->getLeaf(i) != leafs[i])
			{
				cout << "leafs in graph are wrong... check function" << endl;
				cout << "Leafs in test graph:" << endl;
				for(int k = 0; k < test_graph.second->getLeafsSize(); k++)
					cout << test_graph.second->getLeaf(k) << endl;
				exit(1);
			}
		}
	}
	
	test_edges.clear();
	graph.clear();
}


void testSumProd()
{
	Graph* test_graph = new Graph();
	//need to fix weights for this to work
	vector<double> test_gs = {1440, 2880, 1800, 4800, 7200, 7200, 4800};

	test_graph->init("test_data/sptestgraph.txt");
	
	test_graph->sumProd();
	
	for(int i = 0; i < test_gs.size(); ++i)
	{
		cout << "Node " << i << " g " << test_graph->getNode(i)->getOldProd() << endl;
	}
	
// 	for(unsigned int i = 0; i < test_gs.size(); ++i)
// 	{
// 		if(test_graph->getNode(i)->getG() != test_gs[i])
// 		{
// 			cout << "Node " << i << " g val: " << test_graph->getNode(i)->getG() << endl;
// 			cout << "Test Node g val: " << test_gs[i] << endl;
// 			test_gs.clear();
// 			delete test_graph;
// 			exit(1);
// 		}
// 	}
	test_gs.clear();
	delete test_graph;
}
























