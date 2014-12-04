/*
 * NetworkSingleton.h
 *
 *  Created on: 23.07.2013
 *      Author: Planke
 */

#ifndef NETWORKSINGLETON_H_
#define NETWORKSINGLETON_H_

#include <vector>
#include "GameStat.h"
#include "../Model/ArmyType.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class NetworkSingleton : public CCObject {
public:
	virtual ~NetworkSingleton();
	static NetworkSingleton* getInstance();
	int _gameStart;
	std::vector<GameStat> _gameStats;
	std::vector<std::string> _names;
	GameStat _myGameStat;
	void reset();
	std::string _deviceId;

	void updateNetwork();
	std::string _myName;

	void updateSpawnPositionVector(ArmyType armytype);
	void clearSpawnPositionVectors();
	int getNextSpawnPosition(ArmyType armyType);
	void increaseWaveNum();
	bool fakeNetworkGame;
private:
	void onGetGamestatsCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void onGetLeftRandomSpawnsCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void onGetRightRandomSpawnsCompleted(CCHttpClient *sender, CCHttpResponse *response);
	std::vector<int> parseRandomSpawnpositionResponse(std::string resp);

	static void* networkUpdateThread(void* arg);

	NetworkSingleton();
	static NetworkSingleton* _instance;
	int	_queuePollDelay;
	int _syncGameDelay;
	int _waveNum;
	std::vector<int> _yRandomsLeft;
	std::vector<int> _yRandomsRigth;
};

#endif /* NETWORKSINGLETON_H_ */
