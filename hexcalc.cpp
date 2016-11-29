/*
WinHexCalc

Copyright (C) 2016 Azlan Mukhtar

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

// (c) Peter Kankowski, 2007. http://smallcode.weblogs.us mailto:kankowski@narod.ru
// http://www.strchr.com/expression_evaluator

#include "hexcalc.h"

int64_t Evaluate::Eval(char* expr)
{
	_paren_count = 0;
	_err = EEE_NO_ERROR;

	int64_t res = ParseBitwise(expr);
	// Now, expr should point to '\0', and _paren_count should be zero
	if (_paren_count != 0 || *expr == ')') {
		_err = EEE_PARENTHESIS;
		_err_pos = expr;
		return 0;
	}
	if (*expr != '\0') {
		_err = EEE_WRONG_CHAR;
		_err_pos = expr;
		return 0;
	}
	return res;
}

// Parse Bitwise
int64_t Evaluate::ParseBitwise(char* &expr)
{
	int64_t num1 = ParseSum(expr);

	for (;;) {
		// Skip spaces
		while (*expr == ' ')
			expr++;
		char op = *expr;
		if (op != '^' && op != '&' && op != '|')
			return num1;
		expr++;
		int64_t num2 = ParseSum(expr);

		if (op == '^')
			num1 ^= num2;
		else if (op == '&')
			num1 &= num2;
		else if (op == '|')
			num1 |= num2;
	}
}

// Parse add & minus
int64_t Evaluate::ParseSum(char* &expr)
{
	int64_t num1 = ParseFactors(expr);

	for (;;) {
		// Skip spaces
		while (*expr == ' ')
			expr++;
		char op = *expr;
		if (op != '-' && op != '+')
			return num1;
		expr++;
		int64_t num2 = ParseFactors(expr);

		if (op == '-')
			num1 -= num2;
		else
			num1 += num2;
	}
}

// Parse multiplication and division
int64_t Evaluate::ParseFactors(char* &expr) {
	int64_t num1 = ParseAtom(expr);
	for (;;) {
		// Skip spaces
		while (*expr == ' ')
			expr++;
		// Save the operation and position
		char op = *expr;
		char* pos = expr;
		if (op != '/' && op != '*')
			return num1;
		expr++;
		int64_t num2 = ParseAtom(expr);
		// Perform the saved operation
		if (op == '/') {
			// Handle division by zero
			if (num2 == 0) {
				_err = EEE_DIVIDE_BY_ZERO;
				_err_pos = pos;
				return 0;
			}
			num1 /= num2;
		}
		else
			num1 *= num2;
	}
}

// Parse a number or an expression in parenthesis
int64_t Evaluate::ParseAtom(char* &expr) {
	// Skip spaces
	while (*expr == ' ')
		expr++;

	// Handle the sign before parenthesis (or before number)
	bool negative = false;
	if (*expr == '-') {
		negative = true;
		expr++;
	}
	if (*expr == '+') {
		expr++;
	}

	// Check if there is parenthesis
	if (*expr == '(') {
		expr++;
		_paren_count++;
		int64_t res = ParseBitwise(expr);
		if (*expr != ')') {
			// Unmatched opening parenthesis
			_err = EEE_PARENTHESIS;
			_err_pos = expr;
			return 0;
		}
		expr++;
		_paren_count--;
		return negative ? -res : res;
	}

	// It should be a number; convert it to int
	int64_t res = ParseNumber(expr);

	// Return the result
	return negative ? -res : res;
}

int64_t Evaluate::ParseNumber(char* &expr)
{
	char* end_ptr;
	int64_t res = 0;

	std::cmatch results;

	if (std::regex_search(expr, results, std::regex("^(0[xX][0-9a-fA-F]+)|^([0-9a-fA-F]+h)")))
	{
		res = strtoull(expr, &end_ptr, 16);
		// Advance the pointer and return the result
		expr += results.length();
		return res;
	}

	// binary number - 0b1010 
	if (std::regex_search(expr, results, std::regex("^(0b[0-1]+)")))
	{
		// skip 2 characters (0b)
		expr += 2;
		res = strtoull(expr, &end_ptr, 2);
		// Advance the pointer and return the result
		expr += results.length()-2;
		return res;
	}

	// binary number - 1010b 
	if (std::regex_search(expr, results, std::regex("^([0-1]+b)")))
	{
		res = strtoull(expr, &end_ptr, 2);
		// Advance the pointer and return the result
		expr += results.length();
		return res;
	}

	// octal number - 07121
	if (std::regex_search(expr, results, std::regex("^(0[0-7]+)")))
	{
		std::cout << results.str();
		res = strtoull(expr, &end_ptr, 8);
		// Advance the pointer and return the result
		expr += results.length();
		return res;
	}

	// Decimal number
	res = strtoull(expr, &end_ptr, 10);
	if (end_ptr == expr)
	{
		// Report error
		_err = EEE_WRONG_CHAR;
		_err_pos = expr;
		return 0;
	}
	// Advance the pointer and return the result
	expr = end_ptr;
	return res;
}