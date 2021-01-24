#include "login_state.h"
#include <Windows.h>
#include <iostream>
#include <curl/curl.h>
#include "TidyFunctions.h"
#include <sstream>
#include "game_app.h"

void login_state::Initialize(sf::RenderWindow* window)
{
	//this->login_window = new sf::RenderWindow(sf::VideoMode(500u, 350u), "Login");
	this->login_window = new sf::RenderWindow(sf::VideoMode(500u, 350u), "Login", sf::Style::Titlebar | sf::Style::Close);

	coreState.SetWindow(this->login_window);

	this->nick_name = new Text_field(200.f, 50.f, (float)this->login_window->getSize().x / 2.f, 140.f, "login", false, 20, 2);
	this->password = new Text_field(200.f, 50.f, (float)this->login_window->getSize().x / 2.f, 200.f, "password", true, 20, 2);

	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/Lato-Regular.ttf");

	this->title = new sf::Text("Login to Travian", *this->font, 64u);
	this->title->setOrigin(this->title->getGlobalBounds().width / 2.f, this->title->getGlobalBounds().height / 2.f);
	this->title->setPosition((float)this->login_window->getSize().x / 2.f, 40.f);
	this->title->setFillColor(sf::Color(221u, 221u, 221u));
	this->title->setOutlineColor(sf::Color::Black);
	this->title->setOutlineThickness(4.f);

	this->error_login = new sf::Text("Zly login lub haslo", *this->font, 14u);
	this->error_login->setOrigin(this->error_login->getGlobalBounds().width / 2.f, this->error_login->getGlobalBounds().height / 2.f);
	this->error_login->setPosition((float)this->login_window->getSize().x / 2.f, 240.f);
	this->error_login->setFillColor(sf::Color::Red);

	this->login_button = new button(200.f, 50.f, (float)this->login_window->getSize().x / 2.f, 290.f, "Login", sf::Color(0u,255u,0u), sf::Color(255u, 245u, 5u), sf::Color::Black, sf::Color::White);
	
	this->error_hide = true;

	this->tabKey = true;
	this->enterKey = true;
	this->lclick = true;

}
void login_state::Update(sf::RenderWindow* window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab) && !this->tabKey)
	{
		if (this->nick_name->getActive())
		{
			this->nick_name->setActive();
			this->password->setActive();
		}
		else if (this->password->getActive())
		{
			this->password->setActive();
			this->nick_name->setActive();
		}
		else
		{
			this->nick_name->setActive();
		}
	}

	if (coreState.GetEvent() != NULL && coreState.GetEvent()->type == sf::Event::TextEntered)
	{
		this->nick_name->Write(*coreState.GetEvent());
		this->password->Write(*coreState.GetEvent());
		coreState.SetEvent(NULL);
	}

	this->nick_name->Update(this->login_window);
	this->password->Update(this->login_window);
	this->login_button->Update(this->login_window);

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey) || (this->login_button->onClick(this->login_window) && !this->lclick) && this->nick_name->getString().size() > 2 && this->password->getString().size() > 2)
	{
		if (this->Login("https://ts1.travian.pl/login.php", "libcurl-agent/1.0")) {
			std::cout << "zalogowano" << std::endl;
			coreState.SetState(new game_app());
		}
		else {
			std::cout << "nie zalogowano" << std::endl;
		}
	}
	this->enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
	this->tabKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab);
	this->lclick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
void login_state::Render(sf::RenderWindow* window)
{
	this->nick_name->Render(this->login_window);
	this->password->Render(this->login_window);
	this->login_button->Render(this->login_window);
	this->login_window->draw(*this->title);
	if (!this->error_hide)
		this->login_window->draw(*this->error_login);
}

bool login_state::Login(std::string url, std::string userAgent) {
	std::vector<std::string> attrNames;
	attrNames.push_back("type");
	attrNames.push_back("name");
	std::vector<std::string> attrValues;
	attrValues.push_back("text");
	attrValues.push_back("name");
	std::string found = "not";


	CURL* curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}

	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str()); // libcurl-agent/1.0
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	std::ostringstream oss;

	oss << "name=";
	char* encoded = curl_easy_escape(curl, this->nick_name->getString().c_str(), (int)this->nick_name->getString().length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&password=";
	encoded = curl_easy_escape(curl, this->password->getString().c_str(), (int)this->password->getString().length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");

	tdoc = tidyCreate();
	tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */
	tidyOptSetInt(tdoc, TidyWrapLen, 4096);
	tidySetErrorBuffer(tdoc, &tidy_errbuf);
	tidyBufInit(&docbuf);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);

	int res = curl_easy_perform(curl);
	system("cls");
	if (!res) {
		res = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
		if (res >= 0) {
			res = tidyCleanAndRepair(tdoc); /* fix any problems */
			if (res >= 0) {
				res = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
				if (res >= 0) {
					dumpNodeGetAttrValue(tdoc, tidyGetRoot(tdoc), 0, "input", attrNames, attrValues, found); /* walk the tree */ //pobiera czasy o ktorej dojdzie atak
				}
			}
		}
	}
	else
		std::cerr << curl_errbuf << std::endl;

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	if (found == this->nick_name->getString()) {
		this->error_hide = false;
		return false;
	}

	return true;
}

void login_state::Destroy(sf::RenderWindow* window)
{
	coreState.SetWindow(NULL);
	delete this->login_window;
	delete this->nick_name;
	delete this->password;
	delete this->login_button;
	delete this->font;
	delete this->title;
	delete this->error_login;
}