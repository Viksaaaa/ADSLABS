#include "graph.h"

int main() {
	Graph graph;

	if (!loadGraph(graph, "./test.txt"))
		return 1;

	graph.printVertices(); std::cout << '\n';
	graph.print(); std::cout << '\n';
	printShortestPath(graph, "Saint-Petersburg", "Moscow"); std::cout << '\n';

	return 0;
}