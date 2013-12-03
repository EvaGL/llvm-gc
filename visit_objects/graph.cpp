
#include <fstream>
#include "graph.h"

graph::graph(std::string name) {
	file_name = name;
	file_out.open(name.c_str());
	file_out << "digraph G {\n";
}
    
void graph::write(void* parent, void* son) {
	file_out << "    obj_" << parent << " -> ";
	if (son) {
		file_out << "obj_" << son << ";\n";
	} else {
		file_out << "objnull_" << parent << ";\n";
		file_out << "objnull_" << parent << "[label = \"0\"]" << ";\n";
	}
}

graph::~graph() {
	file_out << "}\n";
	file_out.close();
}

