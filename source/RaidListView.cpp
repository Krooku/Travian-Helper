#include "RaidListView.h"
#include <iostream>

RaidListView::RaidListView(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float margin_top, float x, float y, std::string text, sf::Font* font, unsigned int font_size) : NavBarEntity(villageManager, x, y, text, font, font_size)
{
	this->villageBar = villageBar;

	for (size_t i = 0; i < villageManager->getVillages().size(); i++) {
		villageManager->getVillages()[i]->initTexts(30.f, margin_top + 55.f, 0);
	}
}


RaidListView::~RaidListView()
{
}

void RaidListView::Update(sf::RenderWindow* window) {
	if (this->selected) {
		this->villageBar->Update(window);
		std::string name;
		std::cout << "Podaj nazwe listy: ";
		std::cin >> name;
		this->villageBar->getActiveVillage()->createRaidList(name);
	}
	NavBarEntity::Update(window);
}

void RaidListView::Render(sf::RenderWindow* window) {
	this->villageBar->Render(window);
}