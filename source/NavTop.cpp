#include "NavTop.h"
#include "VillageView.h"
#include "RaidListView.h"

NavTop::NavTop(sf::RenderWindow* window, VillageManager* villageManager, VillageBar* villageBar, float height) : sf::RectangleShape(sf::Vector2f((float)window->getSize().x, height)) {
	this->villageManager = villageManager;
	
	this->setPosition(0.f, 0.f);
	this->setFillColor(sf::Color(15u, 169u, 135u));


	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/data-latin.ttf");

	this->margin = 20.f;

	this->navBarEntities.push_back(new VillageView(window, villageManager, villageBar, height, this->margin, height / 2.f, "Villages", font, 24u));
	this->navBarEntities.push_back(new NavBarEntity(villageManager, this->navBarEntities[0]->getPosition().x + this->navBarEntities[0]->getGlobalBounds().width + this->margin, height / 2.f, "Building", font, 24u));
	this->navBarEntities.push_back(new RaidListView(window, villageManager, villageBar, height, this->navBarEntities[1]->getPosition().x + this->navBarEntities[1]->getGlobalBounds().width + this->margin, height / 2.f, "Raidlist", font, 24u));
	this->navBarEntities.push_back(new NavBarEntity(villageManager, this->navBarEntities[2]->getPosition().x + this->navBarEntities[2]->getGlobalBounds().width + this->margin, height / 2.f, "Resources", font, 24u));

	this->activeEntity = nullptr;
	//this->activeEntity = this->navBarEntities[4];
	//this->activeEntity->setSelected();

	this->lKey = false;
}

NavTop::~NavTop() {
	delete this->font;
	this->navBarEntities.erase(this->navBarEntities.begin(), this->navBarEntities.end());
}

void NavTop::Update(sf::RenderWindow* window) {
	if(this->activeEntity != nullptr)
		this->activeEntity->Update(window);
	if(!this->lKey)
		for (size_t i = 0; i < this->navBarEntities.size(); i++) {
			if(this->activeEntity != this->navBarEntities[i])
				this->navBarEntities[i]->Update(window);
			if (this->navBarEntities[i]->onClick(window)) {
				if (this->activeEntity == nullptr) {
					this->activeEntity = this->navBarEntities[i];
					this->activeEntity->setSelected();
				}
				else {
					this->activeEntity->setSelected();
					this->activeEntity = this->navBarEntities[i];
					this->activeEntity->setSelected();
				}
				continue;
			}
		}
	this->lKey = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

void NavTop::Render(sf::RenderWindow* window) {
	window->draw(*this);
	for (size_t i = 0; i < this->navBarEntities.size(); i++) {
		window->draw(*this->navBarEntities[i]);
	}
	if(this->activeEntity != nullptr)
		this->activeEntity->Render(window);
}