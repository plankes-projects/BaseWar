/*
 * Point.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "OfflineLogger.h"
#include "Tools.h"
#include "../Constants.h"
#include "../Model/Model.h"
#include "BW_Time.h"

OfflineLogger* OfflineLogger::_instance = NULL;
OfflineLogger* OfflineLogger::getInstance() {
	if (_instance == NULL)
		_instance = new OfflineLogger();

	return _instance;
}

OfflineLogger::OfflineLogger() {
	if (!LOG_OFFLINE_GAMES)
		return;

	_currentLogSpotNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("offlineLogSpotNumber", 0);
	_currentLogSpotNumber %= MAX_OFFLINE_GAMES_TO_LOG; //to avoid bugs if MAX_OFFLINE_GAMES_TO_LOG is getting changed
	_currentLogSpot = "";
	_lineDelemiter = "<n/>";
	_timeDelemiter = "<t/>";
	_gameStatDelemiter = "<g/>";
	_lastLog = "";
}

void OfflineLogger::initNewGameLog() {
	if (!LOG_OFFLINE_GAMES)
		return;

	_lastLog = "";
	_currentLogSpotNumber++;
	_currentLogSpotNumber %= MAX_OFFLINE_GAMES_TO_LOG;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("offlineLogSpotNumber", _currentLogSpotNumber);
	CCUserDefault::sharedUserDefault()->flush();
	_currentLogSpot = std::string("offlineLogSpot") + Tools::toString(_currentLogSpotNumber);
	_currentGameLog = ""; //reset the log

	logInformationForCurrentGame(Model::getInstance()->getStartedAs() == PVA ? "PVA" : "PVP");

	//now log init game
	std::list<Player*> p = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
	if (p.size() != 1) {
		CCLOG("INVALID PLAYER NUM AT LEFT SIDE!!! IS THIS REALLY AN OFFLINE GAME?!");
		return;
	}
	Race* race1 = p.front()->getRace();

	p = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
	if (p.size() != 1) {
		CCLOG("INVALID PLAYER NUM AT RIGHT SIDE!!! IS THIS REALLY AN OFFLINE GAME?!");
		return;
	}
	Race* race2 = p.front()->getRace();

	int raceId1 = race1->getRaceId();
	std::string randomRaceString1 = race1->getRandomRaceString();

	int raceId2 = race2->getRaceId();
	std::string randomRaceString2 = race2->getRandomRaceString();

	logInformationForCurrentGame(Tools::toString(raceId1) + ":" + randomRaceString1 + _gameStatDelemiter + Tools::toString(raceId2) + ":" + randomRaceString2);
}

void OfflineLogger::logInformationForCurrentGame(std::string information) {
	if (!LOG_OFFLINE_GAMES)
		return;

	if (_currentLogSpot == "") {
		CCLOG("WHAAAAAAT!!! INIT THE LOGGER BEFORE U USE IT!");
		return;
	}

	int now = BW_Time::getSeconds();
	_currentGameLog += Tools::toString(now) + _timeDelemiter + information + _lineDelemiter;
	CCUserDefault::sharedUserDefault()->setStringForKey(_currentLogSpot.c_str(), _currentGameLog.c_str());
	CCUserDefault::sharedUserDefault()->flush();
}

std::string OfflineLogger::getOldestGameLogAndClear() {
	int oldestLogSpotNumber = _currentLogSpotNumber;

	do {
		//current +1 spot = oldest spot
		oldestLogSpotNumber = (oldestLogSpotNumber + 1) % MAX_OFFLINE_GAMES_TO_LOG;
		std::string key = std::string("offlineLogSpot") + Tools::toString(oldestLogSpotNumber);
		std::string log = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), "");
		if (log != "") {
			//remove log and return it
			CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), "");
			CCUserDefault::sharedUserDefault()->flush();
			return log;
		}

	} while (oldestLogSpotNumber != _currentLogSpotNumber); //check at least every log spot
	return ""; //no log found
}

void OfflineLogger::logSpawn() {
	std::list<Player*> p = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
	if (p.size() != 1) {
		CCLOG("INVALID PLAYER NUM AT LEFT SIDE!!! IS THIS REALLY AN OFFLINE GAME?!");
		return;
	}
	Player* player1 = p.front();

	p = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
	if (p.size() != 1) {
		CCLOG("INVALID PLAYER NUM AT RIGHT SIDE!!! IS THIS REALLY AN OFFLINE GAME?!");
		return;
	}
	Player* player2 = p.front();

	std::string newLog = player1->getGameStat().toString() + _gameStatDelemiter + player2->getGameStat().toString();
	if(newLog != _lastLog){
		_lastLog = newLog;
		logInformationForCurrentGame(_lastLog);
	}
}
