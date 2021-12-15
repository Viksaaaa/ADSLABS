#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "calc.h"

namespace constants
{
	const long double e = std::exp(1.0);
	const long double pi = std::acos(-1.0);
}

namespace
{
	template < typename T >
	class stack
	{
	protected:
		struct node
		{
			T data;
			size_t begin;
			size_t end;
			node *next;
			node *left;
			node *right;

			node
			(
				T data,
				const size_t &begin,
				const size_t &end,
				node *next = nullptr,
				node *left = nullptr,
				node *right = nullptr
			) :
				data{ std::move(data) },
				begin{ begin },
				end{ end },
				next{ next },
				left{ left },
				right{ right } {}
		};

		node *top_;
		size_t size_;

	public:
		stack()
			: top_{}, size_{} {}

		~stack()
		{
			clear();
		}

		void push
		(
			const T &data,
			const size_t &begin = 0,
			const size_t &end = 0,
			node *left = nullptr,
			node *right = nullptr
		)
		{
			top_ = new node{ data, begin, end, top_ ? top_ : nullptr, left, right };
			++size_;
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

		void clear()
		{
			while (size_)
			{
				pop();
			}
		}

		node *top() const
		{
			return top_;
		}

		size_t size() const
		{
			return size_;
		}

		bool empty() const
		{
			return size_ == 0;
		}
	};

	int order(const std::string &operation)
	{
		if (operation == "^")
		{
			return 3;
		}
		if (operation == "*" || operation == "/")
		{
			return 2;
		}
		if (operation == "+" || operation == "-")
		{
			return 1;
		}
		return 0; // ( sin cos tan cot ln lg sqrt arcsin arccos arctan arccot
	}

	long double evaluate(const std::string &operation, const long double &left, const long double &right = 0.0)
	{
		if (operation == "+")
		{
			return left + right;
		}
		if (operation == "-")
		{
			return left - right;
		}
		if (operation == "*")
		{
			return left * right;
		}
		if (operation == "/")
		{
			if (right == 0.0)
			{
				throw std::runtime_error("Divider should not be equal to 0");
			}
			return left / right;
		}
		if (operation == "^")
		{
			return pow(left, right);
		}
		if (operation == "cos")
		{
			return cos(left);
		}
		if (operation == "sin")
		{
			return sin(left);
		}
		if (operation == "tan")
		{
			if (cos(left) == 0.0)
			{
				throw std::runtime_error("Tangent argument should not be divisible by pi / 2");
			}
			return tan(left);
		}
		if (operation == "cot")
		{
			if (sin(left) == 0.0)
			{
				throw std::runtime_error("Cotangent argument should not be divisible by pi");
			}
			return cos(left) / sin(left);
		}
		if (operation == "ln")
		{
			if (left <= 0.0)
			{
				throw std::runtime_error("Logarithm argument should be greater than 0");
			}
			return log(left);
		}
		if (operation == "lg")
		{
			if (left <= 0.0)
			{
				throw std::runtime_error("Logarithm argument should be greater than 0");
			}
			return log10(left);
		}
		if (operation == "sqrt")
		{
			if (left < 0.0)
			{
				throw std::runtime_error("Square root argument should be greater than or equal to 0");
			}
			return sqrt(left);
		}
		if (operation == "arcsin")
		{
			if (left < -1.0 || left > 1.0)
			{
				throw std::runtime_error("Arcsin argument should be greater than -1 and smaller than 1");
			}
			return asin(left);
		}
		if (operation == "arccos")
		{
			if (left < -1.0 || left > 1.0)
			{
				throw std::runtime_error("Arccos argument should be greater than -1 and smaller than 1");
			}
			return acos(left);
		}
		if (operation == "arctan")
		{
			return atan(left);
		}
		if (operation == "arccot")
		{
			return constants::pi / 2 - atan(left);
		}
		throw std::runtime_error(std::string{ "Unknown operator: " }.append(operation));
	}

	std::string infix_to_rpn(const std::string &infix)
	{
		std::string rpn{};
		stack< std::string > operators{};

		for (size_t i{}; i < infix.size(); ++i)
		{
			switch (infix.at(i))
			{
			case ' ':
			{
				break;
			}
			// in case we REALLY need to implement parsing c++ stuff like
			// 4e2
			// 123.123e-3
			// .5E2f
			// etc.
			// then we will end up in a madhouse or in Microsoft office
			// those two are distinct things, right? >:)
			// we handle somewhat ^(0|([1-9][0-9]*))(\.[0-9]+)?$
			// negative numbers are handled via unary '-' for simplicity
			case '0': case '1': case '2': case '3': case '4':		// numbers
			case '5': case '6': case '7': case '8': case '9':		// numbers
			{
				size_t j{ i };

				for (; j + 1 < infix.size() && isdigit(infix.at(j + 1)); ++j) {}

				if (j + 1 != infix.size() && infix.at(j + 1) == '.')
				{
					++j;

					for (; j + 1 < infix.size() && isdigit(infix.at(j + 1)); ++j) {}

					rpn.append(infix.substr(i, j - i + 1));
				}
				else
				{
					rpn.append(infix.substr(i, j - i + 1));
				}
				rpn.append(" ");

				i = j;

				break;
			}
			case 'e': // e constant
			{
				rpn
					.append(std::string{ infix.at(i) })
					.append(" ");

				break;
			}
			case 'p': // pi constant
			{
				rpn
					.append(std::string{ infix.at(i) })
					.append(std::string{ infix.at(i + 1) })
					.append(" ");

				++i;

				break;
			}
			case '(':
			{
				operators.push(std::string{ infix.at(i) });

				break;
			}
			case 'c': case 't':		// cos cot tan
			{
				operators.push
				(
					std::string{ infix.at(i) }
					.append(std::string{ infix.at(i + 1) })
					.append(std::string{ infix.at(i + 2) })
				);

				i += 2;

				break;
			}
			case 's':	// sin sqrt
			{
				switch (infix.at(i + 1))
				{
				case 'i':	// sin
				{
					operators.push
					(
						std::string{ infix.at(i) }
						.append(std::string{ infix.at(i + 1) })
						.append(std::string{ infix.at(i + 2) })
					);

					i += 2;

					break;
				}
				default:	// sqrt
				{
					operators.push
					(
						std::string{ infix.at(i) }
						.append(std::string{ infix.at(i + 1) })
						.append(std::string{ infix.at(i + 2) })
						.append(std::string{ infix.at(i + 3) })
					);

					i += 3;

					break;
				}
				}
				break;
			}
			case 'l':	// lg ln
			{
				operators.push
				(
					std::string{ infix.at(i) }
					.append(std::string{ infix.at(i + 1) })
				);

				++i;

				break;
			}
			case 'a':	// arcsin arccos arctan arccot
			{
				operators.push
				(
					std::string{ infix.at(i) }
					.append(std::string{ infix.at(i + 1) })
					.append(std::string{ infix.at(i + 2) })
					.append(std::string{ infix.at(i + 3) })
					.append(std::string{ infix.at(i + 4) })
					.append(std::string{ infix.at(i + 5) })
				);

				i += 5;

				break;
			}
			case '-': case '+': case '*': case '/': case '^':	// - + * / ^
			{
				while (!operators.empty() && order(std::string{ infix.at(i) }) <= order(operators.top()->data))
				{
					rpn.append(operators.top()->data).append(" ");

					operators.pop();
				}
				if (infix.at(i) == '-' && (i == 0 || infix.at(i - 1) == '(')) // unary -
				{
					rpn.append("0 ");
				}
				operators.push(std::string{ infix.at(i) });

				break;
			}
			default: // )
			{
				while (operators.top()->data != "(")
				{
					rpn.append(operators.top()->data).append(" ");

					operators.pop();
				}
				operators.pop(); // remove (

				if
					(
						operators.top()
						&& operators.top()->data != "+"
						&& operators.top()->data != "-"
						&& operators.top()->data != "*"
						&& operators.top()->data != "/"
						&& operators.top()->data != "^"
						&& operators.top()->data != "("
						)
				{
					// add sin cos tan cot ln lg sqrt arcsin arccos arctan arccot to output
					rpn.append(operators.top()->data).append(" ");
					// remove it from stack
					operators.pop();
				}
			}
			}
		}
		while (!operators.empty())
		{
			rpn.append(operators.top()->data).append(" ");

			operators.pop();
		}
		return rpn;
	}

	long double evaluate_rpn(const std::string &rpn)
	{
		stack< long double > operands{};

		for (size_t i{}; i < rpn.size(); ++i)
		{
			switch (rpn.at(i))
			{
			case '0': case '1': case '2': case '3': case '4':		// numbers
			case '5': case '6': case '7': case '8': case '9':		// numbers
			{
				size_t j{ i };

				for (; rpn.at(j) != ' '; ++j) {}

				operands.push(std::stold(rpn.substr(i, j - i)));

				i = j;

				break;
			}
			case 'e':	// e constant
			{
				operands.push(constants::e);

				++i;

				break;
			}
			case 'p':	// pi constant
			{
				operands.push(constants::pi);

				i += 2;

				break;
			}
			case '+': case '-': case '*': case '/': case '^':	// + - * / ^
			{
				const long double right{ operands.top()->data };

				operands.pop();

				operands.top()->data = evaluate(std::string{ rpn.at(i) }, operands.top()->data, right);
				++i;

				break;
			}
			case 'a':	// arcsin arccos arctan arccot
			{
				operands.top()->data = evaluate
				(
					std::string{ rpn.at(i) }
					.append(std::string{ rpn.at(i + 1) })
					.append(std::string{ rpn.at(i + 2) })
					.append(std::string{ rpn.at(i + 3) })
					.append(std::string{ rpn.at(i + 4) })
					.append(std::string{ rpn.at(i + 5) }),
					operands.top()->data
				);
				i += 6;

				break;
			}
			case 'l':	// ln lg
			{
				operands.top()->data = evaluate
				(
					std::string{ rpn.at(i) }
					.append(std::string{ rpn.at(i + 1) }),
					operands.top()->data
				);
				i += 2;

				break;
			}
			default:	// sin cos tan cot sqrt
			{
				if (rpn.at(i + 1) == 'q')	// sqrt
				{
					operands.top()->data = evaluate
					(
						std::string{ rpn.at(i) }
						.append(std::string{ rpn.at(i + 1) })
						.append(std::string{ rpn.at(i + 2) })
						.append(std::string{ rpn.at(i + 3) }),
						operands.top()->data
					);

					i += 4;
				}
				else	// sin cos tan cot
				{
					operands.top()->data = evaluate
					(
						std::string{ rpn.at(i) }
						.append(std::string{ rpn.at(i + 1) })
						.append(std::string{ rpn.at(i + 2) }),
						operands.top()->data
					);
					i += 3;
				}
				break;
			}
			}
		}
		return operands.top()->data;
	}

	bool process()
	{
		std::cout << "Input:\t\t\t";

		std::string input{};

		std::getline(std::cin, input);

		if (input == "q" || input == "quit")
		{
			return false;
		}
		stack< std::string > operators{};
		stack< long double > operands{};

		std::string rpn{ infix_to_rpn(input) };

		std::cout
			<< "\nInitial expression:\t"
			<< input
			<< "\nPostfix expression:\t"
			<< rpn
			<< "\nResult:\t\t\t"
			<< std::setprecision(16)
			<< evaluate_rpn(rpn)
			<< "\n\n";
		/*
		for ( size_t i{}; i < input.size(); ++i )
		{
			// TODO validation prolly move it to other function
		}
		// TODO validation
		if ( !operands.top() )
		{
			// cases:
			// "()()"
			// "(((  )))"
			// "                    "
			// "             (  )   "
			std::cout << "Nothing to evaluate.\n";
		}
		else
		{
			std::string rpn{ infix_to_rpn( input ) };

			std::cout
			<< "\nInitial expression:\t"
			<< input
			<< "\nPostfix expression:\t"
			<< rpn
			<< "\nResult:\t\t\t"
			<< std::setprecision( 16 )
			<< evaluate_rpn( rpn )
			<< "\n\n";
		}*/
		return true;
	}
}

namespace calculator
{
	int execute()
	{
		std::cout
			<< "Welcome to my calculator!\n\n"
			<< "Please enter infix expressions using integer or floating-point numbers.\n"
			<< "Available operators are:\n\t"
			<< "+\n\t"
			<< "-\n\t"
			<< "*\n\t"
			<< "/\n\t"
			<< "^\n\t"
			<< "sin\n\t"
			<< "cos\n\t"
			<< "tan\n\t"
			<< "cot\n\t"
			<< "ln\n\t"
			<< "lg\n\t"
			<< "sqrt\n\t"
			<< "arcsin\n\t"
			<< "arccos\n\t"
			<< "arctan\n\t"
			<< "arccot\n\n"
			<< "Available constants are:\n\t"
			<< "e\n\t"
			<< "pi\n\n"
			<< "Output:\n"
			<< "\tCorrect input: postfix version of the initial expression along with it's result;\n"
			<< "\tIncorrect input: first error position along with it's description.\n\n"
			<< "To quit type 'q' or 'quit' (without quotes) and hit Enter.\nEnjoy!\n\n";

		for (bool stay{ true }; stay; )
		{
			try
			{
				stay = process();
			}
			catch (std::runtime_error &error)
			{
				std::cout << error.what() << std::endl;
			}
		}
		std::cout << "\nGood bye!\n";

		return 0;
	}
}
