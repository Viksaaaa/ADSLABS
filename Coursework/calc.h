#ifndef CALC_H
#define CALC_H

#include <string>
#include <utility>						// We use utility only for std::move in node constructor

namespace constants
{
	constexpr long double e = 2.718'281'828'459'045'235'360'287'471'352'662'497'757'247'093'699'959'574'966'967L;
	constexpr long double pi = 3.141'592'653'589'793'238'462'643'383'279'502'884'197'169'399'375'105'820'974'944L;
}

namespace data_structures
{
	template < typename T >
	class stack							// Trivial stack
	{
		struct node						// With nontrivial node
		{
			T data;						// Along with data we add
			size_t begin;				// Starting position (index in string) of our T (number/substring)
			size_t end;					// Ending position (index in string) of our T (number/substring)
			node *next;
			void *left;					// Left operand for an operator, if it's not an operator then it's nullptr
			void *right;				// Right operand for an operator, if it's not an operator
										// or it's a single argument function then it's nullptr
			node (T data, const size_t &begin, const size_t &end, node *next = nullptr, void *left = nullptr, void *right = nullptr) :
				data{ std::move(data) }, begin{ begin }, end{ end }, next{ next }, left{ left }, right{ right } {}
		};

		node *top_;
		size_t size_;

	public:
		stack() : top_{}, size_{} {}
		~stack() { clear(); }

		void push (const T &data, const size_t &begin = 0, const size_t &end = 0, void *left = nullptr, void *right = nullptr)
		{
			top_ = new node{ data, begin, end, top_ ? top_ : nullptr, left, right }; ++size_;
		}

		void pop()
		{
			if (top_)
			{
				const auto to_be_deleted{ top_ };
				top_ = top_->next ? top_->next : nullptr;
				--size_;
				delete to_be_deleted;
			}
		}

		void clear() { while (size_) { pop(); } }
		node *top() const { return top_; }
		size_t size() const { return size_; }
		bool empty() const { return size_ == 0; }
	};
}

namespace calculator
{
	std::string parse(const std::string &);	// If we wanna test something with predefined string via test environment
	int execute();								// Our 'main' function with interface... and cookies!
}												// Yay, cookies! Everyone likes 'em

#endif // CALC_H
