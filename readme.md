# Topic Level Influence in Social Networks
This project is a work in progress.
The project is based on this paper: http://keg.cs.tsinghua.edu.cn/jietang/publications/KDD09-Tang-et-al-Social-Influence-Analysis.pdf
which looks to analyze topic level influence in social networks.

# Language
Project is written in C++ and may use hadoop in the feature to improve the processing time of large networks.

# Goal
Simulate the results in the paper. My implementation may be slower due to not using the leveldb library to create bit maps for the nodes.

# Compiling
A make file is included to compile the program. The File only works on Linux. From the folder where the makefile is, simply run the command "make" and the executable main will be created.

# Running
To run the program it must first be compiled as explained above. Then run the executable main giving a path to a data file along with it. Data is included under the folder data/movies or data/graph_pubs.

Example run command:
./main data/movies
