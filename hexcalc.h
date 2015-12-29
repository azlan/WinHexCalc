#ifndef HEXCALC_H
#define HEXCALC_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>

// Error codes
enum EXPR_EVAL_ERR {
	EEE_NO_ERROR = 0,
	EEE_PARENTHESIS = 1,
	EEE_WRONG_CHAR = 2,
	EEE_DIVIDE_BY_ZERO = 3
};

class Evaluate{
private:
	EXPR_EVAL_ERR _err;
	char* _err_pos;
	int _paren_count;
	int ParseBitwise(char*& expr);
	int ParseSum(char*& expr);
	int ParseFactors(char*& expr);
	int ParseAtom(char* &expr);
	int ParseNumber(char* &expr);

public:

	int Eval(char* expr);

	EXPR_EVAL_ERR GetErr() {
		return _err;
	}
	char* GetErrPos() {
		return _err_pos;
	}	



};


#endif