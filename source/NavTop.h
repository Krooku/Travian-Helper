#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "NavBarEntity.h"
#include "VillageManager.h"
#include <vector>
#include "VillageBar.h"
class NavTop : public sf::RectangleShape
{
private:
	VillageManager* villageManager;
	std::vector<NavBarEntity*> navBarEntities;
	NavBarEntity* activeEntity;

	sf::Font* font;
	float margin;
	bool lKey;
public:
	NavTop(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float height);
	~NavTop();
	
	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);

private:

};