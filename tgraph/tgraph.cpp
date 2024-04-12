#include <iostream>
#include "distribution/distribution.h"
#include "graph/OrientedGraph.h"
#include "graph/UnorientedGraph.h"

int main() {
    srand(time(NULL));

    int size = intInput("Enter the number of vertices of the graph: ", 100);
    //int size = 5;

    // generate distribution
    negative_hypergeometric distribution(size);
    distribution.generate();
    std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
    distribution.printDistribution();

    // generate graphs
    OrientedGraph myGraph(distribution.getDistribution());
    UnorientedGraph myUnorientedGraph(distribution.getDistribution());


    /*myGraph.weight_m = { {0,3,1,5,0}, {0,0,0,0,2}, {0,1,0,0,4}, {0,0,2,0,4}, {0,0,0,0,0} };
    myGraph.cost_m = { {0,3,1,5,0}, {0,0,0,0,2}, {0,1,0,0,4}, {0,0,2,0,4}, {0,0,0,0,0} };
    myGraph.adjacency_m = { {0,1,1,1,0}, {0,0,0,0,1}, {0,1,0,0,1}, {0,0,1,0,1}, {0,0,0,0,0} };
    myGraph.bandwidth_m = { {0,3,1,5,0}, {0,0,0,0,2}, {0,1,0,0,4}, {0,0,2,0,4}, {0,0,0,0,0} };*/
  
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
            int a = intInput("1st vertex: ", myGraph.getVerticesNum() - 1);
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum() - 1);
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
            int a = intInput("1st vertex: ", myGraph.getVerticesNum() - 1);
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum() - 1);
            myGraph.Dijkstra(a, b);
        }
            break;

        case 8:
        {
            int a = intInput("1st vertex: ", myGraph.getVerticesNum() - 1);
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum() - 1);
            myGraph.BellmanFord(a, b);
        }
        break;

        case 9:
        {
            int a = intInput("1st vertex: ", myGraph.getVerticesNum() - 1);
            int b = intInput("2nd vertex: ", myGraph.getVerticesNum() - 1);
            myGraph.BellmanFord(a, b, 1);
        }
        break;

        case 10:
            size = intInput("Enter the number of vertices of the graph: ", 100);
            distribution = negative_hypergeometric(size);
            distribution.generate();
            std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
            distribution.printDistribution();

            myGraph = OrientedGraph(distribution.getDistribution());
            break;

        case 11:
            std::cout << "Max flow: " << myGraph.FordFulkerson(0, myGraph.getVerticesNum() - 1) << std::endl;
            break;

        case 12:
            myGraph.printBandwidthMatrix();
            break;

        case 13:
            std::cout << "Min cost flow: " << myGraph.minCostFlow() << std::endl;
            break;

        case 14:
            myUnorientedGraph.printAdjacencyMatrix();
            break;

        case 15:
            myUnorientedGraph.printWeightMatrix();
            break;

        case 16:
            myUnorientedGraph.generareWeightMatrix(intInput("Add negative weights?\n[0] - no, [1] - yes: ", 1));
            break;

        case 17:
            size = intInput("Enter the number of vertices of the graph: ", 100);
            distribution = negative_hypergeometric(size);
            distribution.generate();
            std::cout << "Preliminary distribution of the degrees of the vertices of the graph:\n";
            distribution.printDistribution();

            myUnorientedGraph = UnorientedGraph(distribution.getDistribution());
            break;

        default:
            break;
        }

        if (flag) i = intInput("\nEnter a number\n", 17);
    }

    return 0;
}

void help() {
    std::cout << "\n\n[0] - exit" << '\n';
    std::cout << "[1] - help" << '\n';
    std::cout << "[2] - print adjacency matrix" << '\n';
    std::cout << "[3] - print weight matrix" << '\n';
    std::cout << "[4] - Shimbell method" << '\n';
    std::cout << "[5] - check reachability" << '\n';
    std::cout << "[6] - regenerate weight matrix" << '\n';
    std::cout << "[7] - Dijkstra's algorithm" << '\n';
    std::cout << "[8] - Bellman-Ford algorithm" << '\n';
    std::cout << "[9] - Maximum path" << '\n';
    std::cout << "[10] - regenerate graph" << '\n';
    std::cout << "[11] - Ford-Fulkerson algorithm" << '\n';
    std::cout << "[12] - print bandwidth matrix" << '\n';
    std::cout << "\nUnoriented graph:\n";
    std::cout << "[14] - print adjacency matrix" << '\n';
    std::cout << "[15] - print weight matrix" << '\n';
    std::cout << "[16] - regenerate weight matrix" << '\n';
    std::cout << "[17] - regenerate graph" << '\n';
}