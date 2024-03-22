#pragma once
#include <vector>
#include <algorithm>

class negative_hypergeometric {
public:
	negative_hypergeometric(int size);
	void generate();
	const std::vector<int>& getDistribution() { return distribution; };

private:
	int m, M, N, size;
	std::vector<int> distribution;

};
