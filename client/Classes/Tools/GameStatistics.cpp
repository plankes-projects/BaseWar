/*
 * Trigonometric.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "GameStatistics.h"
#include "cocos2d.h"
#include "../Model/Difficulty.h"
#include "Tools.h"
#include "../Model/Model.h"
using namespace cocos2d;

GameStatistics::GameStatistics() {

	easylocalGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesLost" + Tools::toString((int) DIF_EASY)).c_str(), 0);
	normallocalGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey("localGamesLost", 0);
	hardlocalGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesLost" + Tools::toString((int) DIF_HARD)).c_str(), 0);
	veryhardlocalGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesLost" + Tools::toString((int) DIF_VERYHARD)).c_str(), 0);
	helllocalGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesLost" + Tools::toString((int) DIF_HELL)).c_str(), 0);
	localGamesLost = easylocalGamesLost + normallocalGamesLost + hardlocalGamesLost + veryhardlocalGamesLost;

	easylocalGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesWon" + Tools::toString((int) DIF_EASY)).c_str(), 0);
	normallocalGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey("localGamesWon", 0);
	hardlocalGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesWon" + Tools::toString((int) DIF_HARD)).c_str(), 0);
	veryhardlocalGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesWon" + Tools::toString((int) DIF_VERYHARD)).c_str(), 0);
	helllocalGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey(("localGamesWon" + Tools::toString((int) DIF_HELL)).c_str(), 0);
	localGamesWon = easylocalGamesWon + normallocalGamesWon + hardlocalGamesWon + veryhardlocalGamesWon;

	localGamesStarted = CCUserDefault::sharedUserDefault()->getIntegerForKey("localGamesStarted", 0);

	localMultiplayerGamesStarted = CCUserDefault::sharedUserDefault()->getIntegerForKey("localMultiplayerGamesStarted", 0);
	localMultiplayerGamesFinished = CCUserDefault::sharedUserDefault()->getIntegerForKey("localMultiplayerGamesFinished", 0);

	networkGamesLost = CCUserDefault::sharedUserDefault()->getIntegerForKey("networkGamesLost", 0);
	networkGamesWon = CCUserDefault::sharedUserDefault()->getIntegerForKey("networkGamesWon", 0);
	networkGamesStarted = CCUserDefault::sharedUserDefault()->getIntegerForKey("networkGamesStarted", 0);

	secondsPlayed = CCUserDefault::sharedUserDefault()->getIntegerForKey("secondsPlayed", 0);

	bestSurviveInSec = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestSurviveInSec", 0);
}

int GameStatistics::getLocalGamesLost() {
	return localGamesLost;
}
int GameStatistics::getLocalGamesWon() {
	return localGamesWon;
}
int GameStatistics::getLocalGamesStarted() {
	return localGamesStarted;
}

int GameStatistics::getLocalMultiplayerGamesStarted() {
	return localMultiplayerGamesStarted;
}
int GameStatistics::getLocalMultiplayerGamesFinished() {
	return localMultiplayerGamesFinished;
}

int GameStatistics::getNetworkGamesLost() {
	return networkGamesLost;
}
int GameStatistics::getNetworkGamesWon() {
	return networkGamesWon;
}
int GameStatistics::getNetworkGamesStarted() {
	return networkGamesStarted;
}

int GameStatistics::getSecondsPlayed() {
	return secondsPlayed;
}

void GameStatistics::increaseLocalGamesLost() {
	localGamesLost++;
	Difficulty d = Model::getInstance()->getDifficulty();
	std::string key;
	if (d == DIF_NORMAL)
		key = "localGamesLost";
	else
		key = "localGamesLost" + Tools::toString((int) d);

	int num = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str(), 0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), num + 1);
}

void GameStatistics::increaseLocalGamesWon() {
	localGamesWon++;

	Difficulty d = Model::getInstance()->getDifficulty();
	std::string key;
	if (d == DIF_NORMAL)
		key = "localGamesWon";
	else
		key = "localGamesWon" + Tools::toString((int) d);

	int num = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str(), 0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), num + 1);
}

void GameStatistics::increaseLocalGamesStarted() {

	localGamesStarted++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("localGamesStarted", localGamesStarted);
}

void GameStatistics::increaseLocalMultiplayerGamesStarted() {

	localMultiplayerGamesStarted++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("localMultiplayerGamesStarted", localMultiplayerGamesStarted);
}
void GameStatistics::increaseLocalMultiplayerGamesFinished() {

	localMultiplayerGamesFinished++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("localMultiplayerGamesFinished", localMultiplayerGamesFinished);
}

void GameStatistics::increaseNetworkGamesLost() {

	networkGamesLost++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("networkGamesLost", networkGamesLost);
}
void GameStatistics::increaseNetworkGamesWon() {

	networkGamesWon++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("networkGamesWon", networkGamesWon);
}
void GameStatistics::increaseNetworkGamesStarted() {

	networkGamesStarted++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("networkGamesStarted", networkGamesStarted);
}

void GameStatistics::updateBestSurviveInSec(int bestSurviveInSecNew) {
	if (bestSurviveInSecNew < bestSurviveInSec)
		return;

	bestSurviveInSec = bestSurviveInSecNew;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("bestSurviveInSec", bestSurviveInSec);
}

void GameStatistics::increaseSecondsPlayedBy(int seconds) {
	secondsPlayed += seconds;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("secondsPlayed", secondsPlayed);
}

GameStatistics::~GameStatistics() {
	CCUserDefault::sharedUserDefault()->flush();
}


std::string GameStatistics::getLocalGameStatisticString(){
return std::string("dif/Win/Lose\n")
	+ "easy/" + Tools::toString(easylocalGamesWon) + "/" + Tools::toString(easylocalGamesLost) + "\n"
	+ "normal/" + Tools::toString(normallocalGamesWon) + "/" + Tools::toString(normallocalGamesLost) + "\n"
	+ "hard/" + Tools::toString(hardlocalGamesWon) + "/" + Tools::toString(hardlocalGamesLost) + "\n"
	+ "veryhard/" + Tools::toString(veryhardlocalGamesWon) + "/" + Tools::toString(veryhardlocalGamesLost) + "\n"
	+ "survival/" + Tools::toString(helllocalGamesWon) + "/" + Tools::toString(helllocalGamesLost);
}

