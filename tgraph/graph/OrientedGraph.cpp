#include "OrientedGraph.h"

OrientedGraph::OrientedGraph(const int& size) {
	// генерация пустого графа размера size
	this->vertices_num = size;
	adjacency_m = {};
	adjacency_m.resize(size, std::vector<bool>(size, 0));
	edges_num = 0;

	generateWeightMatrix();
	reachability_m = {};

	generateBandwidthMatrix();
}

OrientedGraph::OrientedGraph(const std::vector<int>& distribution) {
	// генерация графа по распределению
	this->vertices_num = distribution.size();
	adjacency_m = {};
	adjacency_m.resize(vertices_num, std::vector<bool>(vertices_num, 0));
	edges_num = 0;
	
	generateAdjacencyMatrix(distribution);
	generateWeightMatrix();
	reachability_m = {};

	generateBandwidthMatrix();
}

void OrientedGraph::generateAdjacencyMatrix(const std::vector<int>& distribution) {
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

void OrientedGraph::generateWeightMatrix(bool add_negative) {
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
	generateCostMatrix();
}

void OrientedGraph::generateCostMatrix() {
	// генерация матрицы стоимостей на основе весовой
	cost_m = {};
	cost_m.resize(weight_m.size(), std::vector<int>(weight_m.size(), 0));
	for (int i = 0; i < weight_m.size(); i++) {
		for (int j = 0; j < weight_m.size(); j++) {
			cost_m[i][j] = abs(weight_m[i][j]);
		}
	}
}

void OrientedGraph::generateBandwidthMatrix() {
	// генерация матрицы пропускных способностей
	bandwidth_m = {};
	bandwidth_m.resize(adjacency_m.size(), std::vector<int>(adjacency_m.size(), 0));
	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = 0; j < adjacency_m.size(); j++) {
			if (adjacency_m[i][j] != 0)
				bandwidth_m[i][j] = rand() % (max_weight - 1) + 1;
		}
	}
}

std::vector<std::vector<int>> OrientedGraph::ShimbellMethod(const int& edges_num, const bool& flag) {
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

int OrientedGraph::reachabilityCheck(const int& a, const int& b) {
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

void OrientedGraph::Dijkstra(int start_v, int end_v) {
	// алгоритм Дейкстры (поиск пути минимальной длины между вершинами)
	// метод печатает путь от start_v до end_v и вектор расстояний от start_v до всех вершин
	if (negative_weights == 1) {
		std::cout << "The algorithm doesn't work with negative values";
		return;
	}
	// Инициализация
	std::vector<int> distance(vertices_num, INT_MAX); // расстояния до вершин
	std::vector<bool> visited(vertices_num, false); // посещенные вершины
	std::vector<int> predecessor(vertices_num, -1); // Хранит предшественников вершин

	distance[start_v] = 0; // расстояние до начальной вершины равно 0

	std::vector<int> path; // путь от начальной вершины до текущей

	while (!visited[end_v]) {
		// Найти не посещенную вершину с минимальным расстоянием
		int min_distance = INT_MAX;
		int min_distance_v = -1;
		for (int i = 0; i < vertices_num; i++) {
			if (!visited[i] && distance[i] < min_distance) {
				min_distance = distance[i];
				min_distance_v = i;
			}
		}

		// Если все вершины посещены, но конечная вершина не достигнута, выход из цикла
		if (min_distance_v == -1) {
			break;
		}

		// Обновить расстояния до достижимых вершин
		for (int i = 0; i < vertices_num; i++) {
			if (weight_m[min_distance_v][i] != 0) {
				int new_distance = distance[min_distance_v] + weight_m[min_distance_v][i];
				if (new_distance < distance[i]) {
					distance[i] = new_distance;
					predecessor[i] = min_distance_v; // Обновить предшественника
				}
			}
		}

		// Пометить вершину как посещенную
		visited[min_distance_v] = true;
	}

	// Восстановить кратчайший путь
	if (visited[end_v]) {
		int current_v = end_v;
		while (current_v != -1) {
			path.push_back(current_v);
			current_v = predecessor[current_v];
		}
		std::reverse(path.begin(), path.end()); // Изменить порядок вершин на правильный
	}
	else {
		path = std::vector<int>();
	}

	// вывод
	std::cout << "\nDistances: ";
	for (int d : distance) {
		if (d != INT_MAX)
			std::cout << d << " ";
		else
			std::cout << "inf ";
	}
	std::cout << std::endl;

	if (path.empty()) {
		std::cout << "Path was not found" << std::endl;
	}
	else {
		std::cout << "Way: ";
		for (int i = 0; i < path.size(); i++) {
			std::cout << path[i];
			if (i != path.size() - 1) std::cout << " -> ";
		}
	}
}


bool OrientedGraph::BellmanFord(int start_v, int end_v, bool find_max_path) {
	// алгоритм Беллмана-Форда
	// если find_max_path == 1, то ищем максимальный путь, иначе (по умолчанию) минимальный
	// метод печатает путь от start_v до end_v и вектор расстояний от start_v до всех вершин
	int inf;
	if (find_max_path) inf = INT_MIN;
	else inf = INT_MAX;

	std::vector<int>distances = std::vector<int>(vertices_num, inf); // расстояния до вершин
	std::vector<int> predecessors = std::vector<int>(vertices_num, -1);	// Хранит предшественников вершин
	
	distances[start_v] = 0;

	// Основной цикл: (V-1) раз пройтись по всем рёбрам
	for (int i = 0; i < vertices_num - 1; i++) {
		for (int u = 0; u < vertices_num; u++) {
			for (int v = 0; v < vertices_num; v++) {
				if (weight_m[u][v] != 0) {
					bool comparison = distances[u] + weight_m[u][v] < distances[v];
					if (find_max_path) comparison = !comparison;

					if (distances[u] != inf && comparison) {
						distances[v] = distances[u] + weight_m[u][v];
						predecessors[v] = u;
					}
				}
			}
		}
	}

	std::vector<int> path = get_path(start_v, end_v, predecessors);
	// вывод
	std::cout << "\nDistances: ";
	for (int d : distances) {
		if (d != inf)
			std::cout << d << " ";
		else
			std::cout << "inf ";
	}
	std::cout << std::endl;

	if (path.empty()) {
		std::cout << "Path was not found" << std::endl;
	}
	else {
		std::cout << "Way: ";
		for (int i = 0; i < path.size(); i++) {
			std::cout << path[i];
			if (i != path.size() - 1) std::cout << " -> ";
		}
		std::cout << std::endl;
	}

	// Проверка отрицательных циклов
	for (int u = 0; u < vertices_num; u++) {
		for (int v = 0; v < vertices_num; v++) {
			if (weight_m[u][v] != 0) {
				if (distances[u] != inf && distances[u] + weight_m[u][v] < distances[v]) {
					return false; // Есть отрицательный цикл
				}
			}
		}
	}

	return true; // Отрицательных циклов нет
}

std::vector<int> OrientedGraph::BellmanFordPath(int start_v, int end_v, const std::vector<std::vector<int>>& cost_m_copy) {
	// алгоритм Беллмана-Форда, модифицированный только на поиск пути минимальной стоимости в подходящем графе
	// метод вернет вектор номеров вершин минимального пути в графе
	int inf = INT_MAX;

	std::vector<int>distances = std::vector<int>(vertices_num, inf); // расстояния до вершин
	std::vector<int> predecessors = std::vector<int>(vertices_num, -1);	// предшественники вершин в минимальном пути до них

	distances[start_v] = 0;

	// Основной цикл: (V-1) раз пройтись по всем рёбрам
	for (int i = 0; i < vertices_num - 1; i++) {
		for (int u = 0; u < vertices_num; u++) {
			for (int v = 0; v < vertices_num; v++) {
				if (cost_m_copy[u][v] != 0) {
					bool comparison = distances[u] + cost_m_copy[u][v] < distances[v];

					if (distances[u] != inf && comparison) {
						distances[v] = distances[u] + cost_m_copy[u][v];
						predecessors[v] = u;
					}
				}
			}
		}
	}

	return get_path(start_v, end_v, predecessors);
}

// Метод для поиска пути с помощью обхода в глубину (DFS)
bool OrientedGraph::dfs(int start_v, int end_v, std::vector<int>& predecessors, const std::vector<std::vector<int>>& bandwidth_m_copy) {
	std::vector<bool> visited(vertices_num, false);
	std::stack<int> stack;
	stack.push(start_v);
	while (!stack.empty()) {
		int u = stack.top();
		stack.pop();
		if (!visited[u]) {
			visited[u] = true;
			for (int v = 0; v < vertices_num; v++) {
				if (!visited[v] && bandwidth_m_copy[u][v] > 0) {
					predecessors[v] = u; // обновляем предшественника вершины
					stack.push(v);
					if (v == end_v) // Если мы достигли стока, путь найден
						return true;
				}
			}
		}
	}
	return false;
}


// Алгоритм Форда-Фалкерсона для нахождения максимального потока
int OrientedGraph::FordFulkerson(int start_v, int end_v) {
	std::vector<std::vector<int>> bandwidth_m_copy = bandwidth_m;
	std::vector<int> predecessors(vertices_num); // массив для хранения пути
	int max_flow = 0;

	// Пока существует путь от истока к стоку
	while (dfs(start_v, end_v, predecessors, bandwidth_m_copy)) {
		int path_flow = INT_MAX;

		// Найти минимальную пропускную способность в пути
		for (int v = end_v; v != start_v; v = predecessors[v]) {
			int u = predecessors[v];
			path_flow = std::min(path_flow, bandwidth_m_copy[u][v]);
		}

		// Обновить пропускные способности и обратные ребра в пути
		for (int v = end_v; v != start_v; v = predecessors[v]) {
			int u = predecessors[v];
			bandwidth_m_copy[u][v] -= path_flow;
			bandwidth_m_copy[v][u] += path_flow;
		}

		// Добавить поток пути к общему максимальному потоку
		max_flow += path_flow;
	}

	return max_flow;
}

int OrientedGraph::minCostFlow() {
	// поиск потока минимальной стоимости, составляющим 2/3 от максимального потока
	int max_flow = 2 / 3. * (FordFulkerson(0, getVerticesNum() - 1));
	if (max_flow == 0) return 0;

	int cost_flow = 0;
	std::vector<std::vector<int>> bandwidth_m_copy = bandwidth_m;
	std::vector<std::vector<int>> cost_m_copy = cost_m;
	
	// смотрим, какой путь стоит минимально
	std::vector<int> min_path = BellmanFordPath(0, vertices_num - 1, cost_m_copy);
	// пока существует путь и мы не разгрузили весь необходимый поток
	while (!min_path.empty() and max_flow != 0) {
		int path_flow = INT_MAX;
		int path_cost = 0;
		std::cout << "\n\current path: ";
		for (int i = 0; i < min_path.size(); i++) {
			std::cout << min_path[i];
			if (i != min_path.size() - 1) std::cout << " -> ";
		}
		
		// поиск стоимости пути и потока пути
		for (int i = 0; i < min_path.size() - 1; i++) {
			path_flow = std::min(path_flow, bandwidth_m_copy[min_path[i]][min_path[i + 1]]);
			path_cost += cost_m_copy[min_path[i]][min_path[i + 1]];
		}
		std::cout << "\npath cost: " << path_cost;
		
		int counter = 0; // счетчик потока по данному пути
		while (path_flow != 0 and max_flow != 0) {
			for (int i = 0; i < min_path.size() - 1; i++) {
				bandwidth_m_copy[min_path[i]][min_path[i + 1]]--;
				// если поток через ребро заполнен, то удаляем ребро
				if (bandwidth_m_copy[min_path[i]][min_path[i + 1]] == 0)
					cost_m_copy[min_path[i]][min_path[i + 1]] = 0;
			}
			// добавляем к общей стоимости стоимость потока
			cost_flow += path_cost;
			// теперь осталось разгрузить на 1 единицу потока меньше
			max_flow--;
			// через данный путь можно разгрузить на 1 единицу потока меньше
			path_flow--;
			counter++;
		}
		// смотрим, какой путь из оставшихся стоит минимально
		std::cout << "\nflow along the way: " << counter;
		min_path = BellmanFordPath(0, vertices_num - 1, cost_m_copy);
	}
	return cost_flow;
}


std::vector<int> OrientedGraph::get_path(int start_v, int end_v, const std::vector<int>& predecessors) {
	// поиск пути из одной вершины в другую с учетом того, какой вершине какая в этом пути предшествовала 
	std::vector<int> path;
	if (predecessors[end_v] == -1) {
		return path; // Путь не существует
	}

	// Восстановление пути от конечной вершины к начальной
	for (int v = end_v; v != start_v; v = predecessors[v]) {
		path.push_back(v);
	}
	path.push_back(start_v);

	// Разворот пути в правильном порядке
	std::reverse(path.begin(), path.end());
	return path;
}

void OrientedGraph::printAdjacencyMatrix() {
	for (int i = 0; i < adjacency_m.size(); i++) {
		for (int j = 0; j < adjacency_m.size(); j++) {
			std::cout << adjacency_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

void OrientedGraph::printWeightMatrix() {
	for (int i = 0; i < weight_m.size(); i++) {
		for (int j = 0; j < weight_m.size(); j++) {
			std::cout << std::right << std::setw(width) << weight_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

void OrientedGraph::printBandwidthMatrix() {
	for (int i = 0; i < bandwidth_m.size(); i++) {
		for (int j = 0; j < bandwidth_m.size(); j++) {
			std::cout << std::right << std::setw(width) << bandwidth_m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
