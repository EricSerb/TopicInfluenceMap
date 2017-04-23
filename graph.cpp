#include "graph.hpp"

// #define DEBUG
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
	
// 	cout << "17" << endl;
	
	
// 	cout << tokens[1] << endl;

	int numVerts = std::stoi(tokens[1]);

	for (int i = 0; i < numVerts; i++)
	{
		//Insert all of the node info in this loop
		getline(ifile, line);
		tokens.clear();
		tokens = VertParse(line);

		/*
		Token contains:
		node id, name, weight
		*/
		
// 		cout << "Node info: ID= " << tokens[ID] << " name= " << tokens[NAME_ID2] 
// 			<< " weight= " << tokens[WEIGHT] << endl;
		try
		{
			shared_ptr<Node> node = shared_ptr<Node>(new Node(std::stoi(tokens[ID]), tokens[NAME_ID2], std::stoi(tokens[WEIGHT])));
			int nid = node->getId();
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
// 	cout << "get to 69" << endl;
#ifdef COUNT
	unsigned int ecount = 0;
#endif
	while (getline(ifile, line))
	{
		tokens.clear();
		tokens = GenParse(line);

		shared_ptr<Node> n = this->_graph.at(std::stoi(tokens[ID]));
		
		n->addEdge(std::stoi(tokens[ID]), std::stoi(tokens[NAME_ID2]), std::stoi(tokens[WEIGHT]));
		
		n.reset();

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
}


vector<string> Graph::GenParse(string line) 
{
	vector<string> tokens;
	
	stringstream ss = stringstream(line);
	string temp;
	
	while (ss >> temp) 
	{
		tokens.push_back(temp);
#ifdef DEBUG
		//Debug message
		cout << tokens.back() << endl;
#endif
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
// 	cout << "tokens: " << tokens.back() << " Line: " << line << endl;
	
	id_pos = line.rfind("\"") + 1;
	sub.assign(line.substr(0, id_pos));
	tokens.push_back(sub.data());
	//Can change to 1 subtract 1 from delim.length() to remove quotes
	line.erase(0, id_pos + delim.length());
// 	cout << "tokens: " << tokens.back() << " Line: " << line << endl;
	
	stringstream ss = stringstream(line);
	string temp;
	
	while (ss >> temp) 
	{
		tokens.push_back(temp);
#ifdef DEBUG
		//Debug message
		cout << tokens.back() << endl;
#endif
	}
	
	return tokens;
}


unordered_map<int, shared_ptr<Node>>::iterator Graph::graphBegin()
{
	return this->_graph.begin();
}


shared_ptr<Node> Graph::getNode(int index)
{
	return this->_graph.at(index);
}


/******************************
 * Global functions 
 ******************************/

unordered_map<int, Graph> MultiFileRead(string path)
{
	DIR* dir;
	struct dirent* ent;
	unordered_map<int, Graph> graphs;
	
// 	cout << "line 159" << endl;
	
	if((dir = opendir(path.c_str())) != NULL)
	{
		Graph* graph_ptr;
		while((ent = readdir(dir)) != NULL)
		{
			if(ent->d_type != DT_REG)
				continue;
			
			string temp;
			temp.assign(path);
			temp+= "/" + string(ent->d_name);
			
			cout << "Starting read in of "<< temp <<  "..." << endl;
			
			graph_ptr = new Graph();
			
			graph_ptr->init(temp);
			
			cout << "Finished read in of "<< temp <<  "." << endl;
			
// 			cout << "on insert" << endl;
			
			//Will need an else if for author read in if ever added
			//but that requires hadoop
			if(path.find("movies"))
			{
				graphs.insert(make_pair(movies[ent->d_name], *graph_ptr));
			}
			
			else if(path.find("pubs"))
			{
				graphs.insert(make_pair(cit[ent->d_name], *graph_ptr));
			}
			else //for testing only atm
			{
				graphs.insert(make_pair(test[ent->d_name], *graph_ptr));
			}
			delete graph_ptr;
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