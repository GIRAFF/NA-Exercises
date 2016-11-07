#ifndef _LAB1_MAIN
#define _LAB1_MAIN

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <iomanip>

// use this to dump matrices and vectors to std::err
//#define DEBUG

// use this to use double as real 
//#define USE_DOUBLE

// use this to do a barrel roll
//#define DOUBLE_SCAL

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

#endif //_LAB1_MAIN
