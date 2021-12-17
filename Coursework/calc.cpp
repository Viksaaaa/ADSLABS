#include <iomanip>
#include <iostream>
#include <sstream>
#include "calc.h"

namespace
{
	/*
	first % 32 * ( middle % 32 ) * ( last % 32 ) * size
	(		: 512
	+		: 1331
	-		: 2197
	*		: 1000
	/		: 3375
	^		: 27000
	sqrt		: 27360
	ln		: 4704
	lg		: 1176
	sin		: 7182
	cos		: 2565
	tan		: 840
	cot		: 2700
	arcsin		: 1596
	arccos		: 342
	arctan		: 1680
	arccot		: 360
	*/
	enum operation_code								// first % 32 * ( middle % 32 ) * ( last % 32 ) * size
	{
		// parenthesis_left_round	= 512,			// Might need in the future, so why not
		plus = 1331,
		minus = 2197,
		multiply = 1000,
		divide = 3375,
		power = 27000,
		root_square = 27360,
		logarithm_natural = 4704,
		logarithm_decimal = 1176,
		sine = 7182,
		cosine = 2565,
		tangent = 840,
		cotangent = 2700,
		arcsine = 1596,
		arccosine = 342,
		arctangent = 1680,
		// arccotangent			= 360				// Might need in the future, so why not
	};

	enum error_type
	{
		function,
		symbols,
		operation,
		number,
		bracket,
		empty
	};

	int order(const std::string &operation)
	{
		// first % 32 * ( middle % 32 ) * ( last % 32 ) * size
		switch
			(
				operation.at(0) % 32
				* (operation.at(operation.size() - 1) % 32)
				* (operation.at(operation.size() / 2) % 32)
				* static_cast<int>(operation.size())
				)
		{
		case power:						return 3;
		case multiply:	case divide:	return 2;
		case plus:		case minus:		return 1;
			// Actually any valid function or '(', other operators/functions won't reach this 'order()' function
		default:						return 0;
		}
	}

	long double evaluate(const std::string &operation, const long double &left, const long double &right = 0.L)
	{
		// first % 32 * ( middle % 32 ) * ( last % 32 ) * size
		switch
			(
				operation.at(0) % 32
				* (operation.at(operation.size() / 2) % 32)
				* (operation.at(operation.size() - 1) % 32)
				* static_cast<int>(operation.size())
				)	// If we really need to do precise arithmetic - big_int/big_double - whatever fits us instead of double/long double
		{	// We don't want to use any abs eps because then we'll lie to ourselves that we're precise
		case plus:					return left + right;
		case minus:					return left - right;
		case multiply:				return left * right;
		case divide:
			if (right == 0.L) { throw	std::runtime_error("Divider should not be equal to 0"); }
			return	left / right;
		case power:					return	powl(left, right);
		case root_square:
			if (left < 0.L) { throw	std::runtime_error("Square root argument should be greater than or equal to 0"); }
			return	sqrtl(left);
		case logarithm_natural:
			if (left <= 0.L) { throw	std::runtime_error("Logarithm argument should be greater than 0"); }
			return	logl(left);
		case logarithm_decimal:
			if (left <= 0.L)		throw	std::runtime_error("Logarithm argument should be greater than 0");
			return	log10l(left);
		case sine:					return	sinl(left);
		case cosine:				return	cosl(left);
		case tangent:
			if (cosl(left) == 0.L)											// It won't help but we've tried at least
			{
				throw	std::runtime_error("Tangent argument should not be divisible by pi / 2");
			}
			return	tanl(left);
		case cotangent:
			if (fmodl(left, constants::pi) == 0.L || sinl(left) == 0.L)	// It won't help but we've tried at least
			{
				throw	std::runtime_error("Cotangent argument should not be divisible by pi");
			}
			return	cosl(left) / sinl(left);
		case arcsine:
			if (left < -1.L || left > 1.L) // std::abs( left ) > 1
			{
				throw	std::runtime_error("Arcsin argument should be greater than -1 and smaller than 1");
			}
			return	asinl(left);
		case arccosine:
			if (left < -1.L || left > 1.L) // std::abs( left ) > 1
			{
				throw	std::runtime_error("Arccos argument should be greater than -1 and smaller than 1");
			}
			return	acosl(left);
		case arctangent:			return	atanl(left);
			// Actually arccotangent, other operators/functions won't reach this 'evaluate()' function
		default:					return	constants::pi / 2.L - atanl(left);
		}
	}

	void evaluate_once(data_structures::stack< std::string > &operators, data_structures::stack< long double > &operands)
	{
		if (operators.top()->data.size() == 1) // + - * / ^
		{
			const long double right = operands.top()->data;
			operands.pop();
			operands.top()->data = evaluate(operators.top()->data, operands.top()->data, right);
		}
		// sin cos tan cot ln lg sqrt arcsin arccos arctan arccot
		else { operands.top()->data = evaluate(operators.top()->data, operands.top()->data); }
		operators.pop();
	}

	void bind_operations(const data_structures::stack< std::string > &operators, const data_structures::stack< long double > &operands)
	{
		if (operators.top() && operators.top()->data == "(")
		{
			for (auto *current_operator{ operators.top() }; current_operator; current_operator = current_operator->next)
			{	// We're seeking for operators which right part/function which first argument is current number
				if (current_operator->data.size() > 1 && !current_operator->left)
				{
					current_operator->left = operands.top();
				}
				else if (current_operator->data.size() == 1 && current_operator->data != "(" && !current_operator->right)
				{
					current_operator->right = operands.top();	// If it's an operator (+ - * / ^)
					break;										// we're done
				}
			}
		}
		else if (operators.top() && operators.top()->data != "(") { operators.top()->right = operands.top(); }
	}

	std::string infix_to_rpn(const std::string &infix)
	{
		std::string rpn{};
		data_structures::stack< std::string > operators{};
		for (size_t i{}; i < infix.size(); ++i)
		{
			switch (infix.at(i))
			{
			case ' ': break;	// Skip spaces
			// In case we REALLY need to implement parsing c++ stuff like
			// 4e2
			// 123.123e-3
			// .5E2f
			// etc.
			// then we will end up in a madhouse or in Microsoft office
			// Those two are distinct things, right? >:)
			// We handle somewhat ^(0|([1-9][0-9]*))(\.[0-9]+)?$
			// Negative numbers are handled via unary '-' for simplicity
			case '0': case '1': case '2': case '3': case '4':		// Numbers
			case '5': case '6': case '7': case '8': case '9':		// Numbers
			{
				size_t j{ i };
				for (; j + 1 < infix.size() && isdigit(infix.at(j + 1)); ++j) {}
				if (j + 1 != infix.size() && infix.at(j + 1) == '.')
				{
					++j;
					for (; j + 1 < infix.size() && isdigit(infix.at(j + 1)); ++j) {}
				}
				rpn.append(infix.substr(i, j - i + 1)).append(" ");
				i = j;
				break;
			}
			case 'e': case 'p': // e and pi constants
			{
				const bool pi = infix.at(i) == 'p';
				rpn.append(infix.substr(i, 1ULL + pi)).append(" ");
				i += pi;
				break;
			}
			case '-': case '+': case '*': case '/': case '^':	// + - * / ^
				while (!operators.empty() && order(std::string{ infix.at(i) }) <= order(operators.top()->data))
				{
					rpn.append(operators.top()->data).append(" ");
					operators.pop();
				}
				if (infix.at(i) == '-') // Check if it's an unary minus 
				{							// (all minus signs are treated as unary/binary operator '-')
					bool unary = i == 0;
					if (!unary)
					{
						size_t j{ i };
						while (j-- != 0 && infix.at(j) == ' ') {}
						unary = j < i && (infix.at(j) == ' ' || infix.at(j) == '(');
					}
					if (unary) { rpn.append("0 "); }
				}
				operators.push(std::string{ infix.at(i) });
				break;
			case 'a': case 'c': case 'l': case 's': case 't':	// arcsin arccos arctan arccot cos cot ln lg sin sqrt tan
			{
				const size_t distance{ infix.at(i) == 'a' ? 5ULL : infix.at(i + 1) == 'q' ? 3ULL : infix.at(i) != 'l' ? 2 : 1 };
				operators.push(infix.substr(i, distance + 1));
				i += distance;
				break;
			}
			case '(':
				operators.push("(");
				break;
			default:	// ')'
				while (operators.top()->data != "(")	// + - * / ^
				{
					rpn.append(operators.top()->data).append(" ");
					operators.pop();
				}
				operators.pop();	// remove '('
				if (operators.top() && operators.top()->data.size() > 1) // ln lg sqrt sin cos tan cot arcsin arccos arctan arccot
				{
					rpn.append(operators.top()->data).append(" ");
					operators.pop();
				}
				break;
			}
		}
		while (!operators.empty())
		{
			rpn.append(operators.top()->data).append(" ");
			operators.pop();
		}
		return rpn;
	}

	[[noreturn]] void alert									// We're not gonna use enum class instead of pure enum
	(															// since no one will ever get our error_type enum
		const error_type &type,									// and typing things like 
		const std::string &main,								// error_type::function
		const bool &flag_1 = true,					// error_type::bracket
		const bool &flag_2 = true,					// etc
		const size_t &begin = 0,					// will pollute our code with unnecessary error_type::
		const size_t &size = 0,
		const std::string &additional = ""
	)
	{
		switch (type)	// We add 1 for positions indices everywhere for convenience
		{				// No one counts from 0
		case function:	// Only some strange guys who call themselves programmers (:
			throw std::runtime_error
			{
				std::string{ "Unresolved function:\t" }.append(main.substr(begin, size + 1))
				.append("\nAt positions:\t\t[").append(std::to_string(begin + 1))
				.append("...").append(std::to_string(begin + size + 1)).append("]\n")
				.append(flag_1 ? "No argument present, try putting one" : flag_2 ? "No opening bracket" : additional)
			};
		case symbols:
			throw std::runtime_error
			{
				std::string{ "Unresolved symbol" }
				.append
				(
					flag_1	// True stands for Multiple symbols
					?
					std::string{ "s:\t" }.append(main.substr(begin, size + 1))
					.append("\nAt positions:\t\t[").append(std::to_string(begin + 1))
					.append("...").append(std::to_string(size + 1)).append("]")
					:
					std::string{":\t" }.append(main.substr(begin, 1))
					.append("\nAt position:\t\t").append(std::to_string(begin + 1))
				)
			};
		case operation:	// + - * / ^
			throw std::runtime_error
			{
				std::string{ "Unresolved operator:\t" }.append(main.substr(begin, 1))
				.append("\nAt position:\t\t").append(std::to_string(begin + 1))
				.append
				(
					flag_1
					? "\nNo left argument present, try putting one"
					: flag_2
					? "\nNo right argument present, try putting one"
					: std::string{ '\n' }.append(additional)	// Something's wrong with an operator (i.e. dividing by 0)
				)
			};
		case bracket:	// ( )
			throw std::runtime_error
			{
				std::string{ "No pair for:\t\t" }.append(flag_1 ? ")" : "(")
				.append(" bracket\nAt position:\t\t").append(std::to_string(begin + 1))
			};
		case number:	// e pi 
			throw std::runtime_error
			{
				std::string{ "No operator present\nFor number:\t\t" }
				.append(flag_1 ? "e" : flag_2 ? "pi" : main.substr(begin, size + 1))
				.append("\nAt position")
				.append
				(
					flag_1 || !size	// single digit number, so it's e or 0 1 2 3 4 5 6 7 8 9
					?
					std::string{ ":\t\t" }.append(std::to_string(begin + 1))
					:
					std::string{ "s:\t\t["}.append(std::to_string(begin + 1))
					.append("...").append(std::to_string(begin + size + 1)).append("]")
				)
			};
		case empty:	throw std::runtime_error{ "Nothing to evaluate" }; // No numbers
		}
	}

	void check_operand
	(
		const std::string &infix,
		const data_structures::stack< std::string > &operators,
		const data_structures::stack< long double > &operands,
		const bool &flag_1, // true == we're dealing with constants 'e' or 'pi', false == just numbers
		const bool &flag_2,	// true == it's 'pi'
		const size_t &current,
		const size_t &begin
	)
	{
		const std::string &s{ infix };
		if
			(
				current + flag_2 + 1 != s.size() && s.at(current + flag_2 + 1) != ' '
				&& s.at(current + flag_2 + 1) != '+' && s.at(current + flag_2 + 1) != '-' && s.at(current + flag_2 + 1) != '*'
				&& s.at(current + flag_2 + 1) != '/' && s.at(current + flag_2 + 1) != '^' && s.at(current + flag_2 + 1) != ')'
				)
		{
			alert(symbols, s, true, true, begin, current - begin + 1 + flag_2);
		}
		if
			(
				operators.empty() && operands.size() == 1 || !operators.empty()
				&& operators.top()->data.size() == 1 && operators.top()->data != "(" && operators.top()->right
				)
		{
			alert(number, s, flag_1 ? !flag_2 : flag_1, flag_2, begin, flag_1 ? flag_2 : current - begin);
		}
		if (operators.top())
		{
			size_t amount_brackets{};
			for (auto *current_operator{ operators.top() }; current_operator; current_operator = current_operator->next)
			{
				if (current_operator->data.size() > 1)
				{
					if (current_operator->left)
					{
						alert(number, s, flag_1 ? !flag_2 : flag_1, flag_2, begin, flag_1 ? flag_2 : current - begin);
					}
					else { break; }
				}
				else if (current_operator->data != "(")
				{
					if (current_operator->right)
					{
						alert(number, s, flag_1 ? !flag_2 : flag_1, flag_2, begin, flag_1 ? flag_2 : current - begin);
					}
					else { break; }
				}
				else { ++amount_brackets; }
			}
			if (operands.size() == 1 && amount_brackets == operators.size())
			{
				alert(number, s, flag_1 ? !flag_2 : flag_1, flag_2, begin, flag_1 ? flag_2 : current - begin);
			}
		}
	}

	long double validate_infix(const std::string &infix)
	{
		data_structures::stack< std::string > operators{};
		data_structures::stack< long double > operands{};
		const std::string &s{ infix };
		for (size_t i{}; i < s.size(); ++i)
		{
			switch (s.at(i))
			{
			case ' ':  break;										// Skip spaces
			case '0': case '1': case '2': case '3': case '4':		// Numbers
			case '5': case '6': case '7': case '8': case '9':		// Numbers
			{
				size_t j{ i };
				// Search for other digits before . if the first digit of a number is not 0
				if (s.at(j) != '0') { for (; j + 1 < s.size() && isdigit(s.at(j + 1)); ++j) {} }
				if (j + 1 != s.size() && s.at(j + 1) == '.') // Fractional part
				{
					if (++j + 1 == s.size()) { alert(symbols, s, true, true, i, j - i); }
					if (!isdigit(s.at(j + 1))) { alert(symbols, s, true, true, i, j - i + 1); }
					for (; j + 1 < s.size() && isdigit(s.at(j + 1)); ++j) {}
				}
				check_operand(s, operators, operands, false, false, j, i);
				operands.push(std::stold(s.substr(i, j - i + 1)));	// Pushing number to operators stack finally!
				bind_operations(operators, operands);
				i = j;
				break;
			}
			case 'e': case 'p':	// e or pi constant
			{
				const bool pi = s.at(i) == 'p';
				if (pi && i + 1 == s.size()) { alert(symbols, s, false, true, i); }
				check_operand(s, operators, operands, true, pi, i, i);
				operands.push(pi ? constants::pi : constants::e);		// Pushing e or pi constant to operators stack finally!
				bind_operations(operators, operands);
				i += pi;
				break;
			}
			case '+': case '-': case '*': case '/': case '^':	// + - * / ^
			{
				bool potential_minus_unary = s.at(i) == '-' && i == 0;
				if (!potential_minus_unary)
				{
					size_t j{ i };
					while (j-- != 0 && infix.at(j) == ' ') {}
					potential_minus_unary = j < i && (infix.at(j) == ' ' || infix.at(j) == '(');
				}
				if (potential_minus_unary)	// Unary minus solution
				{
					operands.push(0.L);
					bind_operations(operators, operands);
				}
				else							// Binary + - * / ^
				{
					if (operands.empty() || !operators.empty() && operators.top()->left == operands.top())
					{
						alert(operation, s, true, true, i);
					}
					while (!operators.empty() && order(std::string{ s.at(i) }) <= order(operators.top()->data))
					{
						if (operators.top()->data.size() == 1 && operators.top()->data == "(" && !operators.top()->right)
						{
							alert(operation, s, false, true, operators.top()->begin);
						}
						try { evaluate_once(operators, operands); }
						catch (std::runtime_error &error)
						{
							alert(operation, s, false, false, operators.top()->begin, 0, error.what());
						}
					}
				}
				operators.push(std::string{ s.at(i) }, i, i, operands.top());	// Push operator to operators stack
				break;
			}
			case 'a': case 'c': case 'l': case 's': case 't':	// arcsin/arccos/arctan/arccot/cos/cot/ln/lg/sin/sqrt/tan
			{
				const size_t distance{ s.size() - i };
				if (distance == 1) { alert(symbols, s, false, true, i); }
				switch (s.at(i))
				{
				case 'l':									// ln lg
					if (distance == 2 && s.substr(i, 2) != "ln" && s.substr(i, 2) != "lg")
					{
						alert(symbols, s, true, true, i, 2);
					}
					break;
				case 'c': case 's': case 't':				// cos cot sin sqrt tan
					switch (s.at(i + 1))
					{
					case 'q':								// sqrt
						if (distance < 4 || s.substr(i, 4) != "sqrt")
						{
							alert(symbols, s, true, true, i, 3ULL < distance - 1 ? 3ULL : distance - 1);
						}
						break;
					default:								// cos cot sin tan
						if
							(
								distance < 3
								|| s.substr(i, 3) != "sin" && s.substr(i, 3) != "cos"
								&& s.substr(i, 3) != "tan" && s.substr(i, 3) != "cot"
								)
						{
							alert(symbols, s, true, true, i, 2ULL < distance - 1 ? 2ULL : distance - 1);
						}
					}
					break;
				default:									// arcsin arccos arctan arccot
					if
						(
							distance < 5
							|| s.substr(i, 6) != "arcsin" && s.substr(i, 6) != "arccos"
							&& s.substr(i, 6) != "arctan" && s.substr(i, 6) != "arccot"
							)
					{
						alert(symbols, s, true, true, i, 5ULL < distance - 1 ? 5ULL : distance - 1);
					}
				}
				const size_t end{ s.at(i) == 'a' ? 6ULL : s.at(i + 1) == 'q' ? 4ULL : s.at(i) != 'l' ? 3 : 2 };
				// Pushing ln/lg/sqrt/sin/cos/cot/tan/arcsin/arccos/arctan/arccot to operators stack
				operators.push(s.substr(i, end), i, i + end - 1);
				i += end - 1;
				size_t j{ i };				// Checking for '(' located after every function
				while (j++ + 1 != s.size() && s.at(j) == ' ') {}
				if (j == s.size() || s.at(j) != '(')
				{
					alert(function, s, false, true, operators.top()->begin, operators.top()->end);
				}
				operators.push("(", i);	// Pushing '(' located after every function to operators stack
				i = j;
				break;
			}
			case '(': // (
				operators.push("(", i);
				break;
			case ')': // )
				while (!operators.empty() && operators.top()->data != "(")	// + - * / ^
				{
					if (!operators.top()->right) { alert(operation, s, false, true, operators.top()->begin); }
					try { evaluate_once(operators, operands); }			// We evaluate
					catch (std::runtime_error &error)
					{
						alert(operation, s, false, false, operators.top()->begin, 0, error.what());
					}
				}
				if (operators.empty()) { alert(bracket, s, true, true, i); }
				operators.pop();											// We pop '('
				if (operators.top() && operators.top()->data.size() > 1)	// We're trying to deal with a potential function
				{
					if (!operators.top()->left)
					{
						alert(function, s, true, false, operators.top()->begin, i - operators.top()->begin);
					}
					try { operands.top()->data = evaluate(operators.top()->data, operands.top()->data); }
					catch (std::runtime_error &error)
					{
						alert(function, s, false, false, operators.top()->begin, i - operators.top()->begin, error.what());
					}
					operators.pop();
				}
				break;
			default:									alert(symbols, s, false, true, i); // Not a valid symbol at all
			}
		}
		while (!operators.empty())	// Clear the operators stack 'til it's empty
		{
			if (operators.top()->data == "(") { alert(bracket, s, false, true, operators.top()->begin); }
			if (operators.top()->data.size() == 1 && !operators.top()->right)
			{
				alert(operation, s, false, true, operators.top()->begin);
			}
			try { evaluate_once(operators, operands); }
			catch (std::runtime_error &error) { alert(operation, s, false, false, operators.top()->begin, 0, error.what()); }
		}
		// No digits/operators
		// Just parentheses or spaces or spaces with parentheses in correct order
		// So we check that
		if (operands.empty()) { alert(empty, s); }
		return operands.top()->data;
	}
}

namespace calculator
{
	std::string parse(const std::string &input)
	{
		try
		{
			std::ostringstream stream;
			const long double result = validate_infix(input);
			stream << std::setprecision(sizeof(long double) * 2 + 1)
				<< "Postfix expression:\t" << infix_to_rpn(input)
				<< "\nResult:\t\t\t" << result << "\n\n";
			return stream.str();
		}
		catch (...) { throw; }
	}

	int execute()
	{
		std::cout
			<< "Welcome to my calculator!\n\n"
			<< "Please enter infix expressions using integer or floating-point numbers.\n"
			<< "Available operators are:\n\t"
			<< "+\n\t" << "-\n\t" << "*\n\t" << "/\n\t" << "^\n\n"
			<< "Available functions are:\n\t"
			<< "sqrt\n\t" << "ln\n\t" << "lg\n\t"
			<< "sin\n\t" << "cos\n\t" << "tan\n\t" << "cot\n\t"
			<< "arcsin\n\t" << "arccos\n\t" << "arctan\n\t" << "arccot\n"
			<< "Trigonometric functions work with radians\n\n"
			<< "Available constants are:\n\t"
			<< "e\n\t" << "pi\n\n"
			<< "Output:\n\t"
			<< "Correct input:   postfix version of the initial expression along with it's result;\n\t"
			<< "Incorrect input: description of first error.\n\n"
			<< "To quit type 'q' or 'quit' (without quotes) and hit Enter.\nEnjoy!\n\n";
		while (true)
		{
			std::cout << "Input:\t\t\t";
			std::string input{};
			std::getline(std::cin, input);
			if (input == "q" || input == "quit") { break; }
			try									 { std::cout << parse(input); }
			catch (std::runtime_error &error)	 { std::cerr << error.what() << "\n\n" << std::flush; }
		}
		std::cout << "\nGood bye!\n";
		return 0;
	}
}
