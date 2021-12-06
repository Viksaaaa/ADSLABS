#include "gtest/gtest.h"
#include "binary_search_tree.h"

TEST(insert, inserts_in_empty_tree)
{
	binary_search_tree tree{};
	constexpr int element = 666;

	tree.insert(element);

	const int result = tree.create_bft_iterator().next();

	ASSERT_EQ(result, element);
}

TEST(insert, inserts_in_tree_with_elements)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;

	tree.insert(element_1);

	ASSERT_EQ(tree.create_dft_iterator().next(), element_1);

	constexpr int element_2 = 7;

	tree.insert(element_2);

	ASSERT_EQ(tree.create_dft_iterator().next(), element_2);

	constexpr int element_3 = 55;

	tree.insert(element_3);

	auto iterator{ tree.create_dft_iterator() };

	for (int i = 0; i < 2; ++i)
	{
		iterator.next();
	}

	ASSERT_EQ(iterator.next(), element_3);
	
	
	constexpr int element_4 = 24;

	tree.insert(element_4);

	auto iterator2{ tree.create_dft_iterator() };

	for (int i = 0; i < 2; ++i)
	{
		iterator2.next();
	}

	ASSERT_EQ(iterator2.next(), element_4);
	
}

TEST(remove, removes_element_in_tree)
{
	binary_search_tree tree{};
	constexpr int element_1 = 21;
	tree.insert(element_1);
	ASSERT_EQ(tree.create_bft_iterator().next(), element_1);

	tree.remove(element_1);
	ASSERT_THROW(tree.create_bft_iterator().next(), std::out_of_range);

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

	ASSERT_EQ(tree.create_dft_iterator().next(), element_2);

	tree.remove(element_2);

	ASSERT_FALSE(tree.contains(element_2));

	ASSERT_EQ(tree.create_dft_iterator().next(), element_1);
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


TEST(create_dft_iterator, works)
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

	auto iterator{ tree.create_dft_iterator() };

	for (int i = 0; i < 2; ++i)
	{
		iterator.next();
	}

	ASSERT_EQ(iterator.next(), element_2);

	for (int i = 0; i < 2; ++i)
	{
		iterator.next();
	}

	ASSERT_EQ(iterator.next(), element_4);
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

	auto iterator{ tree.create_bft_iterator() };

	for (int i = 0; i < 2; ++i)
	{
		iterator.next();
	}

	ASSERT_EQ(iterator.next(), element_3);

	for (int i = 0; i < 3; ++i)
	{
		iterator.next();
	}

	ASSERT_EQ(iterator.next(), element_7);
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

    	auto dft{ tree.create_dft_iterator() };

   	while ( dft.has_next() )
   	{
      		std::cout << dft.next() << '\n';
   	}
   	std::cout << "\nBreadth first:\n\n";

  	auto bft{ tree.create_bft_iterator() };
	
  	while ( bft.has_next() )
    	{
   	     std::cout << bft.next() << '\n';
  	}
		
return 0;
}
*/
