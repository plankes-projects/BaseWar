#include "BalanceSingleton.h"
#include "../Constants.h"
#include "../Tools/Tools.h"
#include "../Model/Model.h"

BalanceSingleton * BalanceSingleton::getInstance() {
	return _instance;
}

bool BalanceSingleton::isAnotherRound() {
	return _roundsDone < BALANCE_TEST_GAME_ROUNDS;
}

void BalanceSingleton::increaseRace1Win() {
	_raceId1Wins++;
}
void BalanceSingleton::increaseRace2Win() {
	_raceId2Wins++;
}

std::string BalanceSingleton::getInformation() {
	std::string linebegin = "STAT - ";
	std::string s = "\n";
	s += linebegin + "------------- BALANCE STATISTIC BEGIN -------------\n";
	s += linebegin + "Run: " + Tools::toString(_roundsDone) + "/" + Tools::toString(BALANCE_TEST_GAME_ROUNDS) + "\n";
	s += linebegin + "Stuck: " + Tools::toString(_gamesStuck) + "\n";
	s += linebegin + "Gamespeed: " + Tools::toString((int) BALANCE_TEST_GAME_SPEED) + "\n";

	int sec = _seconds;
	int min = sec / 60;
	sec = sec % 60;
	std::string secString = sec > 9 ? Tools::toString(sec) : "0" + Tools::toString(sec);
	s += linebegin + "Duration: " + Tools::toString(min) + ":" + secString + "\n";

	sec = _seconds / (_raceId1Wins + _raceId2Wins);
	min = sec / 60;
	sec = sec % 60;
	secString = sec > 9 ? Tools::toString(sec) : "0" + Tools::toString(sec);
	s += linebegin + "Duration / run: " + Tools::toString(min) + ":" + secString + "\n";

	sec = _seconds / Model::getInstance()->getGameSpeed();
	min = sec / 60;
	sec = sec % 60;
	secString = sec > 9 ? Tools::toString(sec) : "0" + Tools::toString(sec);
	s += linebegin + "RealTime duration: " + Tools::toString(min) + ":" + secString + "\n";

	sec = _seconds / Model::getInstance()->getGameSpeed() / (_raceId1Wins + _raceId2Wins);
	min = sec / 60;
	sec = sec % 60;
	secString = sec > 9 ? Tools::toString(sec) : "0" + Tools::toString(sec);
	s += linebegin + "RealTime duration / run: " + Tools::toString(min) + ":" + secString + "\n";

	s += linebegin + "Race1: " + Tools::toString(_raceId1) + " - Wins: " + Tools::toString(_raceId1Wins) + "/"
			+ Tools::toString(_raceId1Wins + _raceId2Wins) + "\n";
	s += linebegin + "Race2: " + Tools::toString(_raceId2) + " - Wins: " + Tools::toString(_raceId2Wins) + "/"
			+ Tools::toString(_raceId1Wins + _raceId2Wins) + "\n";
	s += linebegin + "-------------- BALANCE STATISTIC END --------------";
	return s;
}

void BalanceSingleton::addDuration(int seconds) {
	_seconds += seconds;
}

void BalanceSingleton::reset() {
	if (_instance != NULL)
		delete _instance;

	_instance = new BalanceSingleton();
}

void BalanceSingleton::increaseRound() {
	_roundsDone++;
}
BalanceSingleton::~BalanceSingleton() {

}

BalanceSingleton::BalanceSingleton() {
	_raceId1 = 0;
	_raceId2 = 0;
	_raceId1Wins = 0;
	_raceId2Wins = 0;
	_roundsDone = 0;
	_seconds = 0;
	_gamesStuck = 0;
}

BalanceSingleton* BalanceSingleton::_instance = NULL;
