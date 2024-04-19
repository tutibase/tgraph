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


void UnorientedGraph::Prim() {
// Поиск минимального остовного дерева с помощью алгоритма Прима
	int count_iter = 0;
	std::vector<int> predecessors(vertices_num); // предшетсвенники вершин в остове

	// минимальный вес ребра, с помощью которого попали в вершину
	std::vector<int> distances(vertices_num, INT_MAX);

	// включена ли вершина в остов
	std::vector<bool> mstSet(vertices_num, false);

	// Начнем с нулевой вершины
	distances[0] = 0;
	predecessors[0] = -1; // Первая вершина всегда корень остова

	for (int _ = 0; _ < vertices_num - 1; _++) {
		// среди ещё не попавших в остов вершин выбираем такую, расстояние до которой сейчас минимально из вершин остова
		int u = findMinDist(distances, mstSet);

		// Добавление выбранной вершины в остов
		mstSet[u] = true;

		// Обновление значения distances и predecessors
		for (int v = 0; v < vertices_num; v++) {
			// distances[v] == INT_MAX для вершин, которые еще не добавлены в остов
			// Обновляется только если weight_m[u][v] меньше distances[v] (новое расстояние меньше старого)
			if (weight_m[u][v] && mstSet[v] == false && weight_m[u][v] < distances[v]) {
				predecessors[v] = u;
				distances[v] = weight_m[u][v];
			}
			count_iter++;
		}
	}

	for (int i = 0; i < predecessors.size(); i++) {
		if (predecessors[i] == 0) {
			//predecessors[0] = i;
			break;
		}
	}
	predecessors_Prim = predecessors;

	// выводим построенное MST
	printMST(predecessors);
	std::cout << "\nIterations num: " << count_iter;
}

int UnorientedGraph::findMinDist(std::vector<int>& distances, std::vector<bool>& mstSet) {
	int min = INT_MAX;
	int min_index = -1;

	for (int v = 0; v < vertices_num; v++)
		if (mstSet[v] == false && distances[v] < min) {
			min = distances[v];
			min_index = v;
		}

	return min_index;
}

void UnorientedGraph::printMST(std::vector<int>& predecessors) {
	int total_weight = 0;
	std::cout << "Minimum spanning tree:\n" << "Edge \tWeight\n";
	for (int i = 1; i < vertices_num; i++) {
		std::cout << predecessors[i] << " - " << i << " \t" << weight_m[i][predecessors[i]] << " \n";
		total_weight += weight_m[i][predecessors[i]];
	}
		
	std::cout << "Total weight of Minimum Spanning Tree is " << total_weight << std::endl;
}


void UnorientedGraph::Boruvka() {
	// алгоритм Борувки для поиска минимального остовного дерева
	int count_iter = 0;
	std::vector<int> parent(vertices_num), rank(vertices_num, 0), cheapest(vertices_num, -1);
	std::iota(parent.begin(), parent.end(), 0); // Инициализируем parent[i] = i

	int numTrees = vertices_num; // изначально каждая вершина - это отдельное дерево
	int MST_weight = 0; // Вес остова

	// Продолжаем объединять компоненты, пока не останется только одна компонента
	while (numTrees > 1) {
		// Инициализация cheapest каждый раз новым поиском
		std::fill(cheapest.begin(), cheapest.end(), -1);

		// Ищем наименьшее ребро для каждой компоненты
		for (int u = 0; u < vertices_num; u++) {
			for (int v = 0; v < vertices_num; v++) {
				if (find(u, parent) != find(v, parent) && adjacency_m[u][v]) { // выбираем только те рёбра, которые соединяют разные компоненты
					count_iter++;
					if (cheapest[find(u, parent)] == -1 or 
						weight_m[u][v] < weight_m[cheapest[find(u, parent)] / vertices_num][cheapest[find(u, parent)] % vertices_num]) 
					{
						cheapest[find(u, parent)] = u * vertices_num + v;
					}
				}
			}
		}

		// Для каждой компоненты добавляем найденное ребро в MST
		for (int i = 0; i < vertices_num; i++) {
			if (cheapest[i] != -1) {
				int u = cheapest[i] / vertices_num;
				int v = cheapest[i] % vertices_num;
				int set_u = find(u, parent);
				int set_v = find(v, parent);

				if (set_u != set_v) {
					MST_weight += weight_m[u][v];
					std::cout << u << " - " << v << " \t" << weight_m[u][v] << " \n";
					union_sets(set_u, set_v, parent, rank);
					numTrees--;
				}
			}
		}
	}

	std::cout << "Total weight of Minimum Spanning Tree is " << MST_weight << std::endl;
	std::cout << "\nIterations num: " << count_iter;
}


int UnorientedGraph::find(int u, std::vector<int>& parent) {
	if (parent[u] != u)
		parent[u] = find(parent[u], parent); // Путь сжатия
	return parent[u];
}

void UnorientedGraph::union_sets(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
	u = find(u, parent);
	v = find(v, parent);

	if (u != v) {
		if (rank[u] < rank[v])
			parent[u] = v;
		else if (rank[u] > rank[v])
			parent[v] = u;
		else {
			parent[v] = u;
			rank[u]++;
		}
	}
}


void UnorientedGraph::generatePruferCode(const std::vector<int>& predecessors) {
	// Генерация кода Прюфера для хранения остова
	int n = predecessors.size();  // Получаем количество вершин в остове

	std::vector<int> vertex_degree(n, 0);  // вектор для хранения степеней вершин

	// Вычисляем степени всех вершин
	for (int i = 1; i < n; i++) {
			vertex_degree[i]++;  // Увеличиваем степень текущей вершины
			vertex_degree[predecessors[i]]++;  // Увеличиваем степень родителя текущей вершины
	}

	std::vector<std::pair<int, int>> edges;
	for (int i = 1; i < vertices_num; i++) {
		edges.push_back(std::pair<int, int>(std::min(predecessors[i], i), std::max(predecessors[i], i)));
	}

	int ptr = 0; // Указатель на вершину с минимальным номером и степенью 1
	while (vertex_degree[ptr] != 1) {
		ptr++;
	}

	int leaf = ptr; // текущий лист дерева (сначала самый маленький по номеру)
	std::vector<int> prufer_code; // Вектор для сохранения кода Прюфера
	std::vector<int> prufer_code_weights; // Вектор для сохранения весов

	// Генерируем код 
	for (int i = 0; i < n - 2; i++) {
		int next = -1; // next — родитель текущего листа
		for (int j = 0; j < edges.size(); j++) {
			if (edges[j].first == leaf) {
				next = edges[j].second;
				edges.erase(edges.begin() + j);
				break;
			}
			if (edges[j].second == leaf) {
				next = edges[j].first;
				edges.erase(edges.begin() + j);
				break;
			}
		}

		prufer_code.push_back(next); // Добавляем родителя текущего листа в код Прюфера
		prufer_code_weights.push_back(weight_m[next][leaf]); // добавляем вес 
		vertex_degree[leaf]--;  // Уменьшаем степень листа
		vertex_degree[next]--;  // Уменьшаем степень родителя

		// ищем новый лист с минимальным номером, пропускаем вершины, которые не являются листьями
		ptr = 0;
		while (ptr < n and vertex_degree[ptr] != 1) {
			ptr++;
		}

		leaf = ptr; // Новый лист для следующего шага
	}

	// сохраняем код
	int next = -1;
	for (int j = 0; j < edges.size(); j++) {
		if (edges[j].first == leaf) {
			next = edges[j].second;
			break;
		}
		if (edges[j].second == leaf) {
			next = edges[j].first;
			break;
		}
	}
	prufer_code_weights.push_back(weight_m[next][leaf]); // добавляем вес 

	this->prufer_code = prufer_code;
	this->prufer_code_weights = prufer_code_weights;

	// Выводим код Прюфера
	std::cout << "Prufer Code:\n";
	for (int i = 0; i < prufer_code.size(); i++) {
		std::cout << '(' << prufer_code[i] << "; " << prufer_code_weights[i] << ")\n"; // Вывод каждого элемента кода Прюфера
	}
	std::cout << "\n";
}


void UnorientedGraph::decodePruferCode() {
	std::vector<std::vector<int>> matrix = {};
	matrix.resize(weight_m.size(), std::vector<int>(weight_m.size(), 0));

	std::vector<bool> vertices(vertices_num, 0);
	int size = prufer_code.size();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			if (vertices[j] or isInVec(prufer_code, j)) continue;
			matrix[j][prufer_code[0]] = prufer_code_weights[i];
			matrix[prufer_code[0]][j] = matrix[j][prufer_code[0]];

			vertices[j] = 1;
			prufer_code.erase(prufer_code.begin());
			break;
		}
	}

	// добавление последнего ребра
	std::pair<int, int> last_edge(-1, -1);
	for (int i = 0; i < vertices_num; i++) {
		if (vertices[i] == 0) {
			if (last_edge.first == -1)
				last_edge.first = i;
			else
				last_edge.second = i;
		}
	}

	matrix[last_edge.first][last_edge.second] = prufer_code_weights[prufer_code_weights.size() - 1];
	matrix[last_edge.first][last_edge.second] = matrix[last_edge.second][last_edge.first];

	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix.size(); j++) {
			std::cout << std::right << std::setw(width) << matrix[i][j] << ' ';
		}
		std::cout << '\n';
	}
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

bool isInVec(const std::vector<int>& vec, int item) {
	return std::find(vec.begin(), vec.end(), item) != vec.end();
}