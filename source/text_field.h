#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Text_field : public sf::RectangleShape
{
public:
	Text_field(float x, float y, float pos_x, float pos_y, std::string text, bool hide, int max_size, int type);
	~Text_field();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Write(sf::Event event); //0 only letters, 1 only numbers, 2 numbers and letters, 4 others
	bool getActive();
	void setActive();

	std::string getString();

private:
	sf::Font* font;
	sf::Text* title;

	std::string text;
	std::string current_text;

	sf::Clock clock;
	sf::Time time;

	int max_size;
	int type;

	bool active;
	bool flag;
	bool hide;
};