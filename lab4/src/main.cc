#include "main.h"

class SNLE
{
	public:
		SNLE()
		{
#ifdef DEBUG
			// unbuffer log stream
			log.rdbuf()->pubsetbuf(0, 0);
			log.open(LOG_PATH, std::ofstream::out);
#endif
		}
		~SNLE()
		{
#ifdef DEBUG
			log.close();
#endif
		}
		void init(std::istream& in)
		{
			in >> n >> max_iter >> min_beta >> min_residual;
#ifndef NOGUI
			if (2 == n) {
				can_draw = true;
				gui.init();
			} else {
				can_draw = false;
#ifdef DEBUG
				log << "Drawing disabled when n != 2.\n";
#endif
			}
#endif
			x = new real[n];
			for (uint i = 0; i < n; ++i)
				in >> x[i];

			a = new real*[n];
			for (uint i = 0; i < n; ++i)
				a[i] = new real[n];
			dx = new real[n];
			f = new real[n];
			xv = new real[n];
			fv = new real[n];
			load_functions(in);
		}
		void load_functions(std::istream& in)
		{
			jacobian = new func_t*[n];
			for (uint i = 0; i < n; ++i)
				jacobian[i] = new func_t[n];
			func = new func_t[n];

			std::string str;
			// read the rest of the prev line
			std::getline(in, str);
			for (uint i = 0; i < n; ++i)
				for (uint j = 0; j < n; ++j) {
					std::getline(in, str);
					jacobian[i][j] = read_rpolish(str);
				}
			for (uint i = 0; i < n; ++i) {
				std::getline(in, str);
				func[i] = read_rpolish(str);
			}
#ifndef NOGUI
			gui.read_funcs("turtle");
#endif
		}
		real norm(real *v)
		{
			real sum = 0;
			for (uint i = 0; i < n; ++i) {
				sum += pow(v[i],2);
			}
			return sqrt(sum);
		}
		// Gauss
		void sle(real *res)
		{
			for (uint i = 0; i < n; ++i)
				f[i] = -f[i];
			// reduction
			for (uint k = 0; k < n; ++k) {
				uint i_max = k;
				for (uint i = k + 1; i < n; ++i) {
					if (a[i][k] > a[i_max][k])
						i_max = i;
				}
				assert(a[i_max][k] != 0);
				std::swap(a[k], a[i_max]);
				std::swap(f[k], f[i_max]);
				for (uint i = k+1; i < n; ++i) {
					real fr = a[i][k] / a[k][k];
					for (uint j = k+1; j < n; ++j)
						a[i][j] -= a[k][j]*fr;
					f[i] -= f[k]*fr;
					a[i][k] = 0;
				}
			}
			for (int k = n-1; k >= 0; --k) {
				res[k] = f[k];
				for (uint i = k+1; i < k; ++i)
					res[k] -= a[k][i]*res[i];
				res[k] /= a[k][k];
			}
		}
		void iterate()
		{
			// || F^{k-1} ||
			real nfk = norm(f);
			// F^k and A^k
			for (uint i = 0; i < n; ++i) {
				f[i] = func[i](x);
				for (uint j = 0; j < n; ++j)
					a[i][j] = jacobian[i][j](x);
			}
#ifdef DEBUG
			uint debug_min = 0;
			log << "F: ";
			for (uint i = 0; i < n; ++i) {
				log << f[i] << ' ';
				if (fabs(f[i]) < fabs(f[debug_min]))
					debug_min = i;
			}
			log << "\nMinimal is " << debug_min << '\n';
#endif
			// dx
			sle(dx);
			// beta
			beta = 1;
			while (true) {
				for (uint i = 0; i < n; ++i) {
					xv[i] = x[i] + beta*dx[i];
					fv[i] = func[i](xv);
				}
				if (norm(fv) <= nfk) break;
				beta /= 2;
				if (beta < min_beta) {
					log << "Falling with beta < "
						<< min_beta << std::endl;
					break;
				}
			}
			// x
			memcpy(x, xv, n*sizeof(real));
		}
		void solve()
		{
			// start values
			beta = 1;
			for (uint i = 0; i < n; ++i)
				f[i] = func[i](x);
			nf = norm(f);
			iter = 0;

			real nfk;
			while ((nfk = norm(f))/nf > min_residual
					&& iter++ < max_iter) {
				iterate();
#ifndef NOGUI
				if(can_draw) gui.add_point(x[0], x[1]);
#endif
				for (uint i = 0; i < n; ++i)
					printf("%.20e ", x[i]);
				printf("\n|F| = %.20e beta = %.20e\n", nfk, beta);
			}
			std::cout << std::endl << iter << std::endl;
#ifndef NOGUI
			gui.show();
#endif
		}
	private:
		uint n, max_iter, iter;
		real min_beta, min_residual;
		real beta, nf;
		real *x, *dx, *f, **a;
		real *xv, *fv;

		func_t *func, **jacobian;
#ifndef NOGUI
		turtle gui;
		bool can_draw = false;
#endif
#ifdef DEBUG
		std::ofstream log;
#endif
};

int main(int argc, char **argv)
{
	SNLE snle;
	snle.init(std::cin);
	snle.solve();
	return 0;
}
