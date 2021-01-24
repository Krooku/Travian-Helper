#include "app.h"
#include "curl/curl.h"
#include <iostream>
#include <sstream>

App::App(const std::string& url, const std::string& userAgent, const std::string& username, const std::string& password) {
	//if (!this->loginToTravian(url, userAgent, username, password))
	//	std::cout << "Sprobuj ponownie" << std::endl;
	//this->villageManager = new VillageManager();
}

void App::Update(sf::RenderWindow* window) {

}
void App::Render(sf::RenderWindow* window) {

}

VillageManager App::getVillageManager() {
	return *this->villageManager;
}

bool App::loginToTravian(const std::string& url, const std::string& userAgent, const std::string& username, const std::string& password) {
	CURL* curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str()); // libcurl-agent/1.0
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	std::ostringstream oss;

	oss << "name=";
	char* encoded = curl_easy_escape(curl, username.c_str(), (int)username.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&password=";
	encoded = curl_easy_escape(curl, password.c_str(), (int)password.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");

	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << std::endl;
	else
		// use res as needed...
		std::cout << "res" << std::endl;

	curl_easy_cleanup(curl);
	return true;
}

App::~App() {
	delete this->villageManager;
}