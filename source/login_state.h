#pragma once
#include "game_state.h"
#include "text_field.h"
#include "button.h"

class login_state : public tiny_state
{
public:
	void Initialize(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Destroy(sf::RenderWindow* window);
	bool Login(std::string url, std::string userAgent);
private:
	sf::RenderWindow* login_window;

	Text_field* nick_name;
	Text_field* password;

	sf::Font* font;
	sf::Text* title;
	sf::Text* error_login;

	button* login_button;

	bool error_hide;

	bool tabKey;
	bool enterKey;
	bool lclick;
};