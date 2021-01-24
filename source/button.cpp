#include "button.h"

button::button(float x, float y, float pos_x, float pos_y, std::string text, sf::Color bg_color, sf::Color hbg_color, sf::Color font_color, sf::Color hfont_color) : sf::RectangleShape(sf::Vector2f(x, y))
{
	this->bg_color = new sf::Color(bg_color);
	this->font_color = new sf::Color(font_color);
	this->hbg_color = new sf::Color(hbg_color);
	this->hfont_color = new sf::Color(hfont_color);

	this->setFillColor(bg_color);

	this->setPosition(pos_x, pos_y);
	this->setOutlineThickness(1.f);
	this->setOutlineColor(sf::Color::Black);
	this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);

	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/data-latin.ttf");

	this->title = new sf::Text(text, *this->font, 16U);
	this->title->setFillColor(font_color);
	this->title->setOrigin(this->title->getGlobalBounds().width / 2.f, this->title->getGlobalBounds().height / 2.f);
	this->title->setPosition(this->getPosition().x, this->getPosition().y - 4.f);//to do

	this->hover_w = false;
	this->active = true;

}

void button::Update(sf::RenderWindow * window)
{
	if (this->getActive())
	{
		if (this->hover(window))
		{
			this->setFillColor(*this->hbg_color);
			this->title->setFillColor(*this->hfont_color);
			this->hover_w = true;
		}
		else if (this->hover_w)
		{
			this->setFillColor(*this->bg_color);
			this->title->setFillColor(*this->font_color);
			this->hover_w = false;
		}
	}
}
void button::Render(sf::RenderWindow * window)
{
	window->draw(*this);
	window->draw(*this->title);
}

bool button::onClick(sf::RenderWindow * window)
{
	if (this->hover(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		return true;
	}
	return false;
}

bool button::hover(sf::RenderWindow * window)
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	if (mouse_pos.x <= (int)this->getPosition().x + (int)this->getGlobalBounds().width / 2.f && mouse_pos.x >= (int)this->getPosition().x - (int)this->getGlobalBounds().width / 2.f && mouse_pos.y <= (int)this->getPosition().y + (int)this->getGlobalBounds().height / 2.f && mouse_pos.y >= (int)this->getPosition().y - (int)this->getGlobalBounds().height / 2.f)
	{
		return true;
	}
	return false;
}

bool button::getActive()
{
	return this->active;
}

void button::setActive()
{
	this->active = !this->active;
}

void button::setStringButton(std::string text)
{
	this->title->setString(text);
}

button::~button()
{
	delete this->title;
	delete this->font;
	delete this->bg_color;
	delete this->hbg_color;
	delete this->font_color;
	delete this->hfont_color;
}
