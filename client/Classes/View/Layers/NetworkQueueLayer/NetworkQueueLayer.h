#ifndef __NETWORKQUEUELAYER_H__
#define __NETWORKQUEUELAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class NetworkQueueLayer: public CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void leaveQueueTouched(CCObject* pSender);
	void setPlayerName(std::string playerName);
	void setRaceId(int raceId);

	// implement the "static node()" method manually
	CREATE_FUNC (NetworkQueueLayer);
private:
	void onTeamsizeConfigCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void onPlayerNumInfoCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void onGetServerInfoCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void onQueueCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void completeQueueRequest(int code, std::string firstServerLine);
	void queuePart();
	void gameCounterPart();
	void initAINetworkPlayers();
	std::vector<std::string> getFakePlayerNames();

	bool _foundGame;
	bool _gameStarted;
	std::string _randomRaceString;
	int _raceId;
	std::string _playerName;
	void update(float dt);
	bool _sendingRequest;
	float _send_time;
	CCLabelTTF* _countDown;
	CCMenuItemSprite * _leaveQueue;

	CCLabelTTF* _serverMessage;
	CCLabelTTF* _serverInformation;
	bool _loadedServerInformation;
	bool _loadedPlayerNumInformation;
	bool _loadedTeamsizeConfig;

	int _queueTimeInSec;
	int _numOfPlayersOnline;
	int _teamsizeConfig;
	int _maxSecToStart;

	bool _playerSet;
};

#endif // __NETWORKQUEUELAYER_H__
