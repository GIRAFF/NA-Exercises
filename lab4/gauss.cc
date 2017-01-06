#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>

#define SIZE 6

typedef double real;

real f[SIZE] = {1,2,3,4,5,6};
real **a;

void gauss(real *res)
{
	// reduction
	for (uint k = 0; k < SIZE; ++k) {
		uint i_max = k;
		for (uint i = k + 1; i < SIZE; ++i) {
			if (a[i][k] > a[i_max][k])
				i_max = i;
		}
		assert(a[i_max][k] != 0);
		std::swap(a[k], a[i_max]);
		std::swap(f[k], f[i_max]);
		for (uint i = k+1; i < SIZE; ++i) {
			real fr = a[i][k] / a[k][k];
			for (uint j = k+1; j < SIZE; ++j)
				a[i][j] -= a[k][j]*fr;
			f[i] -= f[k]*fr;
			a[i][k] = 0;
		}
	}
	for (int k = SIZE-1; k >= 0; --k) {
		res[k] = f[k];
		for (uint i = k+1; i < k; ++i)
			res[k] -= a[k][i]*res[i];
		res[k] /= a[k][k];
	}
}

void print_mtx(real **a)
{
	for (uint i = 0; i < SIZE; ++i) {
		for (uint j = 0; j < SIZE; ++j)
			std::cout << a[i][j] << ' ';
		std::cout << std::endl;
	}
}

int main()
{
	real *x = new real[SIZE];
	a = new real*[SIZE];
	for (uint i = 0; i < SIZE; ++i)
		a[i] = new real[SIZE];

	srand(time(NULL));
	for (uint i = 0; i < SIZE; ++i)
		for (uint j = 0; j < SIZE; ++j)
			//a[i][j] = rand() % 10 + 1;
			a[i][j] = i != j ? 0 : 1;

	print_mtx(a);
	std::cout << std::endl;
	gauss(x);
	for (uint i = 0; i < SIZE; ++i)
		std::cout << x[i] << ' ';
	std::cout << std::endl;
	return 0;
}
