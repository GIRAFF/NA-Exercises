#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>

#define N      9
#define ZRODI  4
#define JACOBI 0
#define GAUSS  1

typedef double real;

class SLE
{
public:
	SLE() {}
	~SLE()
	{
		for (uint i = 0; i < N; ++i) free(di[i]);
		free(di);
		free(x);
		//free(jx);
		free(f);
	}
	/* Input file:
	 *
	 * n m weight epsilon max_iter
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
		for (uint i = 0; i < n; ++i)
			in >> f[i];
		x = (real*)malloc(n*sizeof(real));
		for (uint i = 0; i < n; ++i)
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
#define Jee(e,y,z) (-(ZRODI+e) + e*(y/3) + y + z)
	real residual()
	{
		real *ax = (real*)calloc(n,sizeof(real));
		for (uint i = 0; i < n; ++i)
		for (uint k = 0; k < N; ++k) {
			int j = Jee(m,k,i);
//#ifdef DEBUG
		//std::cerr << "j = " << j << std::endl;
//#endif
			ax[i] += j >= 0 && j < n ? di[k][i]*x[j] : 0;
		}
		for (uint i = 0; i < n; ++i) {
			ax[i] = f[i] - ax[i];
		}
		real nax = norm(ax,n);
		real nf = norm(f,n);
		real res = nax / nf;
//#ifdef DEBUG
		//std::cerr << "Residual is " << nax << '/' << nf
			//<< '=' << res << std::endl;
//#endif
		free(ax);
		return res;
	}
	void solve(int method)
	{
		for (iter = 0; iter < max; ++iter) {
			iterate(method);
			if (residual() < eps) break;
		}
//#ifdef DEBUG
		//std::cerr << "Total number of iterations: " << iter << std::endl;
//#endif
	}
	void iterate(int method)
	{
		if (JACOBI == method) memcpy(jx, x, n*sizeof(real));
		else jx = x;
		for (uint i = 0; i < n; ++i) {
			real sum = 0;
			for (int k = 0; k < N; ++k) {
				int j = Jee(m,k,i);
				sum += j >= 0  && j < n ? di[k][i]*jx[j] : 0;
			}
			x[i] = jx[i] + (w/di[ZRODI][i])*(f[i] - sum);
		}
//#ifdef DEBUG
		//std::cerr << "x: ";
		//print_x(std::cerr);
//#endif
	}
#undef Jee
	void print_x(std::ostream& out)
	{
		for (uint i = 0; i < n; ++i)
			out << x[i] << ' ';
		out << std::endl;
	}
	void find_w(int method, std::ostream& out)
	{
		real v[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		real pit = 200001;
		real step;
		out << "\\begin{longtable}{|c|c|c|c|} \\hline\n";
		out << "\\(\\omega\\)&\\(x\\)&\\(x^*-x\\)&Итераций\\\\ \\hline\n";
		if (JACOBI == method) w = step = .5;
		else w = step = 1;
		while (step > .01) {
			iter = 0;
			memset(x, 0, n*sizeof(real));
			solve(method);
			out << "\\(" << w << "\\) &\n\\(\\begin{aligned} ";
			for (int i = 0; i < n; ++i) {
				out << "& " << x[i] << (i == n-1 ? " " : " \\\\ ");
			}
			out << "\\end{aligned}\\) &\n\\(\\begin{aligned} ";
			for (int i = 0; i < n; ++i) {
				out << "& " << v[i] - x[i] << (i == n-1 ? " " : " \\\\ ");
			}
			out << "\\end{aligned}\\) &\n\\("
				<< iter << "\\) \\\\ \\hline\n";
			step = step/2;
			if (iter < pit) w += step;
			else w -= step;
			pit = iter;
		}
		out << "\\end{longtable}\n";
	}
	uint total_iter()
	{
		return iter;
	}
	real *x;
private:
	uint n, m, max, iter;
	real w, eps, **di, *f, *jx;
};

int main(int argc, char **argv)
{
	std::cout << std::setprecision(20);
	SLE sle;
	sle.init(std::cin);
	//sle.solve(JACOBI);
	//sle.print_x(std::cout);
	sle.find_w(JACOBI, std::cout);
	return 0;
}
