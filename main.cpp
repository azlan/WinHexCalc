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
		fgets(buff, sizeof(buff), stdin);
		buff[strcspn(buff, "\r\n")] = 0; // remove trailing newline

		// If the string is empty, then exit
		if(*buff == '\0')
			return 0;

		// Evaluate the expression
		Evaluate eval;
		double res = eval.Eval(buff);
		if(eval.GetErr() != EEE_NO_ERROR) {
			printf("  Error: %s at %s\n", errors[eval.GetErr()], eval.GetErrPos());
		} else {
			printf("  = %g\n", res);
		}
	}
}

