#pragma once
#include "NavBarEntity.h"
#include "VillageBar.h"
class RaidListView : public NavBarEntity
{
private:
	VillageBar* villageBar;

public:
	RaidListView(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float margin_top, float x, float y, std::string text, sf::Font* font, unsigned int font_size);
	~RaidListView();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
};

