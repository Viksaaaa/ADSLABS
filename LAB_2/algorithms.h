#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>

namespace algorithms
{
	bool binary_search(const std::vector<int> &, const int &, size_t &);

	std::vector<char> counting_sort(const std::vector<char> &, const bool &mode = true);

	void bogo_sort(std::vector<int> &, const bool &mode = true);

	void insertion_sort(std::vector<int> &, const bool &mode = true);

};

#endif