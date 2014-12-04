/*
 * RendevouzCommunication.h
 *
 *  Created on: 01.06.2013
 *      Author: Planke
 */

#ifndef SERVERCOMMUNICATION_H_
#define SERVERCOMMUNICATION_H_

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include "GameStat.h"

class ServerCommunication: public cocos2d::CCNode {
public:
	ServerCommunication();
	virtual ~ServerCommunication();

	/**
	 * queues the client
	 * sending this request all x seconds
	 * returns >100 int which indicates the milliseconds until start of the game
	 * returns 0 if in the queue
	 * returns 2 if joined a game
	 * returns -1 if no connection
	 * returns -2 if invalid init message
	 * returns -3 if invalid api key
	 *
	 *	We want to be min 5 seconds in queue, for exidental queueing
	 *	queuing 5 times in 5 min --> block for 10 min
	 */
	void queueMe(int raceId, std::string playerName, std::string randomRaceString, CCObject* pTarget, SEL_HttpResponse pSelector);

	/**
	 * Sending an serializeable object which represent my game stat
	 */
	void sendGameStat(GameStat gameStat);

	/**
	 *	getting the game stats of other players
	 */
	void getGameStats(CCObject* pTarget, SEL_HttpResponse pSelector);

	/**
	 * adding info to the current game
	 */
	void sendGameInfo(std::string info);
	void sendGameProgress();
	void sendOldestOfflineLog();

	void getServerinformation(CCObject* pTarget, SEL_HttpResponse pSelector);
	void getPlayerNum(CCObject* pTarget, SEL_HttpResponse pSelector);
	void getTeamsizeConfig(CCObject* pTarget, SEL_HttpResponse pSelector);

	void sendLog(std::string data, CCObject* pTarget, SEL_HttpResponse pSelector);

	void sendFeedback(std::string data, CCObject* pTarget, SEL_HttpResponse pSelector);


	void getRandomSpawnsFor(bool me, int wave, CCObject* pTarget, SEL_HttpResponse pSelector);

	void printResultCallback(CCHttpClient *sender, CCHttpResponse *response);
private:
	std::string escape(std::string data);
	std::string _apiKey;
	std::string _host;

	std::string _urlQueue;
	std::string _urlSetGamestat;
	std::string _urlGetGamestats;
	std::string _urlSendGameInfo;
	std::string _urlSendGameProgress;
	std::string _urlSendFeedback;

	std::string _urlGetServerInformation;
	std::string _urlLogger;
	std::string _urlGetPlayerNum;
	std::string _urlGetRandomSpawn;
	std::string _urlGetTeamsizeConfig;
	std::string _urlSendOfflineLog;
};

#endif /* SERVERCOMMUNICATION_H_ */
