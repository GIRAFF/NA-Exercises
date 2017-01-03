#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

typedef double real;

class SLE
{
	public:
		SLE() {}
		void init(std::istream& in)
		{
			in >> n >> max_iter >> eps;
			ig = new uint[n+1];
			for (uint i = 0; i <= n; ++i)
				in >> ig[i];
			jg = new uint[ig[n]];
			for (uint i = 0; i < ig[n]; ++i)
				in >> jg[i];
			di = new real[n];
			for (uint i = 0; i < n; ++i)
				in >> di[i];
			ggl = new real[ig[n]];
			for (uint i = 0; i < ig[n]; ++i)
				in >> ggl[i];
			f = new real[n];
			for (uint i = 0; i < n; ++i)
				in >> f[i];
			//x = new real[n];
			x = (real*)calloc(n,sizeof(real));
			//for (uint i = 0; i < n; ++i)
				//in >> x[i];
			z = new real[n];
			r = new real[n];
			az = new real[n];
		}
		real norm(real *v)
		{
			real sum = 0;
			for (uint i = 0; i < n; ++i) {
				sum += pow(v[i],2);
			}
			return sqrt(sum);
		}
		real residual()
		{
			return norm(r) / nf;
		}
		// ATTENTION! This function stores result to az!
		// Have fun.
		void multA(real *v)
		{
			for (uint i = 0; i < n; ++i) {
				az[i] = v[i]*di[i];			
			}
			for (uint i = 0; i < n; ++i)
				for (uint j = ig[i]; j < ig[i+1]; ++j) {
					az[i] += v[jg[j]] * ggl[j];
					az[jg[j]] += v[i] * ggl[j];
				}
		}
		real dot_prod(real *v, real *_v)
		{
			real res = 0;
			for (uint i = 0; i < n; ++i)
				res += v[i] * _v[i];
			return res;
		}
		void load()
		{
			// makin' r_0
			multA(x);
			for (uint i = 0; i < n; ++i) {
				r[i] = f[i] - az[i];
			}
			// makin' z_0
			memcpy(z, r, n*sizeof(real));
			// makin' (r_0, r_0)
			rr = dot_prod(r, r);
			// makin' Az_0
			multA(z);
			// makin' ||f||
			nf = norm(f);
		}
		void iterate()
		{
			real alpha = rr / dot_prod(az, z);
			for (uint i = 0; i < n; ++i) {
				x[i] += alpha * z[i];
				r[i] -= alpha * az[i];
			}
			// TODO don't like it
			real beta = 1 / rr;
			rr = dot_prod(r, r);
			beta *= rr;
			for (uint i = 0; i < n; ++i)
				z[i] = r[i] + beta * z[i];
			multA(z);
		}
		void solve()
		{
			load();
			for (iter = 0; iter < max_iter; ++iter) {
				iterate();
				if (residual() < eps) break;
			}
#ifdef DEBUG
			std::cout << "X:\n";
			for (uint i = 0; i < n; ++i)
				printf("%.20e\n", x[i]);
			std::cout << '\n' << iter << '\n';
#endif
		}
	private:
		uint n, max_iter;
		real eps;
		uint *ig, *jg;
		real *di, *ggl, *f, *x;

		uint iter = 0;
		real rr, nf;
		real *z, *r, *az;
};

int main(int argc, char** argv)
{
	SLE sle;
	//std::cout << std::scientific;
	sle.init(std::cin);
	sle.solve();
	std::cout << sle.residual() << '\n';
	return 0;
}
