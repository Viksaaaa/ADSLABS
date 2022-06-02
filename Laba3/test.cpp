#include "graph.h"

int main() {
	Graph graph;

	if (!loadGraph(graph, "./test.txt"))
		return 1;

	graph.printVertices(); std::cout << '\n';
	graph.print(); std::cout << '\n';
	printShortestPath(graph, "Kazan", "Moscow"); std::cout << '\n';

	return 0;
}