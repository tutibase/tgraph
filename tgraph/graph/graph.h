#pragma once
#include <vector>
#include <iostream>

const int max_weight = 10;

class graph {
public:
	graph(const int& size);
	graph(const std::vector<int>& distribution);

	void generareAdjacencyMatrix(const std::vector<int>& distribution);
	void generareWeightMatrix();
	void ShimbellMethod(const int& edges_num, const bool& flag = 1);

	void printAdjacencyMatrix();
	void printWeightMatrix();

	const std::vector<std::vector<bool>>& getAdjacencyMatrix() { return adjacency_m; };
	const int& getVerticesNum() { return vertices_num; };
	const int& getEdgesNum() { return edges_num; };

private:
	int vertices_num; // количество вершин
	int edges_num; // количество ребер

	std::vector<std::vector<bool>> adjacency_m; // матрица смежности
	std::vector<std::vector<int>> weight_m; // матрица весов

};

