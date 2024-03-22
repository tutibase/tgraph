#include "distribution.h"

negative_hypergeometric::negative_hypergeometric(int size) {
	// сделать проверку на неотрицательность m, M, N и m <= M <= N
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
