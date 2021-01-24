#include "text_field.h"

Text_field::Text_field(float x, float y, float pos_x, float pos_y, std::string text, bool hide, int max_size, int type) : sf::RectangleShape(sf::Vector2f(x, y))
{
	this->text = text;
	this->current_text = "";

	this->setFillColor(sf::Color(102u, 51u, 51u));
	this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);
	this->setPosition(pos_x, pos_y);
	this->setOutlineThickness(2.f);
	this->setOutlineColor(sf::Color::Black);

	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/data-latin.ttf");

	this->title = new sf::Text(this->text, *this->font, 16U);
	this->title->setFillColor(sf::Color::Black);

	this->title->setPosition(this->getPosition().x - this->getGlobalBounds().width / 2.f + 10.f, this->getPosition().y - this->title->getGlobalBounds().height / 2.f - 4.f);//to do
	this->time = this->clock.getElapsedTime();

	this->max_size = max_size;
	this->type = type;

	this->active = false;
	this->flag = true;
	this->hide = hide;

}

void Text_field::Update(sf::RenderWindow * window)
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	if (mouse_pos.x <= (int)this->getPosition().x + (int)(this->getGlobalBounds().width / 2.f) && mouse_pos.x >= (int)this->getPosition().x - (int)(this->getGlobalBounds().width / 2.f) && mouse_pos.y <= (int)this->getPosition().y + (int)(this->getGlobalBounds().height / 2.f) && mouse_pos.y >= (int)this->getPosition().y - (int)(this->getGlobalBounds().height / 2.f) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->active = true;
	}
	else if ((mouse_pos.x > (int)this->getPosition().x + (int)(this->getGlobalBounds().width / 2.f) || mouse_pos.x < (int)this->getPosition().x - (int)(this->getGlobalBounds().width / 2.f) || mouse_pos.y >(int)this->getPosition().y + (int)(this->getGlobalBounds().height / 2.f) || mouse_pos.y < (int)this->getPosition().y - (int)(this->getGlobalBounds().height / 2.f)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->active = false;
	}

	std::string hided = this->current_text;
	if (this->hide)
	{
		for (size_t i = 0; i < this->current_text.length(); i++)
		{
			hided[i] = '*';
		}
	}

	if (this->active)
	{
		this->setFillColor(sf::Color(57u, 165u, 241u));
		if (this->flag)
		{
			if (!this->hide)
				this->title->setString(this->current_text + "|");
			else
				this->title->setString(hided + "|");
		}
		else
		{
			if (!this->hide)
				this->title->setString(this->current_text);
			else
				this->title->setString(hided);
		}

	}
	else
	{
		this->setFillColor(sf::Color(102u, 51u, 51u));
		if (this->current_text == "")
			this->title->setString(this->text);
		else if (this->hide)
		{
			this->title->setString(hided);
		}
		else
			this->title->setString(this->current_text);
	}

	if (this->time.asSeconds() > 0.5f)
	{
		this->flag = !this->flag;
		this->clock.restart();
	}

	this->time = clock.getElapsedTime();
}
void Text_field::Render(sf::RenderWindow * window)
{
	window->draw(*this);
	window->draw(*this->title);
}

void Text_field::Write(sf::Event event) //0 only letters, 1 only numbers, 2 numbers and letters, 3 emails, 4 others
{
	if (this->getActive())
	{
		if (this->current_text.length() < (size_t)this->max_size)
		{
			if (this->type == 0 || this->type == 2 || this->type == 3)
			{
				if ((event.text.unicode < 123 && event.text.unicode > 96) || (event.text.unicode < 91 && event.text.unicode > 64))
				{
					//std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
					this->current_text += static_cast<char>(event.text.unicode);
				}
			}
			if (this->type == 1 || this->type == 2 || this->type == 3)
			{
				if (event.text.unicode < 58 && event.text.unicode > 47)
				{
					//std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
					this->current_text += static_cast<char>(event.text.unicode);
				}
			}
			else if (this->type == 3)
			{
				if (event.text.unicode == 96 || event.text.unicode == 95)
				{
					//std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
					this->current_text += static_cast<char>(event.text.unicode);
				}

			}
		}
		if (this->current_text.length() > 0 && event.text.unicode == 8)
		{
			this->current_text.pop_back();
		}
	}
}

bool Text_field::getActive()
{
	return this->active;
}

void Text_field::setActive()
{
	this->active = !this->active;
}

std::string Text_field::getString()
{
	return this->current_text;
}

Text_field::~Text_field()
{
	delete this->title;
	delete this->font;
}
