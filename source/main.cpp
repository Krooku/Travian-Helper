/*#include <SFML/Graphics.hpp>
#include <iostream>
#include "app.h"
#include <Windows.h>


#include <curl/curl.h>
#include <sstream>
#include "TidyFunctions.h"

void changeVname(const std::string& url, const std::string& userAgent) { //https://ts4.travian.pl/ajax.php?cmd=changeVillageName

	CURL* curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");
	headers = curl_slist_append(headers, "cookie: __gads=ID=983edbb6c36694ed:T=1576621189:S=ALNI_MY0xvqNLr5J-G3O-m9cvMMRWYDdCQ; _ga=GA1.2.953378611.1576527444; active_rallypoint_sub_filters_2=4%2C5; travian-dismissCookieNotice=1; active_rallypoint_sub_filters_1=1; travian-language=pl-PL; mapId1=%7B%22grid%22%3Atrue%7D; JWT=eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJzYWx0IjoiZmI5NWUyNDI5YTdiYTQzNjM2YmVlYmI5NmM5N2U1ZTIiLCJwcm9wZXJ0aWVzIjp7ImxvZ2luSWQiOjIzOTI0MiwibG93UmVzIjpmYWxzZSwibW9iaWxlT3B0aW1pemF0aW9ucyI6ZmFsc2UsInZpbGxhZ2VQZXJzcGVjdGl2ZSI6InBlcnNwZWN0aXZlUmVzb3VyY2VzIn0sInRpbWVzdGFtcCI6MTU4NTc1NTY4MywiaGFzaCI6IjBlMDA2NzEwNjcxMDY3MTAxYjM2ZWNlZTBkNmVmZTM3IiwidXVpZCI6IjlhZmQyODAwLTIwYWItMTFlYS0xOTA0LTAxMDAwMDAwMDE3NiIsInB3IjoiNmM2MDA4MzlhNDRkMmIzN2M1MDY3YjE5OGMzMjAyMzVjMTY0ZWQzNCJ9.BLV5O8srCUMunJ6BzctNBvzhQshp_jumSSeiXyuaX0Gl2yPM_eDsgmMHvfxwdkEaBYZU5cygkkuPXnnAnlj9-gGwTXX7O7S5p20trwbrtuhisa6eMXwF0r5aOUiGRARXws70n-AawHZtaHQ0pD4eJ8Spmz4e8DZ2ViDBNzWCfbaaLgIMlc1aiptBzR_q0Psc6gtWMMpBLFJr05bRCcZBk2_-66L7pItBHhJgr1kPwrJ_F3a3TDZ593c5QfM57zExm7RFEF-Pnix6PxrYcvTKN6NCpZDW4YIs5mmUurSAz-RIzpSWjiUebhtP6IoEj9t8smqZGWA0cks4XGZ0POdLnw");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str()); // libcurl-agent/1.0
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	std::ostringstream oss;

	oss << "{\"cmd\":\"changeVillageName\", \"name\":\"hmm\", \"did\":\"2447\", \"ajaxToken\":\"7c9ae9672dc7bb54830eb88ea9eed8ee\"}";

	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");

	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << std::endl;


	curl_easy_cleanup(curl);
}

int main() {
	curl_global_init(CURL_GLOBAL_ALL);

	// create the window
	sf::RenderWindow window(sf::VideoMode(800u, 600u), "Travian Client");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Green);

		// draw everything here...
		// window.draw(...);

		// end the current frame
		window.display();
	}

	//App app("https://ts4.travian.pl/login.php", "libcurl-agent/1.0", "kroku", "qwe100");

	//app.getVillageManager().getVillage(2477)->sendRaidList();

	//changeVname("https://ts4.travian.pl/ajax.php?cmd=changeVillageName", "libcurl-agent/1.0");
	//sendRaidList("https://ts4.travian.pl/build.php?gid=16&tt=99", "libcurl-agent/1.0");

	//app.getVillageManager().getVillage(8725)->setActiveVillage();
	//app.getVillageManager().checkAttacks();
	//app.getVillageManager().getVillage(8725)->resAttackTime();
	//app.getVillageManager().getVillage(8725)->refreshRessourcesInfo();

	//app.getVillageManager().displayVillages();

	curl_global_cleanup();
	return 0;
}*/

// SpacePlane.cpp : Defines the entry point for the console application.
//

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "login_state.h"

#include "SFML/System.hpp"

game_state coreState;
std::string login;
sf::Color bg_color;
bool loading;

int main()
{
	srand(time(NULL));
	coreState.SetState(new login_state);

	login = "";
	bg_color = sf::Color(15u, 51u, 65u);
	loading = false;

	coreState.GetWindow()->setFramerateLimit(60u);

	sf::Font font;
	font.loadFromFile("Fonts/Crimson-Roman.ttf");

	sf::Text loadingText;
	loadingText.setString("Loading, please wait...");
	loadingText.setFillColor(sf::Color::Magenta);

	while (coreState.GetWindow()->isOpen())
	{
		sf::Event event;
		//coreState.SetEvent(&event);
		while (coreState.GetWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::TextEntered)
			{
				coreState.SetEvent(&event);
				//nick_name.Write(event);
				//password.Write(event);
			}
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				coreState.SetEvent(&event);
			}
			else if (event.type == sf::Event::Closed)
			{
				coreState.GetWindow()->close();
				return 0;
			}
		}

		coreState.GetWindow()->clear(bg_color);
		if (loading) {
			loadingText.setPosition((float)coreState.GetWindow()->getSize().x / 2.f, (float)coreState.GetWindow()->getSize().y / 2.f);
			coreState.GetWindow()->draw(loadingText);
			coreState.GetWindow()->clear(bg_color);
			loading = false;
		}
		coreState.Update();
		coreState.Render();

		coreState.GetWindow()->display();
	}
	return 0;
}