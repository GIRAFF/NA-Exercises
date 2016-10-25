#include "SLE.h"

SLE::SLE() {}

/*
 * Input format:
 * N
 * ia_1, ia_2, ... ia_N, ia_{N+1}
 * di_1, di_2, ... di_N
 * al_1, al_2, ... al_{ia_{N+1}}
 */

void SLE::load(std::istream& in)
{
	in >> n;
	//allocate vector
	vec = new real[n];
	di = new real[n];
	ia = new int[n+1];
	// read profile
	for(int i = 0; i < n+1; ++i)
		in >> ia[i];
	// allocate some more memory
	al = new real[ia[n]];
	// read all the stuff
	for(int i = 0; i < n; ++i)
		in >> di[i];
	for(int i = 0; i < ia[n]; ++i)
		in >> al[i];
	for(int i = 0; i < n; ++i)
		in >> vec[i];

#ifdef DEBUG
	std::cerr << "A:\n";
	for(int r = 0; r < n; ++r)
	{
		std::cerr << "  ";
		for(int c = 0; c <= r; ++c)
		{
			if(r == c)
			{
				std::cerr << di[r];
				continue;
			}
			int len = ia[r+1] - ia[r];
			std::cerr << (c < r-len ? 0 : al[ia[r]+c-(r-len)]) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';
#endif
}

void SLE::computeL()
{
	int r = 0, c = 0;
	for(int i = 0; i <= ia[n]; ++i)
	{
		// if the row is done
		while(i == ia[r+1])
		{
			// computing di[r]
			real sum = 0;
			for(int k = ia[r]; k < ia[r+1]; ++k) sum += pow(al[k], 2);
			di[r] = sqrt(di[r] - sum);
			// moving r and c
			++r;
			if(r == n) break;
			c = r - ia[r+1] + ia[r]; 
		}
		if(r == n) break;
		real sum = 0;
		// the thing is about zros
		int offr = ia[r+1]-ia[r];
		int offc = ia[c+1]-ia[c];
#define notneg(x) ((x) > 0 ? (x) : 0)
		int sc = ia[c] + notneg(offc-offr+1);
		int sr = ia[r] - notneg(offc-offr+1);
#undef notneg
		for(int k = 0; sr+k < i; ++k)
			sum += al[sr+k] * al[sc+k];
		al[i] = (al[i] - sum)/di[c];
		++c;
	}

#ifdef DEBUG
	std::cerr << "L:\n";
	for(int r = 0; r < n; ++r)
	{
		std::cerr << "  ";
		for(int c = 0; c <= r; ++c)
		{
			if(r == c)
			{
				std::cerr << di[r];
				continue;
			}
			int len = ia[r+1] - ia[r];
			std::cerr << (c < r-len ? 0 : al[ia[r]+c-(r-len)]) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';
#endif
}

void SLE::computeY()
{
	for (int i = 0; i < n; ++i) {
		real sum = 0;
		for (int k = ia[i]; k < ia[i+1]; ++k) {
			sum += al[k] * vec[i-ia[i+1]+k]; 
		}
		vec[i] = (vec[i] - sum)/di[i];
	}
#ifdef DEBUG
	std::cerr << "Y:\n\t";
	for (int i = 0; i < n; ++i) {
		std::cerr << vec[i] << " ";
	}
	std::cerr << "\n\n";
#endif
}

void SLE::computeX()
{
	for (int i = n-1; i >= 0; --i) {
		for (int k = ia[i]; k < ia[i+1]; ++k) {
			// TODO seems, like we need to do kinda reverse of indecies
			vec[i-ia[i+1]+k] -= al[k];
			std::cerr << "Mmm! " << i-ia[i+1]+k << '\n';
		}
		vec[i] /= di[i];
	}
#ifdef DEBUG
	std::cerr << "X:\n\t";
	for (int i = 0; i < n; ++i) {
		std::cerr << vec[i] << " ";
	}
	std::cerr << "\n\n";
#endif
}
