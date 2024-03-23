#include <iostream>
#include "distribution.h"
#include "graph.h"

int main() {
    srand(time(NULL));

    //int size = intInput("Enter the number of vertices of the graph: ", 15);
    int size = 5;

    // generate distribution
    negative_hypergeometric distribution(size);
    distribution.generate();
    std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
    distribution.printDistribution();

    // generate graph
    graph myGraph(distribution.getDistribution());

    //myGraph.weight_m = { {0,3,1,5,0}, {0,0,0,0,2}, {0,1,0,0,4}, {0,0,2,0,4}, {0,0,0,0,0} };
  
    // menu
    int i = 1;
    bool flag = 1;
    while (flag) {
        switch (i) {
        case 0:
            flag = 0;
            break;

        case 1:
            help();
            break;

        case 2:
            myGraph.printAdjacencyMatrix();
            break;

        case 3:
            myGraph.printWeightMatrix();
            break;

        case 4:
            myGraph.ShimbellMethod(intInput("Enter the number of edges: ", myGraph.getVerticesNum() - 1), 0);
            break;

        default:
            break;
        }

        if (flag) i = intInput("\nEnter a number [0; 4]\n", 4);
    }

    return 0;
}
