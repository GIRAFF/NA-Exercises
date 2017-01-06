#define _USE_MATH_DEFINES

#include <cstring>
#include <cmath>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <functional>

using namespace std;

typedef double real;
typedef function<real(real*)> func_t;

vector<string> cpp_strtok(string &str)
{
	vector<string> res;
	// TODO dirty hack
	char *c_str = const_cast<char*>(str.c_str());
	char *p = strtok(c_str, " \n\t");
	while (p != NULL) {
		res.push_back(string(p));
		p = strtok(NULL, " \n\t");
	}
	return res;
}

// TODO check input (non-valid characters, incorrect indices etc)
func_t read_rpolish(string &str)
{
	assert(str.compare(""));
	vector<string> tokens = cpp_strtok(str);
	stack<func_t> st;
	for (auto it = tokens.begin(); it != tokens.end(); ++it) {
		func_t val;
		// here we have an argument reference
		// ('_3' means the third element of vector)
		if ('_' == (*it)[0]) {
			uint index = atoi(it->substr(1).c_str());
			val = [index](auto vec) { return vec[index]; };
			st.push(val);
		}
// this can be tricky and buggy, but I hate repeating the code and I dunno
// how to do this thing right
#define fst val(vec)
#define snd _val(vec)
#define binary(a,b) \
		else if (!it->compare(a)) { \
			func_t _val = st.top(); \
			st.pop(); \
			val = st.top(); \
			st.top() = [val, _val](auto vec) \
				{ return b; }; \
		}
#define unary(a,b) \
		else if (!it->compare(a)) { \
			val = st.top();\
			st.top() = [val](auto vec)\
				{ return b; };\
		}
#define nullary(a,b) \
		else if (!it->compare(a)) { \
			val = [](auto vec) { return b; }; \
			st.push(val); \
		}

		binary ("+", fst + snd)
		binary ("-", fst - snd)
		binary ("*", fst * snd)
		binary ("/", fst / snd)
		binary ("^", pow(fst, snd))
		unary ("abs", fabs(fst))
		unary ("sin", sin(fst))
		unary ("cos", cos(fst))
		unary ("tg", tan(fst))
		unary ("arcsin", asin(fst))
		unary ("arccos", acos(fst))
		unary ("arctg", atan(fst))
		unary ("exp", exp(fst))
		unary ("ln", log(fst))
		unary ("lg", log10(fst))
		// log_{snd} fst
		binary ("log", log(fst)/log(snd))
		nullary ("pi", M_PI)
		nullary ("e", M_E)

		// here we have just a numeric constant
		else {
			real data = atof(it->c_str());
			val = [data](auto vec) { return data; };
			st.push(val);
		}

#undef fst
#undef snd
#undef binary
#undef unary
#undef nullary
	}
	func_t res = st.top();
	return res;
}

/*int main(int argc, char **argv)
{
	string str;
	getline(cin, str);
	func_t f = read_rpolish(str);
	// just to test
	real *v = new real[1];
	v[0] = 5;
	cout << f(v) << endl;
	return 0;
}*/
