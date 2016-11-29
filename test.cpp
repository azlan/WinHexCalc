#include <assert.h>
#include "hexcalc.h"
#include "test.h"

void testHexCalc() {
#ifndef _MSC_VER 
	#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif	
	Evaluate eval;

	// Bitwise, Oct and Binary numbers
	assert(eval.Eval("0xa ^ 0xa") == 0 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("0101b^ah") == 15 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("0101b & 0110b") == 4 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("0101b | 0110b") == 7 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("011 & 12") == 8 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("011 ^ 012") == 3 && eval.GetErr() == EEE_NO_ERROR);
	
	// Hex number - 0x1a
	assert(eval.Eval("0x2Aa1") == 10913 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("0x2Aa1 + 2") == 10915 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("2 + 0x2Aa1") == 10915 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("4+0x2Aa1") == 10917 && eval.GetErr() == EEE_NO_ERROR);

	// Hex number - 1ah
	assert(eval.Eval("2Aa1h") == 10913 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("02Aa1h + 2") == 10915 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("2 + 02Aa1h") == 10915 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("4+2Aa1h") == 10917 && eval.GetErr() == EEE_NO_ERROR);

	// Some simple expressions
	assert(eval.Eval("1234") == 1234 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("1+2*3") == 7 && eval.GetErr() == EEE_NO_ERROR);

	// Parenthesis
	assert(eval.Eval("5*(4+4+1)") == 45 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("5*(2*(1+3)+1)") == 45 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("5*((1+3)*2+1)") == 45 && eval.GetErr() == EEE_NO_ERROR);

	// Spaces
	assert(eval.Eval("5 * ((1 + 3) * 2 + 1)") == 45 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("5 - 2 * ( 3 )") == -1 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("5 - 2 * ( ( 4 )  - 1 )") == -1 && eval.GetErr() == EEE_NO_ERROR);

	// Sign before parenthesis
	assert(eval.Eval("-(2+1)*4") == -12 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("-4*(2+1)") == -12 && eval.GetErr() == EEE_NO_ERROR);
	
	// Fractional numbers
	// assert(eval.Eval("1.5/5") == 0.3 && eval.GetErr() == EEE_NO_ERROR);
	// assert(eval.Eval("1/5e10") == 2e-11 && eval.GetErr() == EEE_NO_ERROR);
	// assert(eval.Eval("(4-3)/(4*4)") == 0.0625 && eval.GetErr() == EEE_NO_ERROR);
	// assert(eval.Eval("1/2/2") == 0.25 && eval.GetErr() == EEE_NO_ERROR);
	// assert(eval.Eval("0.25 * .5 * 0.5") == 0.0625 && eval.GetErr() == EEE_NO_ERROR);
	// assert(eval.Eval(".25 / 2 * .5") == 0.0625 && eval.GetErr() == EEE_NO_ERROR);
	
	// Repeated operators
	assert(eval.Eval("1+-2") == -1 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("--2") == 2 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("2---2") == 0 && eval.GetErr() == EEE_NO_ERROR);
	assert(eval.Eval("2-+-2") == 4 && eval.GetErr() == EEE_NO_ERROR);

	// === Errors ===
	// Parenthesis error
	eval.Eval("5*((1+3)*2+1");
	assert(eval.GetErr() == EEE_PARENTHESIS && strcmp(eval.GetErrPos(), "") == 0);
	eval.Eval("5*((1+3)*2)+1)");
	assert(eval.GetErr() == EEE_PARENTHESIS && strcmp(eval.GetErrPos(), ")") == 0);
	
	// Repeated operators (wrong)
	eval.Eval("5*/2");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "/2") == 0);
	
	// Wrong position of an operator
	eval.Eval("*2");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "*2") == 0);
	eval.Eval("2+");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "") == 0);
	eval.Eval("2*");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "") == 0);
	
	// Division by zero
	eval.Eval("2/0");
	assert(eval.GetErr() == EEE_DIVIDE_BY_ZERO && strcmp(eval.GetErrPos(), "/0") == 0);
	eval.Eval("3+1/(5-5)+4");
	assert(eval.GetErr() == EEE_DIVIDE_BY_ZERO && strcmp(eval.GetErrPos(), "/(5-5)+4") == 0);
	eval.Eval("2/"); // Erroneously detected as division by zero, but that's ok for us
	assert(eval.GetErr() == EEE_DIVIDE_BY_ZERO && strcmp(eval.GetErrPos(), "/") == 0);
	
	// Invalid characters
	eval.Eval("~5");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "~5") == 0);
	eval.Eval("5x");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "x") == 0);

	// Multiply errors
	eval.Eval("3+1/0+4$"); // Only one error will be detected (in this case, the last one)
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "$") == 0);
	eval.Eval("3+1/0+4");
	assert(eval.GetErr() == EEE_DIVIDE_BY_ZERO && strcmp(eval.GetErrPos(), "/0+4") == 0);
	//eval.Eval("q+1/0)"); // ...or the first one
	//assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "q+1/0)") == 0);
	eval.Eval("+1/0)");
	assert(eval.GetErr() == EEE_PARENTHESIS && strcmp(eval.GetErrPos(), ")") == 0);
	eval.Eval("+1/0");
	assert(eval.GetErr() == EEE_DIVIDE_BY_ZERO && strcmp(eval.GetErrPos(), "/0") == 0);
	
	// An emtpy string
	eval.Eval("");
	assert(eval.GetErr() == EEE_WRONG_CHAR && strcmp(eval.GetErrPos(), "") == 0);
#ifndef _MSC_VER
	#pragma GCC diagnostic pop
#endif
}

