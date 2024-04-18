#include "UnorientedGraph.h"

UnorientedGraph::UnorientedGraph(const int& size) {
	// генерация пустого графа размера size
	this->vertices_num = size;
	adjacency_m = {};
	adjacency_m.resize(size, std::vector<bool>(size, 0));
	edges_num = 0;

	generateWeightMatrix();
	generateKirchhoffMatrix();
}

UnorientedGraph::UnorientedGraph(const std::vector<int>& distribution) {
	// генерация графа по распределению
	this->vertices_num = distribution.size();
	adjacency_m = {};
	adjacency_m.resize(vertices_num, std::vector<bool>(vertices_num, 0));
	edges_num = 0;

	generateAdjacencyMatrix(distribution);
	generateWeightMatrix();
	generateKirchhoffMatrix();
}

void UnorientedGraph::generateAdjacencyMatrix(const std::vector<int>& distribution) {
	for (int i = 0; i < vertices_num; i++) {
		int upper_lim = (vertices_num - i - 1 > distribution[i]) ? (distribution[i]) : (vertices_num - i - 1);
		if (upper_lim == 0 and i != vertices_num - 1) {
			upper_lim = 1;
		}

		for (int j = 0; j < upper_lim; j++) {
			int k = rand() % vertices_num;
			if (adjacency_m[i][k] != 1 and k > i) {
				adjacency_m[i][k] = 1;
				adjacency_m[k][i] = 1;
				edges_num++;
			}
			else {
				upper_lim++;
			}
		}
	}
}

void UnorientedGraph::generateWeightMatrix(bool add_negative) {
	// генерация матрицы весов на основе матрицы смежности
	// если add_negative == 1, то половина весов будет отрицательной
	negative_weights = add_negative;
	weight_m = {};
	weight_m.resize(adjacency_m.size(), std::vector<int>(adjacency_m.size(), 0));

	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = i; j < adjacency_m.size(); j++) {
			if (adjacency_m[i][j] == 1) {
				weight_m[i][j] = rand() % (max_weight - 1) + 1;

				if (add_negative == 1 and rand() % 2 == 0) {
					weight_m[i][j] = -weight_m[i][j];
				}
				weight_m[j][i] = weight_m[i][j];
			}
		}
	}
}

void UnorientedGraph::generateKirchhoffMatrix() {
	kirchhoff_m = {};
	kirchhoff_m.resize(adjacency_m.size(), std::vector<int>(adjacency_m.size(), 0));

	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = i; j < adjacency_m.size(); j++) {
			if (i == j) {
				kirchhoff_m[i][j] = std::count(adjacency_m[i].begin(), adjacency_m[i].end(), true);
				continue;
			}

			kirchhoff_m[i][j] = - int(adjacency_m[i][j]);
			kirchhoff_m[j][i] = kirchhoff_m[i][j]; // граф неориентированный
		}
	}
}


int UnorientedGraph::spanningTreesNum() {
	// подсчет числа остовных деревьев в исходном графе с помощью матричной теоремы Кирхгофа
	std::vector<std::vector<int>> minor = {};
	minor.resize(kirchhoff_m.size()-1, std::vector<int>(kirchhoff_m.size()-1, 0));

	// получаем минор матрицы без первых столбца и строки 
	for (int i = 1; i < kirchhoff_m.size(); i++) {
		for (int j = 1; j < kirchhoff_m.size(); j++) {
			minor[i - 1][j - 1] = kirchhoff_m[i][j]; 
		}
	}

	return determinant(minor);
}


void UnorientedGraph::printAdjacencyMatrix() {
	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = 0; j < adjacency_m.size(); j++) {
			std::cout << std::right << std::setw(width) << adjacency_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

void UnorientedGraph::printWeightMatrix() {
	for (int i = 0; i < weight_m.size(); i++) {
		for (int j = 0; j < weight_m.size(); j++) {
			std::cout << std::right << std::setw(width) << weight_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

void UnorientedGraph::printKirchhoffMatrix() {
	for (int i = 0; i < kirchhoff_m.size(); i++) {
		for (int j = 0; j < kirchhoff_m.size(); j++) {
			std::cout << std::right << std::setw(width) << kirchhoff_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

int determinant(const std::vector<std::vector<int>>& matrix) {
	// подсчет определителя матрицы
	int n = matrix.size();
	if (n == 1) {
		// Базовый случай: определитель матрицы 1x1 равен единственному элементу
		return matrix[0][0];
	}

	int det = 0;
	int sign = 1;

	for (int i = 0; i < n; i++) {
		// Создаем подматрицу для минора
		std::vector<std::vector<int>> submatrix;
		for (int j = 1; j < n; j++) {
			std::vector<int> row;
			for (int k = 0; k < n; k++) {
				if (k != i) {
					row.push_back(matrix[j][k]);
				}
			}
			submatrix.push_back(row);
		}

		// Рекурсивно вычисляем определитель подматрицы и добавляем в сумму
		det += sign * matrix[0][i] * determinant(submatrix);
		sign = -sign; // чередование знаков
	}

	return det;
}