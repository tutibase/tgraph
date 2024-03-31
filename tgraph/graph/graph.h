﻿#pragma once
#include <vector>
#include <iostream>
#include <iomanip>

const int max_weight = 10;
const int width = 3;

class graph {
public:
	graph(const int& size);
	graph(const std::vector<int>& distribution);

	std::vector<std::vector<int>> ShimbellMethod(const int& edges_num, const bool& flag = 0);
	int reachabilityCheck(const int& a, const int& b);
	void Dijkstra(int start_v, int end_v);
	bool BellmanFord(int start_v, int end_v);

	void generareWeightMatrix(bool add_negative = 0);

	void printAdjacencyMatrix();
	void printWeightMatrix();

	const std::vector<std::vector<bool>>& getAdjacencyMatrix() { return adjacency_m; };
	const int& getVerticesNum() { return vertices_num; };
	const int& getEdgesNum() { return edges_num; };

private:
	int vertices_num; // количество вершин
	int edges_num; // количество ребер
	bool negative_weights; // наличие отрицательных весов дуг

	std::vector<std::vector<int>> weight_m; // матрица весов
	std::vector<std::vector<int>> reachability_m; // мартица достижимости
	std::vector<std::vector<bool>> adjacency_m; // матрица смежности

	std::vector<int> get_shortest_path(int start_v, int end_v, const std::vector<int>& predecessors);

	void generareAdjacencyMatrix(const std::vector<int>& distribution);
};

