#include "main.h"
#define DEBUG

typedef float real;
typedef unsigned int uint;

// TODO decompose this hell
// TODO when findin' vetors there's no need to use elem()
//      (see a notebook for notes)
// TODO don't use size_t

class SLE
{
public:
	SLE();
	bool readProfile(std::istream& in);
	bool computeL();
	bool computeY();
	bool computeX();
private:
	bool loaded = false;
	uint n, ln, *ia = nullptr;
	real *di = nullptr, *al = nullptr, *au = nullptr;
	// ONE VECTOR TO RULE THEM ALL!!1!11!
	real *vec = nullptr;
	real *L = nullptr;
};

SLE::SLE() {}

/*
 * Input format:
 * N
 * ia_1, ia_2, ... ia_N, ia_{N+1}
 * di_1, di_2, ... di_N
 * al_1, al_2, ... al_{ia_{N+1}}
 * au_1, au_2, ... au_{ia_{N+1}}
 */

bool SLE::readProfile(std::istream& in)
{
	in >> n;
	// allocate vector
	vec = new real[n];
	di = new real[n];
	ia = new uint[n];
	// allocate L
	ln = 0;
	for(uint i = 1; i < n; ++i) ln += i;
	L = new real[ln];
	// read profile
	for(uint i = 0; i < n+1; ++i)
		in >> ia[i];
	// allocate some more memory
	al = new real[ia[n]],
	au = new real[ia[n]];
	// read all the stuff
	for(uint i = 0; i < n; ++i)
		in >> di[i];
	for(uint i = 0; i < ia[n]; ++i)
		in >> al[i];
	for(uint i = 0; i < ia[n]; ++i)
		in >> au[i];
	for(uint i = 0; i < n; ++i)
		in >> vec[i];

#ifdef DEBUG
	std::cerr << "Here comes A:\n";
	for(uint r = 0; r < n; ++r)
	{
		for(uint c = 0; c < n; ++c)
		{
			if(r == c)
			{
				std::cerr << di[r] << ' ';
				continue;
			}
			if(r > c)
			{
				uint len = ia[r+1] - ia[r];
				std::cerr << (c < r-len ?
					0 : al[ia[r]+c-(r-len)]) << ' ';
				continue;
			}
			uint len = ia[c+1] - ia[c];
			std::cerr << (r < c-len ?
				0 : au[ia[c]+r-(c-len)]) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';
#endif

	loaded = true;
	return loaded;
}

// hmm... It looks like difference in au brings no changes to L.
// TODO need debug
bool SLE::computeL()
{
	if(!loaded) return false;
	uint idx  = 0,    //current index of L
		 il_i = 0;    //start of the current row
	for(uint i = 0; i < n; il_i += i, ++i)
	{
		for(uint j = 0; j <= i; ++j)
		{
			real sum = 0;
			if(i == j)
			{
				for(uint k = il_i; k < il_i + i; ++k) sum += pow(L[k],2);
				di[i] = sqrt(di[i] - sum);
				//row_s += i;
				continue;
			}
			for(uint k = 0; k < j; ++k) sum += L[il_i+k] * L[ia[j]+k];
			L[idx++] = (al[ia[i] + j] - sum)/di[j];
		}
		ia[i] = il_i;
	}
	ia[n+1] = il_i;
#ifdef DEBUG
	std::cerr << "Here comes L:\n";
	for(uint row = 0; row < n; ++row)
	{
		for(uint col = 0; col < n; ++col)
		{
			if(col == row)
			{
				std::cerr << di[row] << ' ';
				continue;
			}
			if(col > row)
			{
				std::cerr << "0 ";
				continue;
			}
			std::cerr << L[ia[row] + col] << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';
#endif
}

bool SLE::computeY()
{
	if(!loaded) return false;
	for(uint i = 0; i < n; ++i)
	{
		real sum = 0;
		for(uint k = 0; k < i; ++k) sum += L[ia[i]+k] * vec[k];
		vec[i] = (vec[i] - sum)/di[i];
	}
#ifdef DEBUG
	std::cerr << "Wanna see Y? Here it is:\n";
	for(uint i = 0; i < n; ++i) std::cerr << vec[i] << ' ';
	std::cerr << "\n\n";
#endif
}

bool SLE::computeX()
{
	if(!loaded) return false;
	for(int i = n - 1; i >= 0; --i)
	{
		real sum = 0;
		for(uint k = i + 1; k < n; ++k) sum += L[ia[k]+i]*vec[k];
		vec[i] = (vec[i] - sum)/di[i,i];
	}
#ifdef DEBUG
	std::cerr << "X:\n";
	for(uint i = 0; i < n; ++i) std::cerr << vec[i] << ' ';
	std::cerr << '\n';
#endif
}

int main(int argc, char **argv)
{
	SLE sle;
	sle.readProfile(std::cin);
	sle.computeL();
	sle.computeY();
	sle.computeX();
	return 0;
}
