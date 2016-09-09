#include "main.h"

typedef enum
{
	TRIANGLE,
	PROFILE
} mtx_fmt_e;

typedef real_t float;

typedef struct
{
	size_t n;
	mtx_fmt_e fmt;
	union
	{
		real_t *tri = nullptr;
		struct
		{
			real_t *di = nullptr,
				*ia = nullptr,
				*al = nullptr,
				*au = nullptr;
		};
	};
	real_t elem(size_t row, size_t col);
} matrix_t;

typedef struct
{
	size_t n;
	real_t *v;
} vector_t;

real_t matrix_t::elem(size_t row, size_t col)
{
	switch(fmt)
	{
		case TRIANGLE:
			if(col > row) return 0;
			size_t num = row;
			for(size_t i = 1; i < row; ++i) num += i;
			num += col;
			return tri[num];
		break;
		case PROFILE:
			// TODO get what we need
		break;
	}
}

/*
 * Input format:
 * N
 * ia_1, ia_2, ... ia_N, ia_{N+1}
 * di_1, di_2, ... di_N
 * al_1, al_2, ... al_{ia_{N+1}}
 * au_1, au_2, ... au_{ia_{N+1}}
 */

int main(int argc, char **argv)
{
	// Ax = F, remember?
	matrix_t A;
	vector_t F;
	vector_t x;
	// A = LL^T
	matrix_t L;
	// because keepin' L in profile form is not a good idea
	A.fmt = mtx_fmt_e.PROFILE;
	L.fmt = mtx_fmt_e.TRIANGLE;
	// get N
	std::cin >> A.n;
	// allocate memory for L
	// Note: here I'm usin' L.n as temporary variable to store
	//       the size of the array
	L.n = A.n;
	for(size_t i = 1; i < A.n; ++i) L.n += i;
	L.tri = new real_t[L.n];
	// copy n for organizational purposes
	F.n = x.n = L.n = A.n;
	// allocate some memory
	F.v = new real_t[F.n];
	A.di = new real_t[A.n];
	A.ia = new real_t[A.n+1];
	// read profile
	for(size_t i = 0; i < A.n+1; ++i)
		std::cin >> A.ia[i];
	// allocate some more memory
	A.al = new real_t[A.ia[A.n]],
	A.au = new real_t[A.ia[A.n]];
	// read all the stuff
	for(size_t i = 0; i < A.n; ++i)
		std::cin >> A.di[i];
	for(size_t i = 0; i < A.ia[A.n]; ++i)
		std::cin >> A.al[i];
	for(size_t i = 0; i < A.ia[A.n]; ++i)
		std::cin >> A.au[i];
	for(size_t i = 0; i < F.n; ++i)
		std::cin >> F.v[i];

	// TODO now, let's find L
	// TODO find y
	// TODO find x

	return 0;
}
