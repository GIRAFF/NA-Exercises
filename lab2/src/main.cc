#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define N      9
#define ZRODI  4
#define JACOBI 0
#define GAUSS  1

typedef float real;

class SLE
{
public:
	SLE() {}
	~SLE()
	{
		for (uint i = 0; i < N; ++i) free(di[i]);
		free(di);
		free(x);
		free(jx);
		free(f);
	}
	/* Input file:
	 *
	 * n m wax_param epsilon max_iter
	 * <diagonals>
	 * f_0 f_1 ... f_n
	 * x_0 x_1 ... x_n
	 */
	void init(std::istream& in)
	{
		in >> n >> m >> w >> eps >> max;
		di = (real**)malloc(N*sizeof(real*));
		for (uint i = 0; i < N; ++i)
			di[i] = (real*)malloc(n*sizeof(real));
		// TODO optimize it
		for (uint i = 0; i < N; ++i)
			for (uint j = 0; j < n; ++j)
				in >> di[i][j];
		f = (real*)malloc(n*sizeof(real));
		for (uint i = 0; i < N; ++i)
			in >> f[i];
		x = (real*)malloc(n*sizeof(real));
		for (uint i = 0; i < N; ++i)
			in >> x[i];
		jx = (real*)malloc(n*sizeof(real));
	}
	real norm(real *v, int _n)
	{
		real sum = 0;
		for (uint i = 0; i < _n; ++i) {
			sum += pow(v[i],2);
		}
		return sqrt(sum);
	}
#define Jee(e,y,z) (-(ZRODI+e) + e*y/3 + y + z)
	real residual()
	{
		real *ax = (real*)calloc(n,sizeof(real));
		for (uint i = 0; i < n; ++i)
		for (uint k = 0; k < N; ++k) {
			int j = Jee(m,k,i);
			ax[i] += j >= 0 && j < n ? di[k][i]*x[j] : 0;
		}
		for (uint i = 0; i < n; ++i) {
			ax[i] = f[i] - ax[i];
		}
		real nax = norm(ax,n);
		real nf = norm(f,n);
		real res = nax / nf;
#ifdef DEBUG
		std::cerr << "Residual is " << nax << '/' << nf
			<< '=' << res << std::endl;
#endif
		free(ax);
		return res;
	}
	void solve(int method)
	{
		uint i;
		for (i = 0; i < max; ++i) {
			iterate(method);
			if (residual() < eps) break;
		}
#ifdef DEBUG
		std::cerr << "Total number of iterations: " << i << std::endl;
#endif
	}
	void iterate(int method)
	{
		if (JACOBI == method) memcpy(jx, x, n*sizeof(real));
		for (uint i = 0; i < n; ++i) {
			real thing = JACOBI == method ? Jacobi(i) : Gauss(i);
			x[i] = x[i] + (w/di[ZRODI][i])*thing;
		}
#ifdef DEBUG
		std::cerr << "x: ";
		print_x(std::cerr);
#endif
	}
	real Jacobi(int i)
	{
		real sum = 0;
		for (int k = 0; k < N; ++k) {
			int j = Jee(m,k,i);
			sum += j >= 0  && j < n ? di[k][i]*jx[j] : 0;
		}
		return f[i] - sum;
	}
	real Gauss(int i)
	{
		real sum = 0;
		for (int k = 0; k < N; ++k) {
			int j = Jee(m,k,i);
			sum += j >= 0  && j < n ? di[k][i]*x[j] : 0;
		}
		return f[i] - sum;
	}
#undef Jee
	void print_x(std::ostream& out)
	{
		for (uint i = 0; i < n; ++i)
			out << x[i] << ' ';
		out << std::endl;
	}
private:
	uint n, m, max;
	real w, eps, **di, *f, *x, *jx;
};

int main(int argc, char **argv)
{
	SLE sle;
	sle.init(std::cin);
	if (argc == 2) {
		switch (argv[1][0])
		{
			case 'j': sle.solve(JACOBI); break;
			default: sle.solve(GAUSS); break;
		}
	} else {
		std::cerr << "Usage: " << argv[0] << " method" << std::endl;
		exit(1);
	}
	sle.print_x(std::cout);
	return 0;
}
