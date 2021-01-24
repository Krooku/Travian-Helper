#pragma once
#include "NavBarEntity.h"
#include "VillageManager.h"
#include "VillageBar.h"
class VillageView : public NavBarEntity
{
private:
	sf::Text* text;
	VillageBar* villageBar;
public:
	VillageView(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float margin_top, float x, float y, std::string text, sf::Font* font, unsigned int font_size);
	~VillageView();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
};

