#pragma once
#include <vector>

#include "Village.h"

class VillageManager {
private:
	std::vector<Village*> villages;
	bool inited;

//na momemnt
public:
	bool getInited();

	bool initVillages();
	int activeVillageid;

	void init();

public:
	VillageManager();
	~VillageManager();

	void Render(sf::RenderWindow* window);

	std::vector<Village*> getVillages();
	Village* getVillage(int villageId);
	void setTextVillages();
	bool checkAttacks();

	int getActiveVillageid();
};