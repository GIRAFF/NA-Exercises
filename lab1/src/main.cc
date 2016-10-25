/*
 * Matrix is given in profile format.
 */

#include "main.h"
#include "SLE.h"

int main(int argc, char **argv)
{
	SLE sle;
	sle.load(std::cin);
	sle.computeL();
	sle.computeY();
	sle.computeX();
	return 0;
}
