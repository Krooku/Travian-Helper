#pragma once

#include <string>
#include "Attack.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Village {
private:
	int* ressources;
	int* ressources_full_time;
	int granary;
	int warehouse;
	int freeCrop;

	int village_id;
	int x, y;
	std::string name;
	bool under_attack;

	int buildSlots;

	std::vector<int> troopsInVillage;

	std::vector<Attack*> attacks;

	sf::Font* font;
	std::string text;
	sf::Text* villageText;
	std::string text_resources;
	sf::Text* resourcesText;
	std::string troopsInVillageString;
	sf::Text* troopsInVillageText;


public:
	Village(int villave_id, int x, int y, std::string name);
	~Village();

	void Update();
	void Render(sf::RenderWindow* window);
	void setTextVillage();
	void setUnder_attack();
	bool getUnder_attack();
	std::vector<Attack*> getAttacks();

	int getVillage_id();
	std::string getVillageName();

	int setActiveVillage();
	void resAttackTime();

	bool refreshRessourcesInfo();
	bool sendRaidList();

	bool chanceVillageName(std::string villageName);
	bool createRaidList(std::string raidListName);
	bool addToRaidList(std::string x, std::string y, std::string raidListId);
	bool fillRaidList(std::string startX, std::string startY, std::string range, std::string maxDiffPop, std::string raidListId);

	void initTexts(float x, float y, int viewType);

	bool responseTroopsInVillage();
	bool responseAllTroops();

};