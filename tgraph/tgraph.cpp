#include <iostream>
#include "distribution/distribution.h"
#include "graph/graph.h"

int main() {
    srand(time(NULL));

    int size = intInput("Enter the number of vertices of the graph: ", 100);
    //int size = 5;

    // generate distribution
    negative_hypergeometric distribution(size);
    distribution.generate();
    std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
    distribution.printDistribution();

    // generate graph
    graph myGraph(distribution.getDistribution());

    //myGraph.weight_m = { {0,3,1,5,0}, {0,0,0,0,2}, {0,1,0,0,4}, {0,0,2,0,4}, {0,0,0,0,0} };
    //myGraph.adjacency_m = { {0,1,1,1,0}, {0,0,0,0,1}, {0,1,0,0,1}, {0,0,1,0,1}, {0,0,0,0,0} };
  
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
        {
            bool flag = int(intInput("[0] - min route, [1] - max route: ", 1));
            std::vector<std::vector<int>> shimbell_m = myGraph.ShimbellMethod(intInput("\nEnter the number of edges: ",
                                                                              myGraph.getVerticesNum() - 1), flag);

            for (int i = 0; i < shimbell_m.size(); i++) {
                for (int j = 0; j < shimbell_m.size(); j++) {
                    std::cout << std::left << std::setw(width) << shimbell_m[i][j] << ' ';
                }
                std::cout << '\n';
            }
        }
            break;

        case 5:
        {
            int a = intInput("1st vertex: ", myGraph.getVerticesNum());
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum());
            int num = myGraph.reachabilityCheck(a, b);
            if (num == 0)
                std::cout << "\nRoute is impossible\n";
            else
                std::cout << "\n" << num << " routes between these vertices\n";
        }
            break;

        case 6:
            myGraph.generareWeightMatrix(intInput("Add negative weights?\n[0] - no, [1] - yes: ", 1));
            break;

        case 7:
        {
            int a = intInput("1st vertex: ", myGraph.getVerticesNum());
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum());
            myGraph.Dijkstra(a, b);
        }
            break;

        case 8:
        {
            int a = intInput("1st vertex: ", myGraph.getVerticesNum());
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum());
            myGraph.BellmanFord(a, b);
        }
        break;

        case 10:
            size = intInput("Enter the number of vertices of the graph: ", 15);
            distribution = negative_hypergeometric(size);
            distribution.generate();
            std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
            distribution.printDistribution();

            myGraph = graph(distribution.getDistribution());
            break;

        default:
            break;
        }

        if (flag) i = intInput("\nEnter a number\n", 10);
    }

    return 0;
}
