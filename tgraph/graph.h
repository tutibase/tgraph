#pragma once
#include <vector>
#include <iostream>

class graph {
public:
	graph(const int& size);
	graph(const std::vector<int>& distribution);

	void printAdjacencyMatrix();

	const std::vector<std::vector<bool>>& getAdjacencyMatrix() { return adjacency_m; };
	const int& getVerticesNum() { return vertices_num; };
	const int& getEdgesNum() { return edges_num; };

private:
	int vertices_num; // количество вершин
	int edges_num; // количество ребер
	std::vector<std::vector<bool>> adjacency_m; // матрица смежности
};

