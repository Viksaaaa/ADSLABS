#include "algorithms.h"

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

std::vector<char> algorithms::counting_sort(std::vector<char> &vector, const bool &mode)
{
	if ( vector.size() < 2 )
	{
		return std::vector<char>{vector};
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

	for(size_t i = 0; i < vector.size(); ++i)
	{
		++counter.at(vector.at(i) - min);
	}

	if (mode)
	{
		for(size_t i = 1; i < counter.size(); ++i)
		{
			counter.at(i) += counter.at(i - 1);
		}
	}

	else
	{
		for (size_t i = counter.size() - 2; i > 0; --i)
		{
			counter.at(i) += counter.at(i + 1);
		}

		counter.at(0) += counter.at(1);
	}
	
	std::vector<char> result(vector.size());

	for (size_t i = vector.size() - 1; i > 0; --i) 
	{
		result.at(--counter.at(vector.at(i) - min)) = vector.at(i);
	}

	result.at(--counter.at(vector.at(0) - min)) = vector.at(0);

	return result;
}