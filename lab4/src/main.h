#ifndef _LAB4_MAIN_H
#define _LAB4_MAIN_H

#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <functional>
#include <utility>
#include <fstream>

#ifdef DEBUG
#define LOG_PATH "log.log"
#endif

typedef double real;
typedef std::function<real(real*)> func_t;

func_t read_rpolish(std::string &str);

#ifndef NOGUI

#include <SFML/Graphics.hpp>

#define WIN_TITLE "Functions"
#define DEF_VW    16
#define DEF_VH    12

class turtle
{
	public:
		turtle();
		turtle(uint width, uint height);
		void init();
		void read_funcs(std::string filename);
		void add_point(real x, real y);
		void show();
	private:
		uint w = 800, h = 600;
		uint vtx_limit = 10000;
		real step = 1e-2;
		std::vector<func_t> fs;
		sf::Color vs_color = sf::Color::Black;

		sf::RenderWindow window;
		sf::View view;
		std::vector<sf::Vertex> vs;
};

#endif //NOGUI

#endif //_LAB4_MAIN_H
