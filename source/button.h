#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class button : public sf::RectangleShape
{
public:
	button(float x, float y, float pos_x, float pos_y, std::string text, sf::Color bg_color, sf::Color hbg_color, sf::Color font_color, sf::Color hfont_color);
	~button();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);

	bool onClick(sf::RenderWindow* window);
	bool hover(sf::RenderWindow* window);

	bool getActive();
	void setActive();

	void setStringButton(std::string text);

private:
	sf::Font* font;
	sf::Text* title;

	sf::Color* bg_color;
	sf::Color* font_color;
	sf::Color* hbg_color;
	sf::Color* hfont_color;


	bool hover_w;
	bool active;
};