#pragma once
#include <SFML/Graphics.hpp>
#include "VillageManager.h"

class App {
private:
	VillageManager* villageManager;
public:
	App(const std::string& url, const std::string& userAgent, const std::string& username, const std::string& password);
	~App();

	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);

	VillageManager getVillageManager();

	bool loginToTravian(const std::string& url, const std::string& userAgent, const std::string& username, const std::string& password);
};