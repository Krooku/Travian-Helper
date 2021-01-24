#include "VillageManager.h"

#include <curl/curl.h>
#include "TidyFunctions.h"
#include <Windows.h>
#include "email.h"

VillageManager::VillageManager() {
	this->activeVillageid = -1; //not set
	this->inited = false;
	/*if (this->initVillages()) {
		std::cout << "inited villages OK" << std::endl;
	}
	else {
		std::cout << "inited villages FAILED" << std::endl;
		return;
	}*/
}

bool VillageManager::getInited()
{
	return this->inited;
}

void VillageManager::Render(sf::RenderWindow* window) {
	for (size_t i = 0; i < this->villages.size(); i++) {
		this->villages[i]->Render(window);
	}
}

bool VillageManager::initVillages() {
	std::vector<std::vector<std::string>> spans;
	std::vector<std::vector<std::string>> a;
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

		curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/dorf1.php");
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
						dumpNode(tdoc, tidyGetRoot(tdoc), 0, spans, "span"); /* walk the tree */
						dumpNode(tdoc, tidyGetRoot(tdoc), 0, a, "a");
						// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
					}
				}
			}
		}
		else
			std::cerr << curl_errbuf << std::endl;

		system("cls");
		int k = 0;
		for (size_t i = 0; i < spans.size(); i++) {
			for (size_t j = 0; j < spans[i].size(); j++) {
				if (spans[i][j] == "data-x" && j + 7 < spans[i].size()) {
					std::cout << std::stoi(spans[i][j + 5]) << std::endl;
					this->villages.push_back(new Village(std::stoi(spans[i][j + 5]), std::stoi(spans[i][j + 1]), std::stoi(spans[i][j + 3]), spans[i][j + 7]));
					k++;
				}
			}
		}
		for (size_t i = 0; i < a.size(); i++) {
			for (size_t j = 0; j < a[i].size(); j++) {
				if (a[i][j] == "active") {
					//this->activeVillageid = a[i][1];
					std::string str = a[i][1].erase(0, 8);
					this->activeVillageid = std::stoi(str.erase(str.length() - 1, str.length() - 1));
					std::cout << this->activeVillageid << std::endl;
					break;
				}
			}
		}
		std::cout << "villages: " << this->villages.size() << std::endl;
		curl_easy_cleanup(curl);
		tidyBufFree(&docbuf);
		tidyBufFree(&tidy_errbuf);
		tidyRelease(tdoc);
		return true;
	}
	return false;
}


//TEST PONIZEJ

void VillageManager::init() {
	std::vector<std::vector<std::string>> spans;
	std::vector<std::vector<std::string>> a;
	CURL* curl = curl_easy_init();
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = { 0 };
	TidyBuffer tidy_errbuf = { 0 };
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

		curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/dorf1.php");
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
						dumpNode(tdoc, tidyGetRoot(tdoc), 0, spans, "span"); /* walk the tree */
						dumpNode(tdoc, tidyGetRoot(tdoc), 0, a, "a");
						// fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
					}
				}
			}
		}
		else
			std::cerr << curl_errbuf << std::endl;

		system("cls");
		int k = 0;
		for (size_t i = 0; i < spans.size(); i++) {
			for (size_t j = 0; j < spans[i].size(); j++) {
				if (spans[i][j] == "data-x" && j + 7 < spans[i].size()) {
					std::cout << std::stoi(spans[i][j + 5]) << std::endl;
					this->villages.push_back(new Village(std::stoi(spans[i][j + 5]), std::stoi(spans[i][j + 1]), std::stoi(spans[i][j + 3]), spans[i][j + 7]));
					k++;
				}
			}
		}
		for (size_t i = 0; i < a.size(); i++) {
			for (size_t j = 0; j < a[i].size(); j++) {
				if (a[i][j] == "active") {
					//this->activeVillageid = a[i][1];
					std::string str = a[i][1].erase(0, 8);
					this->activeVillageid = std::stoi(str.erase(str.length() - 1, str.length() - 1));
					std::cout << this->activeVillageid << std::endl;
					break;
				}
			}
		}
		std::cout << "villages: " << this->villages.size() << std::endl;
		curl_easy_cleanup(curl);
		tidyBufFree(&docbuf);
		tidyBufFree(&tidy_errbuf);
		tidyRelease(tdoc);

		this->inited = true;
	}
}

std::vector<Village*> VillageManager::getVillages() {
	return this->villages;
}

Village* VillageManager::getVillage(int villageId) {
	for (auto& village : this->villages) // access by reference to avoid copying
	{
		std::cout << village->getVillage_id() << std::endl;
		if (village->getVillage_id() == villageId)
		{
			return village;
		}
	}
	return nullptr;
}

void VillageManager::setTextVillages() {
	for (size_t i = 0; i < this->villages.size(); i++) {
		this->villages[i]->setTextVillage();
	}
}

bool VillageManager::checkAttacks() {
	int quantityAtts = 0;
	int numberVillagesAtts = 0;

	std::vector<std::string> villagesIdUnderAttack;
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

	curl_easy_setopt(curl, CURLOPT_URL, "https://ts1.travian.pl/dorf3.php");
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
					dumpNodeAttacks(tdoc, tidyGetRoot(tdoc), 0, villagesIdUnderAttack, "a", "img", "att1"); /* walk the tree */
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

	if (villagesIdUnderAttack.size() == 0)
		return false;

	//std::cout << villagesIdUnderAttack[0] << std::endl;

	std::string email_body_part2 = "";
	int numberOfAttacks = 0;

	//set underattack
	for (auto& village : villagesIdUnderAttack) // access by reference to avoid copying
	{
		std::string part = village.erase(0, 18);
		std::cout << village << std::endl;
		std::cout << "TUTAJ: " << part.erase(5, part.length() - 5) << std::endl;
		//std::cin >> part;
		//int villageId = std::stoi(part.erase(part.length() - 14, part.length() - 1));
		int villageId = std::stoi(part.erase(5, part.length() - 5));
		if (this->getVillage(villageId) == nullptr) {
			std::cerr << "getVillage nullptr!" << std::endl;
			return false;
		}

		if(!this->getVillage(villageId)->getUnder_attack())
			this->getVillage(villageId)->setUnder_attack();
		this->getVillage(villageId)->setActiveVillage();
		this->getVillage(villageId)->resAttackTime();
		numberOfAttacks += (int)this->getVillage(villageId)->getAttacks().size();
		for (auto& attack : this->getVillage(villageId)->getAttacks()) // access by reference to avoid copying
		{
			email_body_part2 += attack->getFullString() + ".\n";
		}
	}
	std::string email_body_part1 = "Liczba nadchodz¹cych ataków: " + std::to_string(numberOfAttacks) + "\n";
	sendEmail("krokumeister@gmail.com",
		"krokumeister584@gmail.com",
		"krokumeister@gmail.com",
		"Travian helper",
		"Travian atak",
		(email_body_part1 + email_body_part2).c_str(),
		"smtp://smtp.gmail.com:587",
		"KrOkU1@3$");

	return true;
}

int VillageManager::getActiveVillageid() {
	return this->activeVillageid;
}

VillageManager::~VillageManager() {
	this->villages.erase(this->villages.begin(), this->villages.end());
	//for (size_t i = 0; i < this->villages.size(); i++) {
	//	delete this->villages[i];
	//}
}

