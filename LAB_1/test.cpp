#include "pch.h"
#include "linked_list.h"

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

TEST( set, does_not_modify_empty_list )
{
    linked_list list{};

    list.set( 0, 15 );

    ASSERT_EQ( list, linked_list{} );
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

TEST( remove, does_nothing_with_empty_list )
{
    linked_list list{};

    ASSERT_EQ( list, linked_list{} );

    list.remove( 9034231 );

    ASSERT_EQ( list, linked_list{} );
}

TEST( remove, removes_from_list_with_1_element )
{
    linked_list list{};

    list.push_front( initial_value );

    const size_t result{ list.get_size() };

    ASSERT_EQ( result, 1 );

    list.remove( 9034231 );

    ASSERT_EQ( list.get_size(), result );

    list.remove( 0 );

    ASSERT_EQ( list, linked_list{} );
}

TEST( remove, removes_from_list_with_20_elements )
{
    linked_list list{};

    for ( int i = initial_size - 1; i >= 0; --i )
    {
        list.push_front( i );
    }
    ASSERT_EQ( list.get_size(), initial_size );

    list.remove( initial_size );

    ASSERT_EQ( list.get_size(), initial_size );

    constexpr size_t position{ 10 };

    ASSERT_EQ( list.at( position ), position );

    list.remove( position );

    ASSERT_EQ( list.get_size(), initial_size - 1 );
    ASSERT_EQ( list.at( position ), position + 1 );
}
