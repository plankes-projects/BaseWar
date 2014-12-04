/*
 * NetworkSingleton.cpp
 *
 *  Created on: 23.07.2013
 *      Author: Planke
 */

#include "NetworkSingleton.h"
#include "ServerCommunication.h"
#include "../Model/Player.h"
#include "../Model/Model.h"
#include "../Tools/Tools.h"

#include "cocos2d.h"
using namespace cocos2d;

NetworkSingleton::NetworkSingleton() {
	reset();

	_deviceId = CCUserDefault::sharedUserDefault()->getStringForKey("deviceId", "");

	if (_deviceId.length() < 15) {
		_deviceId = Tools::getRandomString(15);
		CCUserDefault::sharedUserDefault()->setStringForKey("deviceId", _deviceId.c_str());
	}
	//CCLOG("DeviceID: %s", _deviceId.c_str());
	//add OS to device ID
	std::string os = "OTH";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	os = "IOS";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	os = "AND";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	os = "LIN";
#endif

	// + additional random string for launching 2 instances of the game (eg linux)
	_deviceId = os + ":" + _deviceId + ":" + Tools::getRandomString(6);
}

NetworkSingleton::~NetworkSingleton() {
}

NetworkSingleton* NetworkSingleton::_instance = NULL;
NetworkSingleton* NetworkSingleton::getInstance() {
	if (_instance == NULL)
		_instance = new NetworkSingleton();

	return _instance;
}

void NetworkSingleton::updateSpawnPositionVector(ArmyType armytype) {
	if (_yRandomsLeft.empty() || _yRandomsRigth.empty()) {
		ServerCommunication sc = ServerCommunication();
		bool myArmy = armytype == Model::getInstance()->getThisPlayer()->getArmyTyp();
		if (armytype == LEFT)
			sc.getRandomSpawnsFor(myArmy, _waveNum, this, httpresponse_selector(NetworkSingleton::onGetLeftRandomSpawnsCompleted));
		else
			sc.getRandomSpawnsFor(myArmy, _waveNum, this, httpresponse_selector(NetworkSingleton::onGetRightRandomSpawnsCompleted));
	}
}

void NetworkSingleton::onGetLeftRandomSpawnsCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response)
		return;

	if (!response->isSucceed()) {
		CCLog("server get info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	_yRandomsLeft = parseRandomSpawnpositionResponse(Tools::toString(response->getResponseData()));
}
void NetworkSingleton::onGetRightRandomSpawnsCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response)
		return;

	if (!response->isSucceed()) {
		CCLog("server get info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	_yRandomsRigth = parseRandomSpawnpositionResponse(Tools::toString(response->getResponseData()));
}

std::vector<int> NetworkSingleton::parseRandomSpawnpositionResponse(std::string resp) {
	std::vector<int> ret = std::vector<int>();

	//now parse the string
	std::istringstream f(resp);
	std::string token;
	while (std::getline(f, token, ';')) {
		ret.push_back(atoi(token.c_str()));
	}

	return ret;
}

void NetworkSingleton::increaseWaveNum() {
	_waveNum++;
}

void NetworkSingleton::clearSpawnPositionVectors() {

	_yRandomsLeft = std::vector<int>();
	_yRandomsRigth = std::vector<int>();

}
int NetworkSingleton::getNextSpawnPosition(ArmyType armyType) {
	int ret = 0;
	if (armyType == LEFT) {
		if (_yRandomsLeft.empty())
			return -1;

		ret = _yRandomsLeft.back();
		_yRandomsLeft.pop_back();
	} else if (armyType == RIGHT) {
		if (_yRandomsRigth.empty())
			return -1;

		ret = _yRandomsRigth.back();
		_yRandomsRigth.pop_back();
	}
	return ret;
}

void NetworkSingleton::reset() {
	//unlock if locked
	_gameStart = -1;
	_gameStats = std::vector<GameStat>();
	_names = std::vector<std::string>();
	_myGameStat = GameStat();
	_waveNum = 0;
	clearSpawnPositionVectors();
}

void NetworkSingleton::updateNetwork() {
	CCLOG("Update routine started");
	//0. get spawn coordinates for next round
	getInstance()->updateSpawnPositionVector(LEFT);
	getInstance()->updateSpawnPositionVector(RIGHT);

	//1. push my gamestat to server
	ServerCommunication sc = ServerCommunication();
	sc.sendGameStat(getInstance()->_myGameStat);
	CCLOG("Sent my gamstat to server");

	//2. pull game stats and sync them
	sc.getGameStats(this, httpresponse_selector(NetworkSingleton::onGetGamestatsCompleted));
}

void NetworkSingleton::onGetGamestatsCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response)
		return;

	if (!response->isSucceed()) {
		CCLog("server get info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::string resp = Tools::toString(response->getResponseData());
	//parse response string to gamestats
	std::vector<GameStat> vector;
	std::istringstream ss(resp);
	std::string token;
	char delemiter = GameStat().getGameStatDelemiter();
	while (std::getline(ss, token, delemiter)) {
		vector.push_back(GameStat(token));
	}

	//process gamestats
	_gameStats = vector;
	CCLOG("Got gamestats from server anz: %li", getInstance()->_gameStats.size());

	//3. sync local gamestats with players
	//generate map with id => player
	std::map<int, Player*> playerMap;
	std::list<Player*> players = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it)
		playerMap[(*it)->_networkPlayerId] = (*it);

	players = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it)
		playerMap[(*it)->_networkPlayerId] = (*it);

	//unset my id
	std::map<int, Player*>::iterator it;
	it = playerMap.find(getInstance()->_myGameStat.getPlayerId());
	playerMap.erase(it);

	int numOfUpdatedPlayers = 0;
	for (unsigned int i = 0; i < getInstance()->_gameStats.size(); i++) {
		GameStat current = getInstance()->_gameStats[i];
		//search for player
		Player* player = 0;

		for (it = playerMap.begin(); it != playerMap.end(); ++it) {
			if (it->first == current.getPlayerId()) {
				player = it->second;
				playerMap.erase(it); //removing the updated player
				numOfUpdatedPlayers++;
				break;
			}
		}

		if (player == 0) {
			CCLOG("Could not find network player id %i", current.getPlayerId());
		} else {
			player->updateWithgameStat(current);
			CCLOG("Updated network player with id %i", current.getPlayerId());
		}
	}
	CCLOG("Updated all network palyers");

	//for all players left, we get no updates anymore --> set to AI
	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		Player* p = it->second;
		if (!p->isAI()) {
			Player* p = it->second;
			p->changeNetworkPlayerToAIPlayer();
			CCLOG("%s left. Is AI now.", p->getName().c_str());

			if(!fakeNetworkGame){
				//show a message to user
				Model::getInstance()->getInGameGameLayer()->showPlayerLeftMessage(p->getName());
			}
		}
	}

	if (numOfUpdatedPlayers == 0) {
		//no network player left, switch to local mode
		Model::getInstance()->setGameMode(PVA);
		CCLOG("No network player left, switch to gamemode PVA.");
	}
}
