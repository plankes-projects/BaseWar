/*
 * GameStat.cpp
 *
 *  Created on: 20.07.2013
 *      Author: Planke
 */

#include "GameStat.h"
#include <stdlib.h>     /* atoi */
#include "../Tools/Tools.h"
#include <sstream>

GameStat::GameStat(std::string serverGameStatString) {
	init();
	fromString(serverGameStatString);
}

GameStat::GameStat() {
	init();
}

void GameStat::init() {
	_delemiter = '#';
	_gameStatsDelemiter = '+';

	_randomRaceString = "";
}

GameStat::~GameStat() {
	// TODO Auto-generated destructor stub
}

char GameStat::getGameStatDelemiter() {
	return _gameStatsDelemiter;
}

std::string GameStat::toString() {
	std::string clientGamestateString = "";
	std::string connect = "";

	for (std::map<std::string, std::string>::iterator i = _map.begin(); i != _map.end(); i++) {
		clientGamestateString += connect + i->first;
		connect = _delemiter;
		clientGamestateString += connect + i->second;
	}

	return clientGamestateString;
}

void GameStat::fromString(std::string serverGameStatString) {

	//first char is the player id
	char playerId = serverGameStatString[0];
	serverGameStatString.erase(0, 1);

	std::istringstream ss(serverGameStatString);
	std::string key;
	std::string value;

	while (std::getline(ss, key, _delemiter)) {
		std::getline(ss, value, _delemiter);
		_map[key] = value;
	}

	setPlayerId(playerId - '0');
}

void GameStat::setValue(std::string key, int value) {
	_map[key] = Tools::toString(value);
}

void GameStat::setValue(std::string key, std::string value) {
	//std::replace(value.begin(), value.end(), _delemiter, '_');
	_map[key] = value;
}

bool GameStat::hasKey(std::string key) {
	std::map<std::string, std::string>::iterator it = _map.find(key);
	return (it != _map.end());
}

void GameStat::setPlayerId(int id) {
	setValue("pi", id);
}

void GameStat::setPlayerTier(int tier) {
	setValue("pt", tier);
}
int GameStat::getPlayerTier() {
	if (hasKey("pt"))
		return atoi(_map["pt"].c_str());
	else
		return 0;
}

int GameStat::getPlayerId() {
	if (hasKey("pi"))
		return atoi(_map["pi"].c_str());
	else
		return -1;
}

int GameStat::getRace() {
	if (hasKey("ra"))
		return atoi(_map["ra"].c_str());
	else
		return -1;
}

void GameStat::setRace(int id) {
	setValue("ra", id);
}

int GameStat::getGoldUp() {
	if (hasKey("gu"))
		return atoi(_map["gu"].c_str());
	else
		return 0;
}
void GameStat::setGoldUp(int goldUp) {
	setValue("gu", goldUp);
}

/**
 *	num-1-1		//for tier 1 unit 1
 */
int GameStat::getUnitNum(int tier, int unit) {
	std::string key = "num-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}
void GameStat::setUnitNum(int tier, int unit, int num) {
	std::string key = "num-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, num);
}

/**
 * Key: ar-1-1   //for armour tier 1 unit 1
 * Key: at-1-1	 //for attack
 */
int GameStat::getArmourUp(int tier, int unit) {
	std::string key = "ar-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}
void GameStat::setArmourUp(int tier, int unit, int armourUp) {
	std::string key = "ar-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, armourUp);
}

/**
 * Key: ar-1-1   //for armour tier 1 unit 1
 * Key: at-1-1	 //for attack
 */
int GameStat::getAttackUp(int tier, int unit) {
	std::string key = "at-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}

void GameStat::setAttackUp(int tier, int unit, int up) {
	std::string key = "at-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, up);
}

int GameStat::getSkillUp(int tier, int unit) {
	std::string key = "su-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}
void GameStat::setSkillUp(int tier, int unit, int up) {
	std::string key = "su-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, up);
}

int GameStat::getManaUp(int tier, int unit) {
	std::string key = "mu-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}
void GameStat::setManaUp(int tier, int unit, int up) {
	std::string key = "mu-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, up);
}

int GameStat::getManaRegUp(int tier, int unit) {
	std::string key = "mru-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}

void GameStat::setManaRegUp(int tier, int unit, int up) {
	std::string key = "mru-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, up);
}

int GameStat::getRangeUp(int tier, int unit) {
	std::string key = "ra-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}

void GameStat::setRangeUp(int tier, int unit, int up) {
	std::string key = "ra-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, up);
}

int GameStat::getLifeUp(int tier, int unit) {
	std::string key = "li-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}

void GameStat::setLifeUp(int tier, int unit, int lifeUp) {
	std::string key = "li-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, lifeUp);
}

int GameStat::getRunspeedUp(int tier, int unit) {
	std::string key = "rs-" + Tools::toString(tier) + "-" + Tools::toString(unit);

	if (hasKey(key))
		return atoi(_map[key].c_str());
	else
		return 0;
}

void GameStat::setRunspeedUp(int tier, int unit, int runspeedUp) {
	std::string key = "rs-" + Tools::toString(tier) + "-" + Tools::toString(unit);
	setValue(key, runspeedUp);
}

/**
 * 1-1-340;2-1-450
 * tier 1 unit 1 y: 340
 * tier 2 unit 1 y: 450
 */
//removed the returned spawn
//returns -1 if no spawn is left
int GameStat::getNextSpawnFor(int tier, int unit) {
	if (!hasKey("spawn") || _map["spawn"] == "")
		return -1;

	//search for this unit in string
	std::string s = _map["spawn"];

	//std::string s = ":2-3:123-;:1-1:345-;:2-3:123-";

	std::string find = ":" + Tools::toString(tier) + "-" + Tools::toString(unit) + ":";

	std::string::size_type loc = s.find(find, 0);
	if (loc != std::string::npos) {
		//extract number
		int num_start = loc + find.size();
		int num_length = s.find("-", num_start) - num_start;
		std::string num = s.substr(num_start, num_length);

		//erase the found position
		int eraseSize = find.size() + num_length + 1;
		if (s[loc + eraseSize] == ';') //maybe there is a ; at the end?
			eraseSize++;
		s.erase(loc, eraseSize);
		_map["spawn"] = s;

		return atoi(num.c_str());
	} else {
		return -1;
	}
}

void GameStat::addSpawnPosition(int tier, int unit, int position) {
	std::string connect = ";";
	if (!hasKey("spawn") || _map["spawn"] == "") {
		_map["spawn"] = ""; //initialize
		connect = "";
	}

	_map["spawn"] += ":" + Tools::toString(tier) + "-" + Tools::toString(unit) + ":" + Tools::toString(position) + "-";
}

void GameStat::clearSpawnPositions() {
	_map["spawn"] = "";
}
