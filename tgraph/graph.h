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
	int vertices_num; // ���������� ������
	int edges_num; // ���������� �����
	std::vector<std::vector<bool>> adjacency_m; // ������� ���������
};

