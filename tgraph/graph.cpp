#include "graph.h"

graph::graph(const int& size) {
	// генерация пустого графа размера size
	this->vertices_num = size;
	adjacency_m = {};
	adjacency_m.resize(size, std::vector<bool>(size, 0));
	edges_num = 0;
}


graph::graph(const std::vector<int>& distribution) {
	// генерация графа по распределению
	this->vertices_num = distribution.size();
	adjacency_m = {};
	adjacency_m.resize(vertices_num, std::vector<bool>(vertices_num, 0));
	edges_num = 0;
	
	for (int i = 0; i < vertices_num; i++) {
		int upper_lim = (vertices_num - i - 1 > distribution[i]) ? (distribution[i]) : (vertices_num - i - 1);
		if (upper_lim == 0 and i != vertices_num - 1) {
			upper_lim = 1;
		}

		for (int j = 0; j < upper_lim; j++) {
			int k = rand() % vertices_num;
			if (adjacency_m[i][k] != 1 and k > i) {
				adjacency_m[i][k] = 1;
				edges_num++;
			}
			else {
				upper_lim++;
			}
		}
	}
}

void graph::printAdjacencyMatrix() {
	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = 0; j < adjacency_m.size(); j++) {
			std::cout << adjacency_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}