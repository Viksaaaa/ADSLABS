#include "gtest/gtest.h"
#include "binary_search_tree.h"

TEST(insert, inserts_in_empty_tree)
{
	binary_search_tree tree{};
	constexpr int element = 666;

	tree.insert(element);

	ASSERT_EQ(*tree.begin(), element);
}

TEST(insert, inserts_in_tree_with_elements)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;

	tree.insert(element_1);

	ASSERT_EQ(*tree.begin(), element_1);

	constexpr int element_2 = 7;

	tree.insert(element_2);

	ASSERT_EQ(*tree.begin(), element_2);

	constexpr int element_3 = 55;

	tree.insert(element_3);

	ASSERT_EQ(*(--tree.end()), element_3);

	constexpr int element_4 = 24;

	tree.insert(element_4);

	ASSERT_EQ(*(tree.end() - 2), element_4);
	ASSERT_EQ(*(tree.begin() + 2), element_4);

	tree.insert(element_2);
	tree.insert(element_3);

	std::vector<int> vector_from_tree{};

	for (const auto &element : tree)
	{
		vector_from_tree.push_back(element);
	}

	bool repeat{ false };

	for (size_t i = 1; i < vector_from_tree.size(); ++i)
	{
		if (vector_from_tree.at(i) == vector_from_tree.at(i - 1))
		{
			repeat = true;
			break;
		}
	}
	ASSERT_FALSE(repeat);

}

TEST(remove, removes_element_in_tree)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;

	tree.insert(element_1);

	ASSERT_EQ(*tree.begin(), element_1);

	tree.remove(element_1);

	ASSERT_EQ(tree.begin(), tree.end());

	ASSERT_THROW(*tree.begin(), std::out_of_range);

}

TEST(remove, removes_element_in_big_tree)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;
	tree.insert(element_1);

	constexpr int element_2 = 7;
	tree.insert(element_2);

	constexpr int element_3 = 55;
	tree.insert(element_3);

	constexpr int element_4 = 24;
	tree.insert(element_4);

	ASSERT_EQ(*(tree.begin() + 1), element_1);

	tree.remove(element_1);

	ASSERT_EQ(*(tree.end() - 2), element_4);
	ASSERT_EQ(*(tree.begin() + 1), element_4);
}

TEST(contains, works)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;
	tree.insert(element_1);

	constexpr int element_2 = 7;
	tree.insert(element_2);

	constexpr int element_3 = 55;
	tree.insert(element_3);

	constexpr int element_4 = 24;
	tree.insert(element_4);

	bool result = tree.contains(element_2);

	ASSERT_TRUE(result);

	result = tree.contains(78);

	ASSERT_FALSE(result);

}

TEST(iterator_in_order, works)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;
	tree.insert(element_1);

	constexpr int element_2 = 7;
	tree.insert(element_2);

	constexpr int element_3 = 55;
	tree.insert(element_3);

	constexpr int element_4 = 24;
	tree.insert(element_4);

	constexpr int element_5 = 2;
	tree.insert(element_5);

	constexpr int element_6 = 17;
	tree.insert(element_6);

	constexpr int element_7 = 5;
	tree.insert(element_7);

	constexpr int element_8 = 46;
	tree.insert(element_8);

	ASSERT_TRUE(*(tree.begin()), element_5);
	ASSERT_THROW(*(tree.begin() - 3), std::out_of_range);
	ASSERT_TRUE(*(tree.begin() + 6), element_8);
	ASSERT_THROW(*(tree.begin() + 10), std::out_of_range);

	ASSERT_TRUE(*(tree.end() - 1), element_3);
	ASSERT_THROW(*(tree.end()), std::out_of_range);
	ASSERT_TRUE(*(tree.end() - 5), element_6);
	ASSERT_THROW(*(tree.end() - 10), std::out_of_range);

}

TEST(iterator_breadth_first, works)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;
	tree.insert(element_1);

	constexpr int element_2 = 7;
	tree.insert(element_2);

	constexpr int element_3 = 55;
	tree.insert(element_3);

	constexpr int element_4 = 24;
	tree.insert(element_4);

	constexpr int element_5 = 2;
	tree.insert(element_5);

	constexpr int element_6 = 17;
	tree.insert(element_6);

	constexpr int element_7 = 5;
	tree.insert(element_7);

	constexpr int element_8 = 46;
	tree.insert(element_8);

	tree.toggle_iterator_type();

	ASSERT_TRUE(*(tree.begin()), element_1);
	ASSERT_THROW(*(tree.begin() - 3), std::out_of_range);
	ASSERT_TRUE(*(tree.begin() + 4), element_6);
	ASSERT_THROW(*(tree.begin() + 10), std::out_of_range);

	ASSERT_TRUE(*(tree.end() - 1), element_8);
	ASSERT_THROW(*(tree.end()), std::out_of_range);
	ASSERT_TRUE(*(tree.end() - 7), element_2);
	ASSERT_THROW(*(tree.end() - 10), std::out_of_range);

}

/*
int main()
{
	binary_search_tree tree{};

	tree.insert(20);
	tree.insert(0);
	tree.insert(2);
	tree.insert(1);
	tree.insert(15);
	tree.insert(8);
	tree.insert(5);
	tree.insert(11);
	tree.insert(23);
	tree.insert(21);
	tree.insert(25);

	std::cout << "In order:\n\n";

	for (const auto &element : tree)
	{
		std::cout << element << '\n';
	}
	std::cout << "\nIn order (reverse):\n\n";

	for (auto it{ --tree.end() }; ; --it)
	{
		std::cout << *it << '\n';

		if (it == tree.begin())
		{
			break;
		}
	}
	tree.toggle_iterator_type();

	std::cout << "\nBreadth first:\n\n";

	for (const auto &element : tree)
	{
		std::cout << element << '\n';
	}
	std::cout << "\nBreadth first (reverse):\n\n";

	for (auto it{ --tree.end() }; ; --it)
	{
		std::cout << *it << '\n';

		if (it == tree.begin())
		{
			break;
		}
	}
	return 0;
}
*/