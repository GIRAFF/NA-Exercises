#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <functional>

typedef double real;
typedef std::function<real(real*)> func_t;

func_t read_rpolish(std::string &str);
