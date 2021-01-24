#pragma once
#include <SFML/Graphics.hpp>
#include "VillageManager.h"

class NavBarEntity : public sf::Text
{
private:
	sf::Color activeColor;
	sf::Color normalColor;
	sf::Color hoverColor;
	sf::Color noActiveColor;

	bool active;

	bool hover;
	bool Hover(sf::RenderWindow* window);
protected:
	bool selected;
	VillageManager* villageManager;
public:
	NavBarEntity(VillageManager* villageManager, float x, float y, std::string text, sf::Font* font, unsigned int font_size);
	~NavBarEntity();

	virtual void Update(sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
	void setActive();
	void setSelected();

	bool onClick(sf::RenderWindow* window);
};

