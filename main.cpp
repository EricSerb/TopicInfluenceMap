#include "graph.hpp"
#include "test.hpp"


#define DEBUG

int main(int argc, char**argv)
{
	
	if(argc < 2)
	{
		cout << "Path to data not given..." << endl;
		cout << "Must give: Exe Path" << endl;
		exit(1);
	}
	
	//Testing code
#ifdef DEBUG
	testSuite();
#endif
	
	/*
	 * Need to read in actual files and run the algorithm on them 
	 * 1) load in newmovies.txt
	 * --> May need to alter load in and graphs for other files since need topic level graphs for sum-product
	 * --> and whole network graphs for TAP
	 * --> they are the same for newmovies.txt
	 * 2) run sum prod algorithm
	 * 3) run TAP
	 */
	
	/*
	 * newmovies.txt is all one file and is not split by topics
	 * some of the nodes don't fit into the topics listed by the original paper
	 * going to treat it as one giant topic graph movies
	 * TODO: create readin for newmovies that seperates them into nodes
	 */

	DIR* dir = opendir("results");
	if(!dir)
	{
		system("mkdir results");
	}
	
	ofstream data_file("results/data.txt", std::fstream::out | std::fstream::app);
	string path = argv[1];
	
#ifdef DEBUG
	if(data_file.is_open())
		cout << "Outfile open" << endl;
#endif
	
	time_t ct = std::time(nullptr);
	data_file << "******Running tests for " << path << " " << 
	std::asctime(std::localtime(&ct)) << endl << endl;
	
	//1 Read in Data
	auto start = std::chrono::high_resolution_clock::now();
	unordered_map<int, shared_ptr<Graph>> graph = MultiFileRead(path);
	auto end = std::chrono::high_resolution_clock::now();
	
	
	auto time = std::chrono::duration<double>(end - start).count();
	cout << "Read in finished in " << time << " seconds" << endl;
	data_file << "Read in finished in " << time << " seconds" << endl << endl;

	
	
	//2 SumProd Alg
	start = std::chrono::high_resolution_clock::now();
	MultiGraphSumProd(graph);
	end = std::chrono::high_resolution_clock::now();
	
	
	time = std::chrono::duration<double>(end - start).count();
	cout << "SumProd Alg finished in " << time << " seconds" << endl;
	data_file << "SumProd Alg finished in " << time << " seconds" << endl << endl;

	
	//Finish printing results to file and clean everything up
	ct = std::time(nullptr);
	data_file << "~~~~~~~~~Finished test for " << path << " " << 
	std::asctime(std::localtime(&ct)) << 
	endl << endl << endl << endl;
	
	data_file.close();
	graph.clear();

	return 0;
}