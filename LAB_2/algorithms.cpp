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
