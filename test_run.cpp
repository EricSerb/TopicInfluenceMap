#include "test.hpp"


void testSuite()
{
	//Testing Parser
	auto start = std::chrono::high_resolution_clock::now();
	testParse();
	auto end = std::chrono::high_resolution_clock::now();

	cout << "testParse() took "
		<< std::chrono::duration<double, std::milli>(end - start).count()
		<< " milliseconds" << endl;
	
		
	//Testing Graph class init or read function
	start = std::chrono::high_resolution_clock::now();
	testRead();
	end = std::chrono::high_resolution_clock::now();

	cout << "testRead() took " 
		<< std::chrono::duration<double, std::milli>(end - start).count() 
		<< " milliseconds" << endl;
		
		
	//Testing multi file read in
	start = std::chrono::high_resolution_clock::now();
	testMultiRead();
	end = std::chrono::high_resolution_clock::now();

	cout << "testMultiRead() took " 
		<< std::chrono::duration<double, std::milli>(end - start).count() 
		<< " milliseconds" << endl;
		
		
	cout << "finished tests" << endl;
}