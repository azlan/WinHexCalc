#include "hexcalc.h"
#include "test.h"

int main()
{

//#ifdef _DEBUG
 	testHexCalc();
// #endif
	static const char *errors[] = {
		"no error",
		"parentheses don't match",
		"invalid character",
		"division by zero"};

	puts("Enter an expression (or an empty string to exit):");
	for(;;) 
	{
		// Get a string from console
		static char buff[256];
		char *expr = gets(buff);

		// If the string is empty, then exit
		if(*expr == '\0')
			return 0;

		// Evaluate the expression
		Evaluate eval;
		double res = eval.Eval(expr);
		if(eval.GetErr() != EEE_NO_ERROR) {
			printf("  Error: %s at %s\n", errors[eval.GetErr()], eval.GetErrPos());
		} else {
			printf("  = %g\n", res);
		}
	}
}

