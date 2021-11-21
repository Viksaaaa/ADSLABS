#include "gtest/gtest.h"
#include "algorithms.h"

constexpr size_t extra_small = 10;
constexpr size_t small = 100;
constexpr size_t medium = 1'000;
constexpr size_t large = 10'000;
constexpr size_t extra_large = 100'000;

TEST(binary_search, works)
{
	std::vector<int> vector{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

	size_t position = 5;
	int number = vector.at(position);
	size_t search_result = 10;
	
	algorithms::binary_search(vector, number, search_result);

	ASSERT_EQ(position, search_result);
	ASSERT_TRUE(algorithms::binary_search(vector, number, search_result));

	position = 9;
	number = vector.at(position);
	search_result = 10;

	algorithms::binary_search(vector, number, search_result);

	ASSERT_EQ(position, search_result);
	ASSERT_TRUE(algorithms::binary_search(vector, number, search_result));
}
/*
TEST(bogo_sort, works)
{
	srand(time(nullptr));

	std::vector<int> vector(extra_small - 3);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::bogo_sort(vector);

	bool sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(extra_small - 2);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::bogo_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(extra_small - 1);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::bogo_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);
}
*/
TEST(counting_sort, works)
{
	srand(time(nullptr));

	std::vector<char> vector(extra_small);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::counting_sort(vector);

	bool sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(medium);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::counting_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(extra_large);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::counting_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);
}

TEST(insertion_sort, works)
{
	srand(time(nullptr));

	std::vector<int> vector(extra_small);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::insertion_sort(vector);

	bool sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(medium);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::insertion_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);

	vector.resize(large);
	std::generate(vector.begin(), vector.end(), std::rand);

	algorithms::insertion_sort(vector);

	sorted = std::is_sorted(vector.begin(), vector.end());

	ASSERT_TRUE(sorted);
}
/*

int main()
{
	srand(time(0));
}
*/

