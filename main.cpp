#include <chrono>
#include "graph.hpp"
#include "test.hpp"

#define DEBUG

int main(int argc, char**argv)
{
	//Testing code
#ifdef DEBUG

	auto start = std::chrono::high_resolution_clock::now();
	testParse();
	auto end = std::chrono::high_resolution_clock::now();

	cout << "testRead() took "
		<< std::chrono::duration_cast<std::chrono::milliseconds(start - end).count()
		<< " milliseconds" << endl;

	auto start = std::chrono::high_resolution_clock::now();
	testRead();
	auto end = std::chrono::high_resolution_clock::now();

	cout << "testRead() took " 
		<< std::chrono::duration_cast<std::chrono::milliseconds(start - end).count() 
		<< " milliseconds" << endl;
#endif

	return 0;
}