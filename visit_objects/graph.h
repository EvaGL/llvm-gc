#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>

struct graph {
	std::string file_name;
	std::ofstream file_out;
	graph(std::string name);
	void write(void* parent, void* son);
	~graph();
};

#endif 
