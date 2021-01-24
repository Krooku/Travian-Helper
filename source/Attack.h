#pragma once
#include <string>

class Attack {
private:
	std::string from_player;
	std::string from_village;
	std::string at;
	int time_value;
public:
	Attack(std::string from_player, std::string from_village, std::string at, int time_value);
	std::string getFullString();
	std::string getFrom_player();
	std::string getFrom_village();
	std::string getAt();
	int getTime_value();
};