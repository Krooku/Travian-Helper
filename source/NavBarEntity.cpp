#include "NavBarEntity.h"
#include <iostream>
#include <Windows.h>

NavBarEntity::NavBarEntity(VillageManager* villageManager, float x, float y, std::string text, sf::Font* font, unsigned int font_size) : sf::Text(text, *font, font_size)
{
	this->activeColor = sf::Color(174u, 180u, 7u);
	this->normalColor = sf::Color(231u, 222u, 220u);
	this->hoverColor = sf::Color::White;
	this->noActiveColor = sf::Color::Red;// sf::Color(211u, 211u, 211u);

	this->active = false;
	this->selected = false;
	
	this->hover = false;

	this->villageManager = villageManager;

	this->setFillColor(this->noActiveColor);
	this->setPosition(x, y - (float)font_size / 2.f - 3.f);
}


NavBarEntity::~NavBarEntity()
{
}

void NavBarEntity::Update(sf::RenderWindow* window) {
	if (!this->selected) {
		if (this->active) {
			if (!this->hover && this->Hover(window)) {
				this->setFillColor(this->hoverColor);
				this->hover = true;
			}
			else if (this->hover && !this->Hover(window)) {
				this->setFillColor(this->normalColor);
				this->hover = false;
			}
		}
		else if (!this->active && this->villageManager->getInited()) {
			this->setFillColor(this->normalColor);
			this->active = true;
		}
	}
}

void NavBarEntity::Render(sf::RenderWindow* window) {
	//tutaj renderowanie wszystkiego poza tym obiektem
}

bool NavBarEntity::Hover(sf::RenderWindow* window) {
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	if (mouse_pos.x <= (int)(this->getPosition().x + this->getGlobalBounds().width) && mouse_pos.x >= (int)(this->getPosition().x) && mouse_pos.y >= (int)(this->getPosition().y) + this->getCharacterSize() / 2 - 6 && mouse_pos.y <= (int)(this->getPosition().y + this->getGlobalBounds().height) + this->getCharacterSize() / 2 - 2)
		return true;
	return false;
}

bool NavBarEntity::onClick(sf::RenderWindow* window)
{
	if (this->active && this->hover && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return true;
	return false;
}

void NavBarEntity::setActive() {
	this->active = !this->active;
}

void NavBarEntity::setSelected() {
	this->selected = !this->selected;
	if (this->selected) {
		this->setFillColor(this->activeColor);
	}
	else {
		this->setFillColor(this->normalColor);
	}
	//this->setActive();
}