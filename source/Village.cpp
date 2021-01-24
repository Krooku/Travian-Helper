#include "Village.h"
#include <iostream>
#include <curl/curl.h>
#include "TidyFunctions.h"

#include <sstream>

Village::Village(int village_id, int x, int y, std::string name) {
	this->village_id = village_id;
	this->x = x;
	this->y = y;
	this->name = name;
	this->under_attack = false;
	this->ressources = new int[4];
	this->ressources_full_time = new int[4];
	this->granary = 800;
	this->warehouse = 800;
	this->freeCrop = 0;
	//init ressources 
	for (int i = 0; i < 4; i++) {
		this->ressources[i] = -1;
		this->ressources_full_time[i] = -1;
	}

	this->font = new sf::Font();
	this->font->loadFromFile("Fonts/Lato-Regular.ttf");

	this->text = this->name + "(" + std::to_string(x) + "|" + std::to_string(y) + ") - brak wszystkich informacji!";
	this->villageText = new sf::Text(this->text, *this->font, 22u);
	this->villageText->setFillColor(sf::Color(221u, 221u, 221u));

	this->text_resources = "nie ustawiono";
	this->resourcesText = new sf::Text(this->text, *this->font, 22u);
	this->resourcesText->setFillColor(sf::Color(221u, 221u, 221u));

	this->troopsInVillageString = "nie ustawiono";
	this->troopsInVillageText = new sf::Text(this->troopsInVillageString, *this->font, 22u);
	this->troopsInVillageText->setFillColor(sf::Color(221u, 221u, 221u));

	if (this->refreshRessourcesInfo())
		this->setTextVillage();

	this->buildSlots = 2;
}

void Village::Update() {

}

void Village::Render(sf::RenderWindow* window) {
	window->draw(*this->villageText);
	window->draw(*this->resourcesText);
	window->draw(*this->troopsInVillageText);
}

void Village::setTextVillage() {
	std::cout << this->name << "              (" << this->x << "|" << this->y << "), id=" << this->village_id << std::endl;
	//std::cout << "Drewno: " << this->ressources[0] << ", Glina: " << this->ressources[1] << ", Zelazo: " << this->ressources[2] << ", Zboze: " << this->ressources[3] << std::endl;
	this->text = "(" + std::to_string(this->village_id) + ")(" + std::to_string(this->x) + "|" + std::to_string(this->y) + ") " + this->name;
	if (under_attack)
		this->text += " jest atakowana!";

	this->villageText->setString(this->text);

	this->text_resources = "Magazyn: " + std::to_string(this->warehouse) + ", Drewno: " + std::to_string(this->ressources[0]) + ", Glina: " + std::to_string(this->ressources[1]) + ", Zelazo: " + std::to_string(this->ressources[2]);
	this->text_resources += ", Spichlerz: " + std::to_string(this->granary) + ", Zboze: " + std::to_string(this->ressources[3]);
	this->resourcesText->setString(this->text_resources);
}

void Village::setUnder_attack() {
	this->under_attack = !this->under_attack;
}
bool Village::getUnder_attack() {
	return this->under_attack;
}

std::vector<Attack*> Village::getAttacks() {
	return this->attacks;
}

int Village::getVillage_id() {
	return this->village_id;
}

std::string Village::getVillageName() {
	return this->name;
}

int Village::setActiveVillage() {
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	if (!curl) {
		std::cerr << "Curl init error!" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, ("https://ts1.travian.pl/dorf1.php?newdid=" + std::to_string(this->village_id) + "&").c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");

	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		return -1;
	}

	curl_easy_cleanup(curl);
	return this->village_id;
}

void Village::resAttackTime() {
	std::vector<std::string> times;
	std::vector<std::string> from_villages;
	std::vector<std::string> from_player;

	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };

	if (!curl) {
		std::cerr << "Curl init error!" << std::endl;
		return;
	}

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?gid=16&tt=1&filter=1&subfilters=1");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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
					dumpNodeText(tdoc, tidyGetRoot(tdoc), 0, times, "div", "span", "at"); /* walk the tree */ //pobiera czasy o ktorej dojdzie atak
					dumpNodeText(tdoc, tidyGetRoot(tdoc), 0, from_villages, "td", "a", "role"); // z jakiej osady atak
					dumpNodeText2(tdoc, tidyGetRoot(tdoc), 0, from_player, "td", "a", "11", "href");
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
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

	for (size_t i = 0; i < times.size(); i ++) // access by reference to avoid copying
	{
		times[i] = times[i].erase(0, 7);
		times[i] = times[i].erase(times[i].length() - 2, times[i].length() - 1);
		from_villages[i] = from_villages[i].erase(from_villages[i].length() - 2, from_villages[i].length() - 1);

		from_player[i] = from_player[i].erase(from_player[i].length() - 2, from_player[i].length() - 1);
		std::cout << times[i] << from_villages[i] << " " << from_player[i] << std::endl;
		this->attacks.push_back(new Attack(from_player[i], from_villages[i], times[i], 0));
	}

}

bool Village::refreshRessourcesInfo() {
	//ilosc surków, czas do zapelnienia, pojemnosc
	std::vector <std::string>capacity;
	std::vector <std::string>ressources_quantity;


	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };

	if (!curl) {
		std::cerr << "Curl init error!" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, ("https://ts1.travian.pl/dorf1.php?newdid=" + std::to_string(this->village_id) + "&").c_str()); //url zmienia aktywna wioske
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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
					dumpNodeText(tdoc, tidyGetRoot(tdoc), 0, capacity, "div", "div", "capacity"); /* walk the tree */ //pobiera pojemnosc
					dumpNodeGetText(tdoc, tidyGetRoot(tdoc), 0, ressources_quantity, "a", "div", "stockBarButton", 0); // DOBRE
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}
	std::string capacity1 = capacity[0].erase(0, 3);
	capacity1 = capacity1.erase(capacity[0].length() - 5, capacity[0].length() - 1);
	capacity1 = capacity1.erase(capacity1.length() - 4, 1);
	this->warehouse = std::stoi(capacity1);

	std::string capacity2 = capacity[1].erase(0, 3);
	capacity2 = capacity2.erase(capacity[1].length() - 5, capacity[1].length() - 1);
	capacity2 = capacity2.erase(capacity2.length() - 4, 1);
	this->granary = std::stoi(capacity2);

	for (size_t i = 0; i < ressources_quantity.size(); i++) {
		ressources_quantity[i] = ressources_quantity[i].erase(0, 3);
		ressources_quantity[i] = ressources_quantity[i].erase(ressources_quantity[i].length() - 5, 5);
		size_t found = ressources_quantity[i].find(" ");
		if(found != std::string::npos)
			ressources_quantity[i] = ressources_quantity[i].erase(found, (size_t)1);

		this->ressources[i] = std::stoi(ressources_quantity[i]);
	}
	return true;
}

bool Village::sendRaidList() {
	if (this->setActiveVillage() < 0) {
		std::cerr << "setActiveVillage error" << std::endl;
		return false;
	}
	Sleep(1000);
	std::string value = "init value";
	std::string url = "https://ts1.travian.pl/build.php?gid=16&tt=99";
	std::string userAgent = "libcurl-agent/1.0";
	//std::string attrNames[2] = { "type", "name" };
	//std::string attrValues[2] = { "hidden", "a" };
	std::vector<std::string> attrNames;
	attrNames.push_back("type");
	attrNames.push_back("name");
	std::vector<std::string>attrValues;
	attrValues.push_back("hidden");
	attrValues.push_back("a");

	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str()); // libcurl-agent/1.0
	//curl_easy_setopt(curl, CURLOPT_POST, 1L);

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
					dumpNodeGetAttrValue(tdoc, tidyGetRoot(tdoc), 0, "input", attrNames, attrValues, value);
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}
	std::cout << value << std::endl;

	std::ostringstream oss;

	oss << "action=startRaid&a=";
	char* encoded = curl_easy_escape(curl, value.c_str(), (int)value.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&sort=distance&direction=asc&lid=318&slot%5B28785%5D=on&slot%5B48099%5D=on&slot%5B43821%5D=on&slot%5B48100%5D=on&slot%5B48113%5D=on&slot%5B48140%5D=on&slot%5B48112%5D=on&slot%5B48125%5D=on&slot%5B48117%5D=on&slot%5B48121%5D=on&slot%5B48126%5D=on&slot%5B48119%5D=on";
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	CURLcode res1 = curl_easy_perform(curl);
	if (res1 != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res1) << std::endl;
		return false;
	}

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	return true;
}

bool Village::chanceVillageName(std::string villageName) {
	if (this->setActiveVillage() < 0) {
		std::cerr << "setActiveVillage error" << std::endl;
		return false;
	}
	Sleep(1000);
	
	std::vector<std::string> texts;

	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?gid=16&tt=99");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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
					dumpNodeGetText1(tdoc, tidyGetRoot(tdoc), 0, texts, "script", "type", "text/javascript");
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}
	system("cls");
	//prepare ajaxToken
	std::cout << texts[2].length() << std::endl;
	//std::cout << texts[2].erase(0, 732) << std::endl;
	std::string ajaxToken = texts[2].erase(0, 733);
	ajaxToken = ajaxToken.erase(32, ajaxToken.length());
	std::cout << ajaxToken << std::endl;
	std::cout << ajaxToken.length() << std::endl;

	//prepare request data andsend
	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/ajax.php?cmd=changeVillageName");

	std::ostringstream oss;

	oss << "cmd=changeVillageName&name=";
	char* encoded = curl_easy_escape(curl, villageName.c_str(), (int)villageName.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&did=2477&ajaxToken=";

	encoded = curl_easy_escape(curl, ajaxToken.c_str(), (int)ajaxToken.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	CURLcode res1 = curl_easy_perform(curl);
	if (res1 != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res1) << std::endl;
		return false;
	}

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	return true;
}

bool Village::createRaidList(std::string raidListName) {
	std::vector<std::string> texts;
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	//TidyDoc tdoc;
	//TidyBuffer docbuf = { 0 };
	//TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	//curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?tt=99&id=39");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

	//curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
	//curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");

	int res;
	/*tdoc = tidyCreate();
	//tidyOptSetBool(tdoc, TidyForceOutput, yes); 
	tidyOptSetInt(tdoc, TidyWrapLen, 4096);
	tidySetErrorBuffer(tdoc, &tidy_errbuf);
	tidyBufInit(&docbuf);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);

	int res = curl_easy_perform(curl);
	system("cls");
	if (!res) {
		res = tidyParseBuffer(tdoc, &docbuf);
		if (res >= 0) {
			res = tidyCleanAndRepair(tdoc); 
			if (res >= 0) {
				res = tidyRunDiagnostics(tdoc); 
				if (res >= 0) {
					dumpNodeGetText1(tdoc, tidyGetRoot(tdoc), 0, texts, "script", "type", "text/javascript");
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); 
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}

	system("cls");
	//prepare ajaxToken
	std::cout << texts[2].length() << std::endl;
	//std::cout << texts[2].erase(0, 732) << std::endl;
	std::string ajaxToken = texts[2].erase(0, 727);
	ajaxToken = ajaxToken.erase(32, ajaxToken.length());
	std::cout << ajaxToken << std::endl;
	std::cout << ajaxToken.length() << std::endl;*/

	//prepare request data andsend
	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/api/v1/ajax/raidList");

	std::ostringstream oss;

	oss << "did=";
	char* encoded = curl_easy_escape(curl, std::to_string(this->village_id).c_str(), (int)std::to_string(this->village_id).length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&listName=";
	encoded = curl_easy_escape(curl, raidListName.c_str(), (int)raidListName.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&method=actionAddList&t1=2&t2=0&t3=0&t4=0&t5=0&t6=0&t7=0&t8=0&t9=0&t10=0";

	/*encoded = curl_easy_escape(curl, ajaxToken.c_str(), (int)ajaxToken.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}*/

	std::string postdata1 = oss.str();

	struct curl_slist* headers = NULL;
	curl_slist_append(headers, "authorization: Bearer e171bd4e59d93c58b78a1da6d26248b3");
	curl_slist_append(headers, "accept: application/json, text/javascript, */*; q=0.01");
	curl_slist_append(headers, "content-Type: application/json; charset=UTF-8");
	curl_slist_append(headers, "origin: https://ts1.travian.pl");
	curl_slist_append(headers, "x-requested-with: XMLHttpRequest");
	//curl_slist_append(headers, "charset: utf-8");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	std::string data = "{\"did\" : \"" + std::to_string(this->village_id) + "\", \"listName\" : \"testnowyyyy\", \"method\" : \"actionAddList\", \"t1\" : \"2\", \"t2\" : \"0\", \"t3\" : \"0\", \"t4\" : \"0\", \"t5\" : \"0\", \"t6\" : \"0\", \"t7\" : \"0\", \"t8\" : \"0\", \"t9\" : \"0\", \"t10\" : \"0\"}";
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata1.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postdata1.length());
	// SEND AJAX
	CURLcode res1 = curl_easy_perform(curl);
	if (res1 != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res1) << std::endl;
		return false;
	}


	//GET LISTS ID
	/*curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?tt=99&id=39#");

	std::vector<std::string> attrVals;
	res = curl_easy_perform(curl);
	system("cls");
	if (!res) {
		res = tidyParseBuffer(tdoc, &docbuf);
		if (res >= 0) {
			res = tidyCleanAndRepair(tdoc);
			if (res >= 0) {
				res = tidyRunDiagnostics(tdoc);
				if (res >= 0) {
					GetAttrValue(tdoc, tidyGetRoot(tdoc), 0, "input", "name", "lid", attrVals);
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); 
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}*/

	//std::cout << attrVals.size() << std::endl;
	int newestListId = 0;

	//if (attrVals.size() == (size_t)0) {
	//	std::cerr << "Create raid list error!" << std::endl;
	//	return false;
	//}
	//newestListId = std::stoi(attrVals[0]);

	//for (size_t i = 1; i < attrVals.size(); i++) {
	//	int listId = std::stoi(attrVals[i]);
	//	if (newestListId < listId)
	//		newestListId = listId;
	//}

	std::cout << "Lista grabiezy id " << newestListId << " stworzona!"<< std::endl;

	curl_easy_cleanup(curl);
	//tidyBufFree(&docbuf);
	//tidyBufFree(&tidy_errbuf);
	//tidyRelease(tdoc);

	//this->fillRaidList("-16", "-6", "84", "14", std::to_string(newestListId));
	return true;
}

bool Village::addToRaidList(std::string x, std::string y, std::string raidListId) {
	std::vector<std::string> texts;
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?tt=99&id=39");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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
					dumpNodeGetText1(tdoc, tidyGetRoot(tdoc), 0, texts, "script", "type", "text/javascript");
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}

	system("cls");
	//prepare ajaxToken
	std::cout << texts[2].length() << std::endl;
	//std::cout << texts[2].erase(0, 732) << std::endl;
	std::string ajaxToken = texts[2].erase(0, 727);
	ajaxToken = ajaxToken.erase(32, ajaxToken.length());
	std::cout << ajaxToken << std::endl;
	std::cout << ajaxToken.length() << std::endl;

	//prepare request data andsend
	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/ajax.php?cmd=raidList");

	std::ostringstream oss;

	oss << "cmd=raidList&method=ActionAddSlot&listId=";
	char* encoded = curl_easy_escape(curl, raidListId.c_str(), (int)raidListId.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&slotId=&x=";
	encoded = curl_easy_escape(curl, x.c_str(), (int)x.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&y=";
	encoded = curl_easy_escape(curl, y.c_str(), (int)y.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&t1=2&t2=0&t3=0&t4=0&t5=0&t6=0&t7=0&t8=0&t9=0&t10=0&ajaxToken=";

	encoded = curl_easy_escape(curl, ajaxToken.c_str(), (int)ajaxToken.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	CURLcode res1 = curl_easy_perform(curl);
	if (res1 != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res1) << std::endl;
		return false;
	}

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	return true;
}


// T¥ FUNKCJE TRZEBA WYCIAGNAC Z VILLAGE BO TAK NIE LADNIE -- VILLAGE NIE POTRZEBUJE JEJ CHYBA
bool Village::fillRaidList(std::string startX, std::string startY, std::string range, std::string maxDiffPop, std::string raidListId) {
	std::vector<std::string> texts;
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, "https://www.gettertools.com/ts1.travian.pl.16/42-Szukaj-nieaktywnych");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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

	std::ostringstream oss;

	oss << "action=calcInaktiv&xyX=";
	char* encoded = curl_easy_escape(curl, startX.c_str(), (int)startX.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&xyY=";
	encoded = curl_easy_escape(curl, startY.c_str(), (int)startY.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&range=";
	encoded = curl_easy_escape(curl, range.c_str(), (int)range.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&maxDiff=";
	encoded = curl_easy_escape(curl, maxDiffPop.c_str(), (int)maxDiffPop.length());
	if (encoded)
	{
		oss << encoded;
		curl_free(encoded);
	}

	oss << "&minSpielerCitys=1&maxSpielerCitys=5&minSpielerEW=0&maxSpielerEW=6000&antiNoob=on&speed=7&quick=7&tp=0";

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	std::string postdata = oss.str();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());

	int res = curl_easy_perform(curl);
	system("cls");
	if (!res) {
		res = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
		if (res >= 0) {
			res = tidyCleanAndRepair(tdoc); /* fix any problems */
			if (res >= 0) {
				res = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
				if (res >= 0) {
					getTextsFromallChildsOfChild(tdoc, tidyGetRoot(tdoc), texts, "a", "target", "_blank", false);
					// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}

	std::cout << texts.size() << std::endl;

	std::cout << "Wypelnianie listy grabiezy..." << std::endl;
	for (size_t x = 1; x < texts.size(); x += 5) {
		std::string X = texts[x].erase(texts[x].length() - 2, 2);
		std::string Y = texts[x + 2].erase(texts[x + 2].length() - 2, 2);
		if(X != "1" && Y != "0")
			this->addToRaidList(X, Y, raidListId);
	}

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	return true;
}

void Village::initTexts(float x, float y, int viewType) {
	float margin_left = 20.f;
	float margin_top = 5.f;
	switch (viewType) {
	case 0:
		//--------- VillageView -------------
		//Left bar

		//-------------------------------------------------------------------

		//Village name + village ressources
		this->villageText->setPosition(x + margin_left, y);
		this->resourcesText->setPosition(this->villageText->getPosition().x, y + this->villageText->getGlobalBounds().height + margin_top);
		//-------------------------------------------------------------------

		//Troops in village
		this->troopsInVillageText->setPosition(this->villageText->getPosition().x, this->resourcesText->getPosition().y + this->resourcesText->getGlobalBounds().height + margin_top);
		//-----------------------------------------

		//Village outside builds + Village centrum builds

		//-------------------------------------------------------------------
		break;
	default:
		std::cout << "Nierozpoznany viewType" << std::endl;
	}
}

bool Village::responseTroopsInVillage() {
	if (this->setActiveVillage() < 0) {
		std::cerr << "setActiveVillage error" << std::endl;
		return false;
	}
	//Sleep((DWORD)250 + (DWORD)(rand()%250));

	std::vector<std::string> texts;

	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/build.php?gid=16&tt=1&filter=3");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // libcurl-agent/1.0

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
					getTextsFromallChildsOfChild(tdoc, tidyGetRoot(tdoc), texts, "tbody", "class", "units last", false);
				}
			}
		}
	}
	else {
		std::cerr << curl_errbuf << std::endl;
		return false;
	}

	std::cout << texts.size() << std::endl;
	this->troopsInVillageString = "";
	for (size_t i = 1; i < texts.size(); i++) {
		texts[i] = texts[i].erase(texts[i].length() - 2, 2);
		std::cout << texts[i] << " ";
		this->troopsInVillage.push_back(std::stoi(texts[i]));
		//std::cout << this->troopsInVillage[i] << std::endl;
		this->troopsInVillageString += std::to_string((int)i) + ": " + std::to_string(this->troopsInVillage[i-1]) + "\n";
		if (((int)i + 1) % 12 == 0)
			break;
	}
	this->troopsInVillageText->setString(this->troopsInVillageString);
	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);

	return true;
}
bool Village::responseAllTroops() {
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (!curl)
	{
		std::cerr << "CURL init failed" << std::endl;
		return false;
	}


	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	//tidyRelease(tdoc);

	return true;
}

Village::~Village() {
	delete[] this->ressources;
	delete[] this->ressources_full_time;
	this->attacks.erase(this->attacks.begin(), this->attacks.end());
	delete this->villageText;
	delete this->resourcesText;
	delete this->troopsInVillageText;
	delete this->font;
}