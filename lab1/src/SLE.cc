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

/*
 * TODO BUG in SLE::computeL()
 * Last two elements break...
 */
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
		// TODO What da hell is going on????
		int sc = ia[r+1]-ia[r] > ia[c+1]-ia[c] ?
			ia[c] : ia[c+1]-1+ia[r]-ia[r+1];
		int sr = ia[r+1]-ia[r] > ia[c+1]-ia[c] ?
			ia[r+1]-1+ia[c]-ia[c+1] : ia[r];
		for(int k = 0; k < ia[r+1]-ia[r] > ia[c+1]-ia[c] ?
				ia[c+1]-ia[c] : ia[r+1] - ia[r];
				++k)
			sum += al[sr+k] * al[sc+k];
		al[i] = (al[i] - sum)/di[c];
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
