#include "linked_list.h"
#include <iostream>
#include "gtest/gtest.h"

constexpr size_t initial_size = 20;
constexpr int initial_value = -17;

TEST( is_empty, equals_true_for_0_elements )
{
    linked_list list{};

    const bool result = list.is_empty();

    ASSERT_TRUE( result );
}

TEST( is_empty, equals_false_for_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const bool result = list.is_empty();

    ASSERT_FALSE( result );
}

TEST( is_empty, equals_false_for_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    const bool result = list.is_empty();

    ASSERT_FALSE( result );
}

TEST( get_size, equals_to_0_for_0_elements )
{
    linked_list list{};

    const bool result = list.get_size();

    ASSERT_EQ( result, 0 );
}

TEST( get_size, equals_to_1_for_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const bool result = list.get_size();

    ASSERT_EQ( result, 1 );
}

TEST( get_size, not_equals_to_35_for_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    const bool result = list.get_size();

    ASSERT_NE( result, 35 );
}

TEST( at, throws_std_out_of_range_for_0_elements )
{
    linked_list list{};
    std::string error_result{};

    try
    {
        int i = list.at( 0 );
    }
    catch ( const std::out_of_range &error )
    {
        error_result.append( error.what() );
    }
    ASSERT_STREQ( error_result.c_str(), "Index (0) should be lower than size (0)" );
    ASSERT_THROW( int i = list.at( 0 ), std::out_of_range );
}

TEST( at, at_0_returns_first_element_for_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const int result = list.at( 0 );

    ASSERT_EQ( result, initial_value );
}

TEST( at, at_19_returns_last_element_for_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    const int result = list.at( initial_size - 1 );

    ASSERT_EQ( result, initial_size - 1 );
}

TEST( set, throws_std_out_of_range_for_0_elements)
{
    linked_list list{};
	std::string error_result{};

	try
	{
		list.set(0, 15);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}

	ASSERT_STREQ(error_result.c_str(), "Index (0) should be lower than size (0)");
	ASSERT_THROW(list.set(0, 15), std::out_of_range);
}


TEST( set, modifies_first_element )
{
    linked_list list{};

    list.push_front( initial_value );

    constexpr size_t position{};
    const int result1 = list.at( position );

    ASSERT_EQ( result1, initial_value );

    constexpr int new_value = 5;

    list.set( position, new_value );

    const int result2 = list.at( position );

    ASSERT_EQ( result2, new_value );
}

TEST( set, modifies_element_in_arbitrary_position )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    constexpr size_t position{ 15 };
    const int result1 = list.at( position );

    ASSERT_EQ( result1, 15 );

    constexpr int new_value = 45;

    list.set( position, new_value );

    const int result2 = list.at( position );

    ASSERT_EQ( result2, new_value );
}

TEST( push_back, pushes_in_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    constexpr int new_value = 10;

    list.push_back( new_value );

    const size_t list_size{ list.get_size() };

    ASSERT_EQ( list_size, 1 );

    const int result = list.at( list_size - 1 );

    ASSERT_EQ( result, new_value );
}

TEST( push_back, pushes_in_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t starting_size{ list.get_size() };

    ASSERT_EQ( starting_size, 1 );

    constexpr int new_value = 20;

    list.push_back( new_value );

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( new_size - 1, starting_size );

    const int result = list.at( new_size - 1 );

    ASSERT_EQ( result, new_value );

}

TEST( push_back, pushes_in_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    const size_t starting_size{ list.get_size() };

    ASSERT_EQ( starting_size, initial_size );

    constexpr int new_value = 36;

    list.push_back( new_value );

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( new_size - 1, initial_size );

    const int result = list.at( new_size - 1 );

    ASSERT_EQ( result, new_value );
}

TEST( push_front, pushes_in_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    constexpr int new_value = 10;

    list.push_front( new_value );

    const size_t list_size{ list.get_size() };

    ASSERT_EQ( list_size, 1 );

    const int result = list.at( list_size - 1 );

    ASSERT_EQ( result, new_value );
}

TEST( push_front, pushes_in_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t starting_size{ list.get_size() };

    ASSERT_EQ( starting_size, 1 );

    constexpr int new_value = 20;

    list.push_front( new_value );

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( new_size - 1, starting_size );

    const int result = list.at( 0 );

    ASSERT_EQ( result, new_value );
}

TEST( push_front, pushes_in_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    const size_t result{ list.get_size() };

    ASSERT_EQ( result, initial_size );

    constexpr int new_value = 36;

    list.push_front( new_value );

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( new_size - 1, initial_size );

    const int result_at_0 = list.at( 0 );

    ASSERT_EQ( result_at_0, new_value );
}

TEST( pop_back, does_nothing_with_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    list.pop_back();

    ASSERT_EQ( list, linked_list{} );
}

TEST( pop_back, pops_from_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t result{ list.get_size() };

    ASSERT_EQ( result, 1 );

    list.pop_back();

    ASSERT_EQ( list, linked_list{} );
}

TEST( pop_back, pops_from_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    ASSERT_EQ( list.get_size(), initial_size );
    ASSERT_EQ( list.at( initial_size - 1 ), initial_size - 1 );

    list.pop_back();

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( initial_size - 1, new_size );
    ASSERT_EQ( list.at( new_size - 1 ), 18 );
}

TEST( pop_front, does_nothing_with_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    list.pop_front();

    ASSERT_EQ( list, linked_list{} );
}

TEST( pop_front, pops_from_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t result{ list.get_size() };

    ASSERT_EQ( result, 1 );

    list.pop_front();

    ASSERT_EQ( list, linked_list{} );
}

TEST( pop_front, pops_from_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    ASSERT_EQ( list.get_size(), initial_size );
    ASSERT_EQ( list.at( 0 ), 0 );

    list.pop_front();

    const size_t new_size{ list.get_size() };

    ASSERT_EQ( initial_size - 1, new_size );
    ASSERT_EQ( list.at( 0 ), 1 );
}

TEST( clear, does_nothing_with_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    list.clear();

    ASSERT_EQ( list, linked_list{} );
}

TEST( clear, clears_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t result{ list.get_size() };

    ASSERT_EQ( result, 1 );

    list.clear();

    ASSERT_EQ( list, linked_list{} );
}

TEST( clear, clears_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    ASSERT_EQ( list.get_size(), initial_size );

    list.clear();

    ASSERT_EQ( list, linked_list{} );
}

TEST(remove, throws_std_out_of_range_for_0_elements)
{
    linked_list list{};
	std::string error_result{};

    ASSERT_EQ( list, linked_list{} );

	try
	{
		list.remove(9034231);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}

	ASSERT_STREQ(error_result.c_str(), "Index (9034231) should be lower than size (0)");
	ASSERT_THROW(list.remove(9034231), std::out_of_range);
}


TEST( remove, removes_from_list_with_1_element )
{
    linked_list list{};
	std::string error_result{};

    list.push_front( initial_value );

    const size_t result{ list.get_size() };

    ASSERT_EQ( result, 1 );

	try
	{
		list.remove(9034231);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}
    
	ASSERT_STREQ(error_result.c_str(), "Index (9034231) should be lower than size (1)");
	ASSERT_THROW(list.remove(9034231), std::out_of_range);
    ASSERT_EQ( list.get_size(), result );

    list.remove( 0 );

    ASSERT_EQ( list, linked_list{} );
}


TEST( remove, removes_from_list_with_20_elements )
{
    linked_list list{};
	std::string error_result{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    ASSERT_EQ( list.get_size(), initial_size );

	try
	{
		list.remove(initial_size);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}

	ASSERT_STREQ(error_result.c_str(), "Index (20) should be lower than size (20)");
	ASSERT_THROW(list.remove(initial_size), std::out_of_range);
    ASSERT_EQ( list.get_size(), initial_size );

    constexpr size_t position{ 10 };

    ASSERT_EQ( list.at( position ), position );

    list.remove( position );

    ASSERT_EQ( list.get_size(), initial_size - 1 );
    ASSERT_EQ( list.at( position ), position + 1 );
}

TEST( insert_int, does_not_insert_in_wrong_position)
{
	linked_list list{};
	std::string error_result{};
	
	ASSERT_EQ( list, linked_list{});

	constexpr int new_element = 15;
	constexpr size_t position{};

	try
	{
		list.insert(position, new_element);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}
	
	ASSERT_STREQ(error_result.c_str(), "Index (0) should be lower than size (0)");
	ASSERT_THROW(list.insert(position, new_element), std::out_of_range);
	ASSERT_EQ( list.get_size(), position );
	ASSERT_THROW( list.at( position ), std::out_of_range );

}

TEST( insert_int, inserts_in_list_with_1_element)
{
	linked_list list{};

	list.push_front( initial_value );

	const size_t starting_size{ list.get_size() };

	ASSERT_EQ( starting_size, 1 );

	constexpr int new_element = 25;
	constexpr size_t position{};
	
	list.insert( position, new_element );

	ASSERT_EQ( list.get_size(), starting_size + 1 );
	ASSERT_EQ( list.at(position), new_element );

}
TEST( insert_int, nserts_in_list_with_20_elements)
{
	linked_list list{};

	for ( int i = initial_size - 1; i >= 0; --i )
	{
		list.push_front( i );
	}
	ASSERT_EQ( list.get_size(), initial_size );

	const size_t starting_size = list.get_size();
	constexpr int new_element = 35;
	constexpr size_t position{};

	list.insert( position, new_element );

	ASSERT_EQ( list.get_size(), starting_size + 1 );
	ASSERT_EQ( list.at( position ), new_element );

}
TEST( insert_list, insert_list2_into_list1_with_no_element )
{
	linked_list list1{};

	ASSERT_EQ( list1, linked_list{} );

	linked_list list2{};

	constexpr size_t start_size_list2{ initial_size };

	for ( int i = start_size_list2 - 1; i >= 0; --i )
	{
		list2.push_front(i);
	}
	ASSERT_EQ( list2.get_size(), start_size_list2 );

	size_t end_size{ list2.get_size() };

	constexpr size_t position{};

	list1.insert( position, list2 );

	ASSERT_EQ( list1.get_size(), end_size );
	ASSERT_EQ( list1.at( position ), position);

}

TEST( insert, insert_list2_into_list1_with_several_elements )
{ 
	linked_list list1{};

	list1.push_front(initial_value);

	const size_t start_size_list1{ list1.get_size() };

	ASSERT_EQ(start_size_list1, 1);

	linked_list list2{};

	constexpr size_t start_size_list2{ initial_size - 8 };

	for (int i = start_size_list2 - 1; i >= 0; --i)
	{
		list2.push_front(i);
	}
	ASSERT_EQ( list2.get_size(), start_size_list2 );

	size_t end_size{ start_size_list1 + start_size_list2 };
	size_t position{};

	list1.insert( position, list2 );

	ASSERT_EQ( list1.get_size(), end_size );
	ASSERT_EQ( list1.at(position), list2.at(position) );
	
	end_size =  2 * end_size ;
	position = 13;

	list1.insert(position, list1);

	ASSERT_EQ( list1.get_size(), end_size );
	ASSERT_EQ( list1.at(6), 6 );

	std::string error_result{};

	try
	{
		list1.insert(7832, list2);
	}
	catch (const std::out_of_range &error)
	{
		error_result.append(error.what());
	}

	ASSERT_STREQ(error_result.c_str(), "Index (7832) should be lower than or equal to size (26)");
	ASSERT_THROW(list1.insert(7832, list2), std::out_of_range);
}

TEST( output_operator, empty_string_for_empty_list )
{
	const linked_list list{};

	ASSERT_EQ(list, linked_list{});

	const std::string string{};
	std::ostringstream ostringstream{};

	ostringstream << list;

	const std::string string_output{ ostringstream.str() };

	ASSERT_EQ(string_output, string);
}

TEST( output_operator, outputs_list_with_1_element )
{
	linked_list list{};

	list.push_front(initial_value);

	ASSERT_EQ(list.get_size(), 1);

	const std::string string{ std::string{ "0.\t" }
							  .append(std::to_string(initial_value)) };
	std::ostringstream ostringstream{};

	ostringstream << list;

	const std::string string_output{ ostringstream.str() };

	ASSERT_EQ(string_output, string);
}

TEST( output_operator, outputs_list_with_20_elements)
{
	linked_list list{};

	for (int i = initial_size - 1; i >= 0; --i)
	{
		list.push_front(i);
	}
	ASSERT_EQ(list.get_size(), initial_size);

	std::string string{};

	for (size_t i{}; i < initial_size; ++i)
	{
		string.append(std::to_string(i))
			.append(".\t")
			.append(std::to_string(i))
			.append(i == initial_size - 1 ? "" : "\n");
	}
	std::ostringstream ostringstream{};

	ostringstream << list;

	const std::string string_output{ ostringstream.str() };

	ASSERT_EQ(string_output, string);

}

TEST( comparison_operator, true_for_two_empty_lists )
{
	linked_list list1{};
	linked_list list2{};

	bool result = list1 == list2;

	ASSERT_TRUE(result);

}

TEST( comparison_operator, false_for_two_not_equal_lists )
{
	linked_list list1{};
	
	linked_list list2{};

	for (int i = 3; i >= 0; --i)
	{
		list2.push_front( i );
	}

	bool result = list1 == list2;

	ASSERT_FALSE( result );

}


int main()
{
	linked_list list{};

	size_t size{ 17 };

	for ( int i = 0; i < size; ++i )
	{
		list.push_front( i );
	}
	std::cout << list << "\n\n";

	list.pop_front();

	std::cout << list << '\n';

	return 0;
}
