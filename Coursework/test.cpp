#include "gtest/gtest.h"
#include "calc.h"
/*
TEST(calculator, works_1)
{
	ASSERT_EQ
	(
	calculator::parse("-5*sin (68)-4/3+cos (  23 )*cot(e)"),
	std::string{"Postfix expression:\t0 5 68 sin * - 4 3 / - 23 cos e cot * + \nResult:\t\t\t4.3389342374775284\n\n"}
	);
}

TEST(calculator, works_2)
{
	ASSERT_EQ
	(
		calculator::parse("-5*arcsin (0.1)-42/308+ln (  e )*sqrt(pi)"),
		std::string{ "Postfix expression:\t0 5 0.1 arcsin * - 42 308 / - e ln pi sqrt * + \nResult:\t\t\t1.1352531087340805\n\n" }
	);
}

TEST(calculator, works_3)
{
	ASSERT_EQ
	(
		calculator::parse("pi   + sqrt(e*(-arcsin(pi-5*pi/4))) / cos      (75)-(-8)"),
		std::string{ "Postfix expression:\tpi e 0 pi 5 pi * 4 / - arcsin - * sqrt 75 cos / + 0 8 - - \nResult:\t\t\t12.841631727236809\n\n" }
	);
}

TEST(calculator, throws_error_1)
{
	ASSERT_THROW
	(
		calculator::parse("pi   + sqrt(e*(arcsin(pi-5*pi/4))) / cos      (75)-(-8)"),
		std::runtime_error
	);

	std::string error_message{};
	try
	{
		calculator::parse("pi   + sqrt(e*(arcsin(pi-5*pi/4))) / cos      (75)-(-8)");
	}
	catch (const std::runtime_error &error)
	{
		error_message = error.what();
	}

	ASSERT_EQ(error_message, std::string("Unresolved function:\tsqrt(e*(arcsin(pi-5*pi/4)))\n"
										 "At positions:\t\t[8...34]\n"
										 "Square root argument should be greater than or equal to 0"));
}

TEST(calculator, throws_error_2)
{
	ASSERT_THROW
	(
		calculator::parse("-5-*arcsin (0.1)-42/308+ln (  e )*sqrt(pi)"),
		std::runtime_error
	);

	std::string error_message{};
	try
	{
		calculator::parse("-5-*arcsin (0.1)-42/308+ln (  e )*sqrt(pi)");
	}
	catch (const std::runtime_error &error)
	{
		error_message = error.what();
	}

	ASSERT_EQ(error_message, std::string("Unresolved operator:\t*\n"
										 "At position:\t\t4\n"
										 "No left argument present, try putting one"));
}

TEST(calculator, throws_error_3)
{
	ASSERT_THROW
	(
		calculator::parse("sqrt(-3)"),
		std::runtime_error
	);

	std::string error_message{};
	try
	{
		calculator::parse("sqrt(-3)");
	}
	catch (const std::runtime_error &error)
	{
		error_message = error.what();
	}

	ASSERT_EQ(error_message, std::string("Unresolved function:\tsqrt(-3)\n"
										 "At positions:\t\t[1...8]\n"
										 "Square root argument should be greater than or equal to 0"));
}
*/

int main() { return calculator::execute(); }