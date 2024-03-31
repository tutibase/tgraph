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

void graph::generareWeightMatrix(bool add_negative) {
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

void graph::Dijkstra(int start_node, int end_node) {
	if (negative_weights == 1) {
		std::cout << "The algorithm doesn't work with negative values";
		return;
	}
	// Инициализация
	std::vector<int> distance(vertices_num, INT_MAX); // расстояния до вершин
	std::vector<bool> visited(vertices_num, false); // посещенные вершины
	std::vector<int> predecessor(vertices_num, -1); // Хранит предшественников вершин

	distance[start_node] = 0; // расстояние до начальной вершины равно 0

	std::vector<int> path; // путь от начальной вершины до текущей

	while (!visited[end_node]) {
		// Найти не посещенную вершину с минимальным расстоянием
		int min_distance = INT_MAX;
		int min_distance_node = -1;
		for (int i = 0; i < vertices_num; i++) {
			if (!visited[i] && distance[i] < min_distance) {
				min_distance = distance[i];
				min_distance_node = i;
			}
		}

		// Если все вершины посещены, но конечная вершина не достигнута, выход из цикла
		if (min_distance_node == -1) {
			break;
		}

		// Обновить расстояния до достижимых вершин
		for (int i = 0; i < vertices_num; i++) {
			if (weight_m[min_distance_node][i] != 0) {
				int new_distance = distance[min_distance_node] + weight_m[min_distance_node][i];
				if (new_distance < distance[i]) {
					distance[i] = new_distance;
					predecessor[i] = min_distance_node; // Обновить предшественника
				}
			}
		}

		// Пометить вершину как посещенную
		visited[min_distance_node] = true;
	}

	// Восстановить кратчайший путь
	if (visited[end_node]) {
		int current_node = end_node;
		while (current_node != -1) {
			path.push_back(current_node);
			current_node = predecessor[current_node];
		}
		std::reverse(path.begin(), path.end()); // Изменить порядок вершин на правильный
	}
	else {
		path = std::vector<int>();
	}

	// вывод
	if (path.empty()) {
		std::cout << "The path was not found" << std::endl;
	}
	else {
		std::cout << "Shortest way has a length of " << distance[end_node] << ". Way: ";
		for (int i = 0; i < path.size(); i++) {
			std::cout << path[i];
			if (i != path.size() - 1) std::cout << " -> ";
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

void graph::printWeightMatrix() {
	for (int i = 0; i < weight_m.size(); i++) {
		for (int j = 0; j < weight_m.size(); j++) {
			std::cout << std::right << std::setw(width) << weight_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
