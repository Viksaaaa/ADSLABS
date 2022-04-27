#include "gtest/gtest.h"
#include "rbtree.h"
#include <iostream>
#include <vector>
/*
 *
 *����� �� ������������ ������ ������ ���������:
1. ��������� ����.
2. ���������� ������.
3. �������� ������������ ������ � �������.
4. ������ ��������� ��������� ������� ������.
5. �������� ������������� unit-������.
6. ������ ������.
7. �������.
*/

/*
 * ����������� ��������� ������������� ������ (map) �� ������ ������-
������� ������. ������� unit-������ �� ���� ����������� ������� ��������
������������ �����������.
������ �������:
+    1. insert(����, ��������)    // ���������� �������� � ������ � ���������
+    2. remove(����)                // �������� �������� �� �����
+    3. find(����)                // ����� �������� �� �����
+    4. clear                    // �������� �������������� �������
+    5. get_keys                    // ���������� ������ ������
+    6. get_values                // ���������� ������ ��������
+    7. print                    // ����� � �������
 */


TEST(insert, inserts)
{
	map<std::string, int> map{};

	const std::string key{ "Milk" };
	constexpr int value = 2;
	const std::pair<bool, bool> result{ map.insert(key, value) };

	ASSERT_TRUE(result.first);
	ASSERT_FALSE(result.second);

	const auto search{ map.find(key) };

	ASSERT_NE(search, nullptr);
	ASSERT_EQ(search->value, value);

	const linked_list<std::string> keys{ map.get_keys() };

	ASSERT_EQ(keys.get_size(), 1);
	ASSERT_EQ(keys.at(0), key);

	const linked_list<int> values{ map.get_values() };

	ASSERT_EQ(values.get_size(), 1);
	ASSERT_EQ(values.at(0), value);
	
	map.clear();

	const std::vector<std::string> new_keys{ "Potatoes", "Almond", "Bread", "Coconut", "Cheese", "Crisps", "" };
	std::vector<int> new_values{ 1, 4, 76, 23, 2, 5, 8};

	for (size_t i{}; i < new_keys.size(); ++i)
	{
		map.insert(new_keys.at(i), new_values.at(i));
	}

	linked_list<std::string> new_result{ map.get_keys() };
	std::vector<std::string> expected{ "", "Almond" , "Bread" , "Cheese", "Coconut", "Crisps",  "Potatoes" };

	for (size_t i{}; i < expected.size(); ++i)
	{
		ASSERT_EQ(expected.at(i), new_result.at(i));
	}
}

TEST(find, works)
{
	map<std::string, int> order{};
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Bread", "Bread" };
	std::vector<int> values{ 1, 4, 76, 23, 2 };

	for (size_t i{}; i < keys.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}
	
	auto result{order.find(keys.at(keys.size()-1))};

	ASSERT_NE(result, nullptr);
	ASSERT_EQ(result->value, values.at(values.size() - 1));
	ASSERT_NE(result->value, values.at(2));
	ASSERT_NE(result->value, values.at(3));
}

TEST(get_keys, works)
{
	map<std::string, int> order{};
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Bread", "Bread" };
	std::vector<int> values{ 1, 4, 76, 23, 2 };

	for (size_t i{}; i < keys.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}
	linked_list<std::string> result{ order.get_keys() };

	ASSERT_TRUE(result.get_size() == 3);

	std::vector<std::string> expected{ "Almond" , "Bread" ,  "Potatoes" };

	for (size_t i{}; i < expected.size(); ++i)
	{
		ASSERT_EQ(expected.at(i), result.at(i));
	}
}

TEST(get_values, works)
{
	map<std::string, int> order{};
	std::vector<int> values{ 1, 4, 76, 23, 2 };
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Coconut", "Carrot" };

	for (size_t i{}; i < values.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}
	linked_list<int> result{ order.get_values() };

	ASSERT_TRUE(result.get_size() == 5);

	std::vector<int> expected{ 4 , 76 , 2 , 23 , 1 };

	for (size_t i{}; i < expected.size(); ++i)
	{
		ASSERT_EQ(expected.at(i), result.at(i));
	}
}

TEST(remove, works)
{
	map<std::string, int> order{};
	std::vector<int> values{ 1, 4, 76, 23, 2 };
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Coconut", "Carrot" };

	for (size_t i{}; i < keys.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}

	auto result{ order.remove(keys.at(keys.size() - 1)) };

	ASSERT_TRUE(result);
}

TEST(clear, works)
{
	map<std::string, int> order{};
	std::vector<int> values{ 1, 3, 5, 7, 9 };
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Coconut", "Carrot" };

	for (size_t i{}; i < keys.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}

	order.clear();
	auto result{ order.print() };

	ASSERT_TRUE(result == "");
}

TEST(print, works)
{
	map<std::string, int> order{};
	std::vector<int> values{ 1, 3, 5, 7, 9 };
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Coconut", "Carrot" };

	for (size_t i{}; i < keys.size(); ++i)
	{
		order.insert(keys.at(i), values.at(i));
	}
	
	order.print();
}
/*
int main()
{
    map<std::string, int> map{};
	std::vector<int> values{ 1, 3, 5, 7, 9 };
	std::vector<std::string> keys{ "Potatoes", "Almond", "Bread", "Coconut", "Carrot" };

	for (size_t i{}; i < keys.size(); ++i)
	{
		map.insert(keys.at(i), values.at(i));
	}

	auto result{ map.remove("Coconut") };

	if (result)	{ std::cout << "Excellent!\n\n"; } 
	else		{ std::cout << "You are very stupid!\n\n"; }

	auto result_keys{ map.get_keys() };
	
	result_keys.print();
	std::cout << '\n';

	auto result_find{ map.find("Bread") };

	if(result_find) { std::cout << "Bread value:\t" << result_find->value << "\n\n"; }
	else			{ std::cout << "Not found!\n\n"; }

	map.insert("Milk", 7);
	map.print();

	auto result_values{ map.get_values() };

	result_values.print();
	std::cout << '\n';

	map.clear();
	
}
*/