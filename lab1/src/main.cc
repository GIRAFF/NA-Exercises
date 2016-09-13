#include "main.h"
#define DEBUG

typedef enum
{
	TRIANGLE,
	PROFILE
} mtx_fmt_e;

typedef float real_t;

typedef struct
{
	size_t n;
	mtx_fmt_e fmt;
	union
	{
		real_t *tri;
		struct
		{
			real_t *di,
				*al,
				*au;
			size_t *ia;
		};
	};
	real_t elem(size_t row, size_t col);
} matrix_t;

typedef struct
{
	size_t n;
	real_t *v;
	real_t& operator[](const int idx);
} vector_t;

// TODO do the same with [][] or (,)
real_t matrix_t::elem(size_t row, size_t col)
{
	switch(fmt)
	{
		case TRIANGLE:
		{
			if(col > row) return 0;
			size_t num = row;
			for(size_t i = 1; i < row; ++i) num += i;
			num += col;
			return tri[num];
			break;
		}
		case PROFILE:
		{
			if(row == col) return di[row];
			if(row > col)
			{
				size_t len = ia[row+1] - ia[row];
				return col < row-len ? 0 :
					al[ia[row]+col-(row-len)];
			}
			size_t len = ia[col+1] - ia[col];
			return row < col-len ? 0 : au[ia[col]+row-(col-len)];
			break;
		}
	}
}

real_t& vector_t::operator[](const int idx)
{
	return v[idx];
}

void print_mtx(matrix_t m, std::ostream& out)
{
	for(size_t i = 0; i < m.n; ++i)
	{
		for(size_t j = 0; j < m.n; ++j)
			out << m.elem(i,j) << ' ';
		out << '\n';
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
	vector_t y;
	// A = LL^T
	matrix_t L;
	// because storin' L in profile form is not a good idea
	A.fmt = PROFILE;
	L.fmt = TRIANGLE;
	// get N
	std::cin >> A.n;
	// allocate memory for L
	// Note: here I'm usin' L.n as temporary variable to store
	//       the size of the array
	L.n = A.n;
	for(size_t i = 1; i < A.n; ++i) L.n += i;
	L.tri = new real_t[L.n];
	// copy n for organizational purposes
	F.n = x.n = y.n = L.n = A.n;
	// allocate some memory
	F.v = new real_t[F.n];
	x.v = new real_t[x.n];
	y.v = new real_t[y.n];
	A.di = new real_t[A.n];
	A.ia = new size_t[A.n+1];
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

#ifdef DEBUG
	std::cerr << "Here comes A:\n";
	print_mtx(A, std::cerr);
	std::cerr << '\n';
#endif

	// TODO now, let's find L
	size_t idx = 0;
	for(size_t i = 0; i < A.n; ++i)
	for(size_t j = 0; j <= i; ++j)
	{
		real_t sum = 0;
		if(i == j)
		{
			for(size_t k = 0; k < i; ++k) sum += pow(L.elem(k,i),2);
			L.tri[idx++] = sqrt(A.elem(i,i) - sum);
			continue;
		}
		for(size_t k = 0; k < i; ++k) sum += L.elem(i,k)*L.elem(j,k);
		L.tri[idx++] = (A.elem(i,j) - sum)/L.elem(j,j);
	}
	
#ifdef DEBUG
	std::cerr << "Here comes L:\n";
	print_mtx(L, std::cerr);
	std::cerr << '\n';
#endif

	// TODO find y
	for(size_t i = 0; i < y.n; ++i)
	{
		real_t sum = 0;
		for(size_t k = 0; k < i; ++k) sum += L.elem(i,k)*y[k];
		y[i] = (F[i] - sum)/L.elem(i,i);
	}
	
#ifdef DEBUG
	std::cerr << "y:\n";
	for(size_t i = 0; i < y.n; ++i) std::cerr << y[i] << ' ';
	std::cerr << '\n';
#endif

	// TODO find x
	for(int64_t i = x.n - 1; i >= 0; --i)
	{
		real_t sum = 0;
		for(size_t k = i + 1; k < x.n; ++k) sum += L.elem(k,i)*x[k];
		x[i] = (y[i] - sum)/L.elem(i,i);
	}
	
#ifdef DEBUG
	std::cerr << "x:\n";
	for(size_t i = 0; i < x.n; ++i) std::cerr << x[i] << ' ';
	std::cerr << "\n\n";
#endif

	for(size_t i = 0; i < x.n; ++i) std::cout << x[i] << '\n';

	return 0;
}
