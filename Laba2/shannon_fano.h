#pragma once

/*
Реализовать кодирование и декодирование по алгоритму Шеннона-Фано
входной строки, вводимой через консоль
2. Посчитать объем памяти, который занимает исходная и закодированная
строки
3. Выводить на экран таблицу частот и кодов, результат кодирования и
декодирования, коэффициент сжатия
4. Стандартные структуры данных C++ использовать нельзя. Необходимо
использовать структуры данных из предыдущих лабораторных работ

Наличие unit-тестов является обязательным требованием.

Отчет по лабораторной работе должен содержать:
1. Титульный лист с указанием варианта задания, ФИО студента и
преподавателя
2. Краткое описание реализуемого алгоритма и используемых структур
данных
3. Оценки временной сложности реализуемых алгоритмов
4. Демонстрация 3х примеров работы программы на разных входных
строках (например, цитаты классиков)
5. Листинг*/

#include <iostream>
#include "rbtree.h"

class shannon_fano
{
	struct cell
	{
		char character{};
		size_t frequency{};
		double probability{};

		explicit cell(const char &character = '\0',
			const size_t &frequency = 0LL,
			const double &probability = 0.) :
			character{ character },
			frequency{ frequency },
			probability{ probability } {}
	};

	static int comparator(const void *first, const void *second)
	{
		const cell left{ *static_cast<const cell*>(first) };
		const cell right{ *static_cast<const cell*>(second) };
		return
			left.frequency < right.frequency ? 1 :
			left.frequency > right.frequency ? -1 :
			left.character < right.character ? 1 :
			-1;
	}

	void make_codes(const int &left, const int &right)
	{
		if (left == right) { return; }
		if (right - left == 1)
		{
			codes_.find(table_[left].character)->value += '0';
			codes_.find(table_[right].character)->value += '1';
			return;
		}
		double half = 0;
		for (int i = left; i <= right; ++i) { half += table_[i].probability; }
		half *= 0.5;
		int pivot = -1;
		double current = 0;
		for (int i = left; i <= right; ++i)
		{
			current += table_[i].probability;
			if (current <= half) { codes_.find(table_[i].character)->value += '0'; }
			else
			{
				codes_.find(table_[i].character)->value += '1';
				if (pivot < 0) pivot = i;
			}
		}
		if (pivot < 0) { pivot = left + 1; }
		make_codes(left, pivot - 1);
		make_codes(pivot, right);
	}

	static constexpr int ascii_size = 256;
	int count_{};
	size_t size_input_{};
	size_t size_output_{};
	std::string input_;
	std::string coded_;
	std::string decoded_;
	cell table_[ascii_size];
	map<char, std::string> codes_{};

public:
	shannon_fano() = default;
	~shannon_fano() = default;

	void encode(const std::string &input)
	{
		size_input_ = input.size();
		if (!size_input_) { std::cout << "Nothing to encode.\n"; return; }
		for (int i{}; i < count_; ++i) { table_[i].frequency = 0; }
		count_ = 0;
		codes_.clear(); coded_.clear(); decoded_.clear();
		input_ = input;
		for (size_t i{}; i < size_input_; ++i)
		{
			const char character = input.at(i);
			const auto index = static_cast<unsigned char>(character);
			codes_.insert(character, "");
			if (!table_[index].frequency) { ++count_; }
			++table_[index].frequency;
			table_[index].character = character;
		}
		std::qsort(table_, ascii_size, sizeof(cell), comparator);
		for (int i = 0; i < count_; ++i)
		{
			table_[i].probability = static_cast<double>(table_[i].frequency) / static_cast<double>(size_input_);
		}
		make_codes(0, count_ - 1);
		for (size_t i{}; i < size_input_; ++i)
		{
			coded_ += codes_.find(input.at(i))->value;
		}
		size_output_ = coded_.size();
	}

	void info()
	{
		if (!size_input_) { std::cout << "No info.\n"; return; }
		const size_t input_bits{ size_input_ * 8 };
		std::cout
			<< "Input was:\t\t" << input_
			<< "\nInput contains:\t\t" << size_input_
			<< " characters\nThat's:\t\t\t" << input_bits
			<< " bits.\nOutput is:\t\t" << size_output_
			<< " bits.\nCompression factor:\t" << static_cast<double>(input_bits) / static_cast<double>(size_output_)
			<< "\nCoded message:\t\t" << coded_
			<< "\nDecoded message:\t\t" << decoded_
			<< "\nTable of frequencies:\n";
		for (int i{}; i < count_; ++i)
		{
			const char character = table_[i].character;
			std::cout
				<< "Character:\t'" << character
				<< "'\tFrequency:\t" << table_[i].frequency
				<< "\tProbability:\t" << table_[i].probability
				<< "\tCode:\t" << codes_.find(character)->value
				<< '\n';
		}
		std::cout << '\n';
	}
};
