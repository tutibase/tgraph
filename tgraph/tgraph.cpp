#include <iostream>
#include "distribution.h"
#include "graph.h"

int main() {
    srand(time(NULL));

    int size = 15;

    negative_hypergeometric distribution(size);
    distribution.generate();

    for (int i = 0; i < distribution.getDistribution().size(); i++) {
        std::cout << distribution.getDistribution()[i] << ' ';
    }

    graph myGraph(distribution.getDistribution());

    std::cout << "\n\n";
    myGraph.printAdjacencyMatrix();

    return 0;
}
