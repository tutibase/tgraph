#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

class negative_hypergeometric {
public:
	negative_hypergeometric(int size);
	void generate();
	void printDistribution();
	const std::vector<int>& getDistribution() { return distribution; };

private:
	int m, M, N, size;
	std::vector<int> distribution;

};

int intInput(const char* msg, int maxi, int mini = 0);
void help();