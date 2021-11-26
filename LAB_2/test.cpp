#include <chrono>
#include "gtest/gtest.h"
#include "algorithms.h"

constexpr size_t initial_size = 10;
constexpr size_t last = 5;

TEST(binary_search, works)
{
	std::vector<int> vector{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
	
	for (size_t i{}; i < vector.size(); ++i)
	{
		size_t position = i;
		int number = vector.at(i);
		size_t search_result = vector.size();

		algorithms::binary_search(vector, number, search_result);

		ASSERT_EQ(position, search_result);
		ASSERT_TRUE(algorithms::binary_search(vector, number, search_result));
	}
}

TEST(bogo_sort, works)
{
	srand(time(nullptr));

	std::vector<int> vector{};
	std::vector<int> initial_sequence{};

	for (size_t i{ 2 }; i < last - 1; ++i)
	{
		vector.resize(initial_size - i);
		generate(vector.begin(), vector.end(), rand);

		initial_sequence = vector;
		algorithms::bogo_sort(initial_sequence);
		bool sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end());
		ASSERT_TRUE(sorted);

		initial_sequence = vector;
		algorithms::bogo_sort(initial_sequence, sort_order::descending);
		sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>());
		ASSERT_TRUE(sorted);
	}
}

TEST(counting_sort, works)
{
	srand(time(nullptr));

	std::vector<char> vector{};
	std::vector<char> initial_sequence{};
	size_t step{ initial_size };

	for (size_t i{}; i < last; ++i)
	{
		vector.resize(step);
		generate(vector.begin(), vector.end(), rand);

		initial_sequence = vector;
		algorithms::counting_sort(initial_sequence);
		bool sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end());
		ASSERT_TRUE(sorted);

		initial_sequence = vector;
		algorithms::counting_sort(initial_sequence, sort_order::descending);
		sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>());
		ASSERT_TRUE(sorted);

		step *= initial_size;
	}
}

TEST(insertion_sort, works)
{
	srand(time(nullptr));

	std::vector<int> vector{};
	std::vector<int> initial_sequence{};
	size_t step{ initial_size };

	for (size_t i{}; i < last - 1; ++i)
	{
		vector.resize(step);
		generate(vector.begin(), vector.end(), rand);

		initial_sequence = vector;
		algorithms::insertion_sort(initial_sequence);
		bool sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end());
		ASSERT_TRUE(sorted);

		initial_sequence = vector;
		algorithms::insertion_sort(initial_sequence, sort_order::descending);
		sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>());
		ASSERT_TRUE(sorted);

		step *= initial_size;
	}
}

TEST(quick_sort, works)
{
	srand(time(nullptr));

	std::vector<int> vector{};
	std::vector<int> initial_sequence{};
	size_t step{ initial_size };

	for (size_t i{}; i < last - 1; ++i)
	{
		vector.resize(step);
		generate(vector.begin(), vector.end(), rand);

		initial_sequence = vector;
		algorithms::quick_sort(initial_sequence);
		bool sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end());
		ASSERT_TRUE(sorted);

		initial_sequence = vector;
		algorithms::quick_sort(initial_sequence, sort_order::descending);
		sorted = std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>());
		ASSERT_TRUE(sorted);

		step *= initial_size;
	}
}
/*
int main()
{
	srand(time(nullptr));
	std::vector<size_t> time_avg_insertion_asc(last);
	std::vector<size_t> time_avg_insertion_des(last);
	std::vector<size_t> time_avg_quick_asc(last);
	std::vector<size_t> time_avg_quick_des(last);
	std::vector<int> test_subject{};
	std::vector<int> initial_sequence{};
	size_t step{ initial_size };

	std::cout << "TEST TIME AVG 10 ATT (ns):\n\n\n";

	for (size_t i{}; i < last; ++i)
	{
		test_subject.resize(step);

		for (size_t j{}; j < initial_size; ++j)
		{
			generate(test_subject.begin(), test_subject.end(), rand);

			initial_sequence = test_subject;
			auto begin{ std::chrono::steady_clock::now() };
			algorithms::insertion_sort(initial_sequence, sort_order::ascending);
			auto end{ std::chrono::steady_clock::now() };
			assert(std::is_sorted(initial_sequence.begin(),initial_sequence.end()));
			time_avg_insertion_asc.at(i) += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / initial_size;

			initial_sequence = test_subject;
			begin = std::chrono::steady_clock::now();
			algorithms::insertion_sort(initial_sequence, sort_order::descending);
			end = std::chrono::steady_clock::now();
			assert(std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>()));
			time_avg_insertion_des.at(i) += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / initial_size;

			initial_sequence = test_subject;
			begin = std::chrono::steady_clock::now();
			algorithms::quick_sort(initial_sequence, sort_order::ascending);
			end = std::chrono::steady_clock::now();
			assert(std::is_sorted(initial_sequence.begin(), initial_sequence.end()));
			time_avg_quick_asc.at(i) += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / initial_size;

			initial_sequence = test_subject;
			begin = std::chrono::steady_clock::now();
			algorithms::quick_sort(initial_sequence, sort_order::descending);
			end = std::chrono::steady_clock::now();
			assert(std::is_sorted(initial_sequence.begin(), initial_sequence.end(), std::greater<>()));
			time_avg_quick_des.at(i) += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / initial_size;
		}
		std::cout << "VECTOR SIZE:\t\t"
				  << std::to_string(step)
				  << "\n\nINSERTION ASC:\t\t"
				  << std::to_string(time_avg_insertion_asc.at(i))
				  << "\nINSERTION DES:\t\t"
				  << std::to_string(time_avg_insertion_des.at(i))
				  << "\nQUICK ASC:\t\t"
				  << std::to_string(time_avg_quick_asc.at(i))
				  << "\nQUICK DES:\t\t"
				  << std::to_string(time_avg_quick_des.at(i))
				  << "\n\n\n";


		step *= initial_size;
	}
	return 0;
}*/