#include "distribution.h"

negative_hypergeometric::negative_hypergeometric(int size) {
	distribution = {};

	// (N-M) - верхняя граница; чем ближе m к M, тем ближе значения к верхней границе
	N = size * 3;
	M = N - size; //size * 2 
	m = M * 0.3;
	this->size = size;
}

void negative_hypergeometric::generate() {
	distribution = {};

	for (int j = 0; j < size; j++) {
		int m_copy = m;
		int M_copy = M;
		int N_copy = N;

		int x = 0;
		int i = 0;

		do {
			float p = float(M_copy) / float(N_copy);
			float r = (rand() % 100) / 100.;

			if (r < p) {
				i++;
				N_copy--;
				M_copy--;
			}
			else {
				x++;
				N_copy--;
			}
		} while (i < m_copy);

		distribution.push_back(x);
	}
	std::sort(distribution.begin(), distribution.end(), std::greater<int>());
}

void negative_hypergeometric::printDistribution() {
	std::cout << '(';
	for (int i = 0; i < distribution.size(); i++) {
		std::cout << distribution[i];
		if (i != distribution.size() - 1) std::cout << ", ";
	}
	std::cout << ')';
}

int intInput(const char* msg, int maxi, int mini) {
	std::cout << msg;
	int val;
	while (true) {
		std::cin >> val;
		if (std::cin.peek() != '\n' or val > maxi or val < mini) {
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			printf("\nInvalid value, repeat the input\n");
		}
		else break;
	}
	return val;
}
