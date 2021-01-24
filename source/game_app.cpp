#include "game_app.h"
#include <Windows.h>
#include <iostream>

void game_app::Initialize(sf::RenderWindow* window)
{
	this->villageManager = new VillageManager();
	this->villageManager->init();

	//this->loading = new sf::Thread(&VillageManager::init, this->villageManager);
	//TEST

	//this->villageManager->getVillages()[0]->fillRaidList("-16", "-6", "84", "2", "niewazne na ten moment");
	//this->villageManager->getVillages()[0]->createRaidList("TESTNR3");
	//Sleep(1000000);

	//-----

	this->app_window = new sf::RenderWindow(sf::VideoMode(1224u, 576u), "Travian helper", sf::Style::Titlebar | sf::Style::Close);
	this->app_window->clear(bg_color);
	coreState.SetWindow(this->app_window);
	coreState.GetWindow()->setFramerateLimit(60u);

	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/Lato-Regular.ttf");

	this->villageBar = new VillageBar(this->app_window, this->villageManager, this->font, 24u, 80.f, 50.f);
	this->navTop = new NavTop(this->app_window, this->villageManager, this->villageBar, 80.f);
	this->raidClock.restart();
	this->raidTime = this->raidClock.getElapsedTime();

	this->checkAttackClock.restart();
	this->checkAttackTime = this->checkAttackClock.getElapsedTime();

	this->tabKey = true;
	this->enterKey = true;
	this->lclick = true;

	this->checkAttack = true;
	this->sendRaid = true;

	/*if (this->villageManager->getVillage(2477) != nullptr)
		this->villageManager->getVillage(2477)->chanceVillageName("02 Ogrimmar");
	else
		std::cout << "nulptr" << std::endl;*/

	this->load = false;

}
void game_app::Update(sf::RenderWindow * window)
{
	if (!this->load) {
		//this->loading->launch();
		this->load = true;
	}
	this->navTop->Update(window);

	/*if (this->sendRaid) {
		this->villageManager->getVillage(2477)->sendRaidList();
		this->raidClock.restart();
		this->sendRaid = false;
	}*/
	if (this->checkAttack) {
		this->villageManager->checkAttacks();
		this->checkAttackClock.restart();
		this->checkAttack = false;
	}

	//Times
	if (this->raidTime.asSeconds() >= 3600 + rand() % 1000 + 1) {
		this->sendRaid = true;
	}
	if (this->checkAttackTime.asSeconds() >= 60 + rand() % 120 + 1) {
		this->checkAttack = true;

	}

	this->raidTime = this->raidClock.getElapsedTime();
	this->checkAttackTime = this->checkAttackClock.getElapsedTime();
}
void game_app::Render(sf::RenderWindow * window)
{
	//this->villageManager->Render(window);
	this->navTop->Render(window);
}
void game_app::Destroy(sf::RenderWindow * window)
{
	delete this->villageManager;

	delete this->navTop;
	delete this->villageBar;
	coreState.SetWindow(NULL);
	delete this->app_window;
	delete this->font;

	delete this->loading;
}