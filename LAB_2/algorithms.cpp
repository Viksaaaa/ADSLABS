#include "algorithms.h"

namespace 
{
	bool sorted(const std::vector<int> &vector, const sort_order &order)
	{
		if (order == sort_order::ascending)
		{
			for (size_t i = 0; i < vector.size() - 1; ++i)
			{
				if (vector.at(i) > vector.at(i + 1))
				{
					return false;
				}
			}
		}
		
		else
		{
			for (size_t i = 0; i < vector.size() - 1; ++i)
			{
				if (vector.at(i) < vector.at(i + 1))
				{
					return false;
				}
			}
		}

		return true;
	}

	void shuffle(std::vector<int> &vector)
	{
		for (size_t i = 0; i < vector.size(); ++i)
		{
			const int temp = vector.at(i);

			const int position = rand() % vector.size();

			vector.at(i) = vector.at(position);

			vector.at(position) = temp;
		}
	}

	size_t partition(std::vector<int> &vector, const size_t &left, const size_t &right, const sort_order &order)
	{
		size_t i = right;

		if (order == sort_order::ascending)
		{
			for (size_t j = right; j >= left + 1; --j)
			{
				if (vector.at(j) > vector.at(left))
				{
					const int temp = vector.at(j);
					vector.at(j) = vector.at(i);
					vector.at(i--) = temp;
				}
			}
		}
		else
		{
			for (size_t j = right; j >= left + 1; --j)
			{
				if (vector.at(j) < vector.at(left))
				{
					const int temp = vector.at(j);
					vector.at(j) = vector.at(i);
					vector.at(i--) = temp;
				}
			}
		}
		const int temp = vector.at(i);
		vector.at(i) = vector.at(left);
		vector.at(left) = temp;

		return i;
	}

	void recursive_quick_sort(std::vector<int> &vector, const size_t &left, const size_t &right, const sort_order &order)
	{
		if (left < right)
		{
			const size_t pivot = partition(vector, left, right, order);

			recursive_quick_sort(vector, left, pivot - (pivot != left), order);
			recursive_quick_sort(vector, pivot + (pivot != right), right, order);
		}
	}

	
};


bool algorithms::binary_search( const std::vector<int> &vector, const int &key, size_t &position )
{
	if (vector.empty())
	{
		return false;
	}

	size_t left{};
	size_t right{ vector.size() };

	const bool order = vector.at( right - 1 ) > vector.at( left );

	while ( right > left )
	{
		const size_t middle{ left + (right - left) / 2 };

		if ( vector.at(middle) == key )
		{
			position = middle;
			
			return true;
		}

		if ( vector.at(middle) > key == order )
		{
			right = middle;
		}
		else
		{
			left = middle + 1;
		}
	}
	return false;
}

void algorithms::counting_sort(std::vector<char> &vector, const sort_order &order)
{
	if ( vector.size() < 2 )
	{
		return;
	}
	
	char min = vector.at(0);
	char max = vector.at(0);

	for(size_t i = 1; i < vector.size(); ++i)
	{
		if (vector.at(i) < min)
		{
			min = vector.at(i);
		}

		if (vector.at(i) > max)
		{
			max = vector.at(i);
		}
	}

	std::vector<size_t> counter(1 + max - min);

	for(const auto &element : vector)
	{
		++counter.at(element - min);
	}

	if (order == sort_order::ascending)
	{
		for(size_t i = 1; i < counter.size(); ++i)
		{
			counter.at(i) += counter.at(i - 1);
		}
	}

	else
	{
		for (size_t i = counter.size() - 1; i-- > 0; )
		{
			counter.at(i) += counter.at(i + 1);
		}
	}
	
	std::vector<char> result(vector.size());

	for (size_t i = vector.size(); i-- > 0; ) 
	{
		result.at(--counter.at(vector.at(i) - min)) = vector.at(i);
	}

	vector = std::move(result);
}

void algorithms::bogo_sort(std::vector<int> &vector, const sort_order &order)
{
	while (!sorted(vector, order))
	{
		shuffle(vector);
	}
}

void algorithms::insertion_sort(std::vector<int> &vector, const sort_order &order)
{
	int temp{};

	if (order == sort_order::ascending)
	{
		for (size_t i = 1; i < vector.size(); ++i)
		{
			temp = vector.at(i);

			size_t j = i;

			for (; j-- > 0 && vector.at(j) > temp; )
			{
				vector.at(j + 1) = vector.at(j);
			}
			vector.at(j + static_cast<bool>(j)) = temp;
		}
	}
	else
	{
		for (size_t i = 1; i < vector.size(); ++i)
		{
			temp = vector.at(i);

			size_t j = i;

			for (; j-- > 0 && vector.at(j) < temp; )
			{
				vector.at(j + 1) = vector.at(j);
			}
			vector.at(j + static_cast<bool>(j)) = temp;
		}
	}
}

void algorithms::quick_sort(std::vector<int> &vector, const sort_order &order)
{
	recursive_quick_sort(vector, 0, vector.size() - 1, order);
}

