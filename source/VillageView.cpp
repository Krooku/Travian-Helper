#include "VillageView.h"
#include <iostream>


VillageView::VillageView(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float margin_top, float x, float y, std::string text, sf::Font* font, unsigned int font_size) : NavBarEntity(villageManager, x, y, text, font, font_size)
{
	this->text = new sf::Text("Village view", *font, font_size);
	this->text->setPosition(30.f, margin_top + 50.f);

	this->villageBar = villageBar;

	for (size_t i = 0; i < villageManager->getVillages().size(); i++) {
		villageManager->getVillages()[i]->initTexts(30.f, margin_top + 55.f, 0);
	}

}


VillageView::~VillageView()
{
	delete this->text;
}

void VillageView::Update(sf::RenderWindow* window) {
	if (this->selected) {
		this->villageBar->Update(window);
		/*if (this->villageBar->getPosition().x < 0.f) {
			this->villageBar->move(1.5f, 0.f);
			for (size_t i = 0; i < this->villagesNames.size(); i++) {
				this->villagesNames[i]->move(1.5f, 0.f);
			}
		}*/
	}
	NavBarEntity::Update(window);
}

void VillageView::Render(sf::RenderWindow* window) {
	//window->draw(*this->text);
	this->villageBar->Render(window);
	this->villageBar->getActiveVillage()->Render(window);
}
