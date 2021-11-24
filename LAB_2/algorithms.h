#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

enum class sort_order
{
	descending,
	ascending
};

namespace algorithms
{
	bool binary_search(const std::vector<int> &, const int &, size_t &);

	void counting_sort(std::vector<char> &, const sort_order &order = sort_order::ascending);

	void bogo_sort(std::vector<int> &, const sort_order &order = sort_order::ascending);

	void insertion_sort(std::vector<int> &, const sort_order &order = sort_order::ascending);

	void quick_sort(std::vector<int> &, const sort_order &order = sort_order::ascending);
};

#endif