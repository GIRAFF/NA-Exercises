/*
 * Matrix is given in profile format.
 */

#include "main.h"
#include "SLE.h"

#ifdef USE_DOUBLE
	#define PREC 16
#else
	#define PREC 7
#endif

int main(int argc, char **argv)
{
	std::cerr << std::setprecision(PREC);
	std::cout << std::setprecision(PREC);
	SLE sle;
	sle.load(std::cin);
	sle.computeL();
	sle.computeY();
	sle.computeX();
	sle.printVec(std::cout);
	return 0;
}
