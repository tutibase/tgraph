#pragma once
#include <vector>
#include <iostream> 
#include <iomanip>
#include "OrientedGraph.h"
#include <numeric>
#include <algorithm>

class UnorientedGraph {
public:
	UnorientedGraph(const int& size);
	UnorientedGraph(const std::vector<int>& distribution);

	void printAdjacencyMatrix();
	void printWeightMatrix();
	void printKirchhoffMatrix();

	void generateWeightMatrix(bool add_negative = 0);
	void generateKirchhoffMatrix();

	int spanningTreesNum(); // количество минимальных остовных деревьев
	void Prim();
	void Boruvka();
	void generatePruferCode(const std::vector<int>& predecessors);
	void decodePruferCode();

	const std::vector<int>& getPredecessorsPrim() { return predecessors_Prim; };


private:
	int vertices_num; // количество вершин
	int edges_num; // количество ребер
	bool negative_weights; // наличие отрицательных весов дуг

	std::vector<std::vector<int>> weight_m; // матрица весов
	std::vector<std::vector<bool>> adjacency_m; // матрица смежности
	std::vector<std::vector<int>> kirchhoff_m; // матрица Кирхгофа

	std::vector<int> predecessors_Prim; // предшественники вершин MST  

	std::vector<int> prufer_code; // Вектор для сохранения кода Прюфера
	std::vector<int> prufer_code_weights; // Вектор для сохранения весов кода Прюфера

	void generateAdjacencyMatrix(const std::vector<int>& distribution);
	int findMinDist(std::vector<int>& distances, std::vector<bool>& mstSet);
	void printMST(std::vector<int>& predecessors);

	int find(int u, std::vector<int>& parent);
	void union_sets(int u, int v, std::vector<int>& parent, std::vector<int>& rank);
};

int determinant(const std::vector<std::vector<int>>& matrix);
bool isInVec(const std::vector<int>& vec, int item);
