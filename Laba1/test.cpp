#include "gtest/gtest.h"
#include "rbtree.h"

/*
 *
 *Отчет по лабораторной работе должен содержать:
1. Титульный лист.
2. Постановка задачи.
3. Описание реализуемого класса и методов.
4. Оценка временной сложности каждого метода.
5. Описание реализованных unit-тестов.
6. Пример работы.
7. Листинг.
*/

/*
 * Реализовать шаблонный ассоциативный массив (map) на основе красно-
черного дерева. Наличие unit-тестов ко всем реализуемым методам является
обязательным требованием.
Список методов:
+    1. insert(ключ, значение)    // добавление элемента с ключом и значением
+    2. remove(ключ)                // удаление элемента по ключу
+    3. find(ключ)                // поиск элемента по ключу
+    4. clear                    // очищение ассоциативного массива
+    5. get_keys                    // возвращает список ключей
+    6. get_values                // возвращает список значений
+    7. print                    // вывод в консоль
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

	const std::vector<std::string> new_keys{ "Potatoes", "Almond", "Bread", "Bread", "Bread", "", "" };
	for (size_t i{}; i < new_keys.size(); ++i)
	{

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
//int main()
//{
//    map<std::string, int> map{};
//    map.insert("Milk", 2);
//
//    map.print();
//}
