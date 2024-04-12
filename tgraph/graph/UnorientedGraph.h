#pragma once
#include <vector>
#include <iostream> 
#include <iomanip>
#include "OrientedGraph.h"

class UnorientedGraph {
public:
	UnorientedGraph(const int& size);
	UnorientedGraph(const std::vector<int>& distribution);

	void printAdjacencyMatrix();
	void printWeightMatrix();

	void generareWeightMatrix(bool add_negative = 0);


private:
	int vertices_num; // количество вершин
	int edges_num; // количество ребер
	bool negative_weights; // наличие отрицательных весов дуг

	std::vector<std::vector<int>> weight_m; // матрица весов
	std::vector<std::vector<bool>> adjacency_m; // матрица смежности

	void generareAdjacencyMatrix(const std::vector<int>& distribution);
};