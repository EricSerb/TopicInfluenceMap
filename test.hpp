#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "graph.hpp"

using namespace std;

/****Function Prototypes****/

/****test_main.cpp Functions****/
void testSuite();

/****test.cpp Functions****/

//Function to test parsing function of graph
void testParse();

//Function to create test edges
vector<vector<Edge>> testEdges();

//Function to test reading in files
void testRead();

//Function to test reading in multiple files, each file is a topic
void testMultiRead();


#endif