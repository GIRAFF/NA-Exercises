#ifndef _LAB1_SLE
#define _LAB1_SLE

#include "main.h"

class SLE
{
public:
	SLE();
	void load(std::istream& in); //Ax = f
	void computeL();  //LLTx = f
	void computeY();  //Ly = f
	void computeX();  //LTx = y
	void printVec(std::ostream& out);

	int n;      //size of A and vec
	real *di;   //array for matrix diagonal elements
	real *al;   //array for matrix non-diagonal elements
	int *ia;    //array for matrix profile
	real *vec;  //array for f, y and x
};

#endif //_LAB1_SLE
