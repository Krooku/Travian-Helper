#pragma once
#include <SFML/Graphics.hpp>

#include <utility> //std::pair

#include "VillageManager.h"
#include "Village.h"
#include "NavBarEntity.h"

class VillageBar : public sf::RectangleShape
{
private:
	VillageManager* villageManager;
	sf::Text* loadingText;
	unsigned int loadedVillages;
	float marginLeft;

	std::vector<std::pair<NavBarEntity*, Village*>> villagesNames;
	std::pair<NavBarEntity*, Village*> activeVillage;
	float villageBarSize;
	bool hover;

	sf::Vector2i mouse_pos;
	sf::Vector2i prev_mouse_pos;

	bool lKey;
	bool moveBar;

	bool responseTroops;
public:
	VillageBar(sf::RenderWindow* window, VillageManager* villageManager, sf::Font* font, unsigned int font_size, float margin_top, float height);
	~VillageBar();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);

	bool Hover(sf::RenderWindow* window);

	Village* getActiveVillage();
};

