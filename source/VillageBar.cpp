#include "VillageBar.h"
#include <iostream>
#include <Windows.h>
#include "game_state.h"

VillageBar::VillageBar(sf::RenderWindow* window, VillageManager* villageManager, sf::Font* font, unsigned int font_size, float margin_top, float height) : sf::RectangleShape(sf::Vector2f((float)window->getSize().x, height))
{
	this->marginLeft = 30.f;
	this->villageBarSize = 0.f;

	this->hover = false;
	this->lKey = false;
	this->moveBar = false;
	this->responseTroops = false;

	this->villageManager = villageManager;
	this->loadingText = new sf::Text("Loading...", *font, font_size);
	this->loadingText->setPosition(10.f, margin_top + height / 2.f);
	this->loadedVillages = 0u;

	this->activeVillage = std::make_pair(nullptr, nullptr);

	this->setPosition(sf::Vector2f(0.f, margin_top));

	if (villageManager->getVillages().size() > (size_t)0) {
		this->villagesNames.push_back(std::make_pair(new NavBarEntity(villageManager, this->getPosition().x + this->marginLeft, margin_top + height / 2.f, villageManager->getVillages()[0]->getVillageName(), font, font_size), villageManager->getVillages()[0]));
		this->villageBarSize += this->marginLeft + this->villagesNames[0].first->getGlobalBounds().width;

		if (villageManager->getActiveVillageid() == villageManager->getVillages()[0]->getVillage_id() || villageManager->getActiveVillageid() == -1) {
			this->activeVillage = this->villagesNames[0];
		}
	}
	else {
		std::cerr << "Nie pobrano wiosek!" << std::endl;
		return;
	}

	for (size_t i = 1; i < villageManager->getVillages().size(); i++) {
		this->villagesNames.push_back(std::make_pair(new NavBarEntity(villageManager, this->villagesNames[i - 1].first->getPosition().x + this->villagesNames[i - 1].first->getGlobalBounds().width + this->marginLeft, margin_top + height / 2.f, villageManager->getVillages()[i]->getVillageName(), font, font_size), villageManager->getVillages()[i]));
		this->villageBarSize += this->marginLeft + this->villagesNames[i].first->getGlobalBounds().width;
		if (villageManager->getActiveVillageid() == villageManager->getVillages()[i]->getVillage_id()) {
			this->activeVillage = this->villagesNames[i];
		}
	}

	if (villageBarSize < (float)window->getSize().x) {
		this->setSize(sf::Vector2f((float)window->getSize().x + this->marginLeft,height));
	}
	else {
		this->setSize(sf::Vector2f(villageBarSize + this->marginLeft, height));
	}
	this->setFillColor(sf::Color(15u, 169u, 135u));
	this->setOutlineColor(sf::Color::Black);
	this->setOutlineThickness(2.f);

	if (this->activeVillage.first == nullptr) {
		this->activeVillage = this->villagesNames[0];
	}
	this->activeVillage.first->setSelected();
	this->activeVillage.second->responseTroopsInVillage();

	this->mouse_pos = sf::Mouse::getPosition(*window);
	this->prev_mouse_pos = mouse_pos;
}


VillageBar::~VillageBar()
{
	delete this->loadingText;
	this->villagesNames.erase(this->villagesNames.begin(), this->villagesNames.end());
}

void VillageBar::Update(sf::RenderWindow* window) {
	if(this->villageManager->getInited())
	{
		this->prev_mouse_pos = mouse_pos;
		this->mouse_pos = sf::Mouse::getPosition(*window);
		if (!this->hover && this->Hover(window)) {
			this->hover = true;
		}
		else if (this->hover && !this->Hover(window)) {

			this->hover = false;
		}

		if (this->responseTroops) {
			this->activeVillage.second->responseTroopsInVillage();
			this->responseTroops = false;
		}

		if (!this->lKey)
			for (size_t i = 0; i < this->villagesNames.size(); i++) {
				this->villagesNames[i].first->Update(window);
				if (this->villagesNames[i].first->onClick(window)) {
					this->activeVillage.first->setSelected();
					this->activeVillage = this->villagesNames[i];
					this->activeVillage.first->setSelected();
					//this->activeVillage.second->responseTroopsInVillage();
					this->responseTroops = true;
					loading = true;
					continue;
				}
			}

		if (this->hover) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				this->moveBar = true;
			}
		}
		if (moveBar) {
			bool allowMove = false;
			float speed = 1.f;
			float velocity = (float)(this->mouse_pos.x - this->prev_mouse_pos.x);
			if (velocity < 0.f && this->getPosition().x >(float)window->getSize().x - this->getGlobalBounds().width) {
				if (velocity * speed + this->getPosition().x < (float)window->getSize().x - this->getGlobalBounds().width + 5.f)
					velocity = ((float)window->getSize().x - this->getGlobalBounds().width - this->getPosition().x) / speed + 5.f;
				allowMove = true;
			}
			else if (velocity > 0.f && this->getPosition().x < 0.f) {
				if (velocity * speed + this->getPosition().x > 0.f)
					velocity = (-this->getPosition().x) / speed;
				allowMove = true;
			}
			if (allowMove) {
				for (size_t i = 0; i < this->villagesNames.size(); i++) {
					this->villagesNames[i].first->move(velocity * speed, 0);
				}
				this->move(velocity * speed, 0);
			}
		}

		this->lKey = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		if (!this->lKey)
			this->moveBar = false;
	}
	else {
		if (this->villageManager->getVillages().size() > this->loadedVillages) {
			this->loadedVillages++;
			this->loadingText->setString("Loaded " + std::to_string(this->loadedVillages) + " villages");
		}
	}
}

void VillageBar::Render(sf::RenderWindow* window) {
	window->draw(*this);
	if (this->villageManager->getInited()) {
		for (size_t i = 0; i < this->villagesNames.size(); i++) {
			window->draw(*this->villagesNames[i].first);
		}
		this->activeVillage.first->Render(window);
	}
	else {
		window->draw(*this->loadingText);
	}
}

bool VillageBar::Hover(sf::RenderWindow* window) {
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	if (mouse_pos.y >= (int)this->getPosition().y && mouse_pos.y <= (int)this->getPosition().y + (int)this->getGlobalBounds().height) {
		return true;
	}
	return false;
}

Village* VillageBar::getActiveVillage() {
	return this->activeVillage.second;
}