#pragma once

#include "game_state.h"
#include "VillageManager.h"
#include "NavTop.h"
#include "VillageBar.h"

#include <SFML/System.hpp>

class game_app : public tiny_state
{
public:
	void Initialize(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Destroy(sf::RenderWindow* window);

	sf::Thread* loading;
	bool load;



private:
	sf::RenderWindow* app_window;
	NavTop* navTop;
	VillageBar* villageBar;
	VillageManager* villageManager;

	sf::Font* font;

	bool tabKey;
	bool enterKey;
	bool lclick;

	bool checkAttack;
	bool sendRaid;

	sf::Time raidTime;
	sf::Clock raidClock;

	sf::Time checkAttackTime;
	sf::Clock checkAttackClock;

};