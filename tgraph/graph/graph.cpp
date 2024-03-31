#include "graph.h"

graph::graph(const int& size) {
	// генерация пустого графа размера size
	this->vertices_num = size;
	adjacency_m = {};
	adjacency_m.resize(size, std::vector<bool>(size, 0));
	edges_num = 0;

	generareWeightMatrix();
	reachability_m = {};

}

graph::graph(const std::vector<int>& distribution) {
	// генерация графа по распределению
	this->vertices_num = distribution.size();
	adjacency_m = {};
	adjacency_m.resize(vertices_num, std::vector<bool>(vertices_num, 0));
	edges_num = 0;
	
	generareAdjacencyMatrix(distribution);
	generareWeightMatrix();
	reachability_m = {};
}

void graph::generareAdjacencyMatrix(const std::vector<int>& distribution) {
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

void graph::generareWeightMatrix() {
	weight_m = {};
	weight_m.resize(adjacency_m.size(), std::vector<int>(adjacency_m.size(), 0));

	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = i; j < adjacency_m.size(); j++) {
			if (adjacency_m[i][j] == 1) {
				weight_m[i][j] = rand() % (max_weight - 1) + 1;
			}
		}
	}
}

std::vector<std::vector<int>> graph::ShimbellMethod(const int& edges_num, const bool& flag) {
	// Метод поиска максимального/минимального пути между вершинами длиной в edges_num ребер \
	flag = 1 - максимального, flag = 0 - минимального; по умолчанию поиск минимального

	if (edges_num == 0) {
		std::vector<std::vector<int>> result;
		result.resize(adjacency_m.size(), std::vector<int>(adjacency_m.size(), 0));
		return result;
	}

	std::vector<std::vector<int>> shimbell_m = weight_m;
	std::vector<std::vector<int>> result = shimbell_m;

	for (int _ = 0; _ < edges_num - 1; _++) {
		for (int i = 0; i < vertices_num; i++) {
			for (int j = 0; j < vertices_num; j++) {
				result[i][j] = 0;
				for (int k = 0; k < vertices_num; k++) {
					int summ = 0;
					if (shimbell_m[i][k] != 0 and weight_m[k][j] != 0) 
						summ = shimbell_m[i][k] + weight_m[k][j];
					
					if (flag) result[i][j] = std::max(summ, result[i][j]);
					else { 
						if (summ != 0 and result[i][j] != 0)
							result[i][j] = std::min(summ, result[i][j]); 
						if (summ != 0 and result[i][j] == 0)
							result[i][j] = summ;
					}
				}
			}
		}
		shimbell_m = result;
	}

	return shimbell_m;
}

int graph::reachabilityCheck(const int& a, const int& b) {
	// функция проверки достижимости из вершины с номером a в вершину с номером b
	// result: количество таких маршрутов

	if (reachability_m.size() != 0)
		return reachability_m[a][b];

	// создание матрицы достижимости
	std::vector<std::vector<int>> result = {}; // результат матричного умножения
	result.resize(vertices_num, std::vector<int>(vertices_num, 0));

	std::vector<std::vector<int>> degree = result; // очередная степень матрицы

	for (int i = 0; i < vertices_num; i++) {
		for (int j = 0; j < vertices_num; j++) {
			degree[i][j] = adjacency_m[i][j];
		}
	}
	reachability_m = degree; // сразу добавляем первую степень

	for (int _ = 0; _ < vertices_num - 1; _++) {
		for (int i = 0; i < vertices_num; i++) {
			for (int j = 0; j < vertices_num; j++) {
				result[i][j] = 0;
				for (int k = 0; k < vertices_num; k++) {
					result[i][j] += degree[i][k] * adjacency_m[k][j];
				}
			}
		}
		degree = result; // новая степень 

		// добавление новой степени в матрицу достижимости
		for (int i = 0; i < vertices_num; i++) {
			for (int j = 0; j < vertices_num; j++) {
				reachability_m[i][j] += degree[i][j];
			}
		}
	}

	// добавление единичной матрицы
	for (int i = 0; i < vertices_num; i++) {
		reachability_m[i][i] += 1;
	}

	return reachability_m[a][b];
}


void graph::printAdjacencyMatrix() {
	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = 0; j < adjacency_m.size(); j++) {
			std::cout << adjacency_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

void graph::printWeightMatrix() {
	for (int i = 0; i < weight_m.size(); i++) {
		for (int j = 0; j < weight_m.size(); j++) {
			std::cout << weight_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
