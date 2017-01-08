#ifndef NOGUI

#include "main.h"
#include <cstdint>

turtle::turtle() {}
turtle::turtle(uint width, uint height)
	: w(width), h(height)
{
	init();
}
void turtle::init()
{
	window.create(sf::VideoMode(w, h), WIN_TITLE, sf::Style::Close);
	view.reset(sf::FloatRect(0,0,DEF_VW,DEF_VH));
	window.setView(view);
}
void turtle::read_funcs(std::string filename)
{
	std::ifstream in(filename, std::ifstream::in);
	std::string f;
	while (!std::getline(in, f).eof()) {
		fs.push_back(read_rpolish(f));
	}
}
void turtle::add_point(real x, real y)
{
	vs.push_back(sf::Vertex(sf::Vector2f(x,y), vs_color));
}
// WARNING: locking function
void turtle::show()
{
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::White);

		// draw functions
		for (func_t func : fs) {
			real p;
			real l = p = view.getViewport().left;
			real r = l + DEF_VW;
			std::vector<sf::Vertex> v;
			for (uint64_t x = 0; p < r; ++x) {
				p = l + x*step; 
				real f = func(&p);
				sf::Vertex vtx(sf::Vector2f(p, f), sf::Color::Red);
				v.push_back(vtx);
			}
			window.draw(&v[0], v.size(), sf::LinesStrip);
		}

		// solution
		window.draw(&vs[0], vs.size(), sf::LinesStrip);

		window.display();
	}
}

#endif
