#include "Attack.h"

Attack::Attack(std::string from_player, std::string from_village, std::string at, int time_value) {
	this->from_player = from_player;
	this->from_village = from_village;
	this->at = at;
	this->time_value = time_value;
}
std::string Attack::getFullString() {
	return this->from_player + " z osady " + this->from_village + ". Atak dojdzie o " + this->at;
}
std::string Attack::getFrom_player() {
	return this->from_player;
}
std::string Attack::getFrom_village() {
	return this->from_village;
}
std::string Attack::getAt() {
	return this->at;
}
int Attack::getTime_value() {
	return this->time_value;
}