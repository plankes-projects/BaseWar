/*
 * RendevouzCommunication.cpp
 *
 *  Created on: 01.06.2013
 *      Author: Planke
 */

#include "ServerCommunication.h"
#include "NetworkSingleton.h"
#include "../Tools/BW_Time.h"
#include "../Tools/Tools.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../Constants.h"
#include "../Tools/GameStatistics.h"
#include "../AchievementSystem/AchievementSystem.h"
#include "../AchievementSystem/Achievement.h"
#include "../Tools/OfflineLogger.h"

ServerCommunication::ServerCommunication() {
	_apiKey = APIKEY;
	_host = "http://basewar.net/server/";
	//_host = "localhost/basewar/server/";

	_urlQueue = _host + "game.php?API_KEY=" + _apiKey + "&REQ=queue";
	_urlSetGamestat = _host + "game.php?API_KEY=" + _apiKey + "&REQ=setGamestat";
	_urlGetGamestats = _host + "game.php?API_KEY=" + _apiKey + "&REQ=getGamestat";
	_urlSendGameInfo = _host + "game.php?API_KEY=" + _apiKey + "&REQ=addGameInfo";

	_urlGetServerInformation = _host + "info.php?API_KEY=" + _apiKey;
	_urlLogger = _host + "info.php?API_KEY=" + _apiKey;
	_urlGetPlayerNum = _host + "game.php?REQ=playerNum";
	_urlGetTeamsizeConfig = _host + "game.php?REQ=teamSize";
	_urlGetRandomSpawn = _host + "game.php?API_KEY=" + _apiKey + "&REQ=getRandomSpawns";

	_urlSendFeedback = _host + "insertFeedback.php?VERSION=" + VERSION;
	_urlSendGameProgress = _host + "insertGameProgress.php";
	_urlSendOfflineLog = _host + "insertOfflineLog.php";
}

ServerCommunication::~ServerCommunication() {

}

void ServerCommunication::getRandomSpawnsFor(bool me, int wave, CCObject* pTarget, SEL_HttpResponse pSelector) {
	std::string data = (me ? "1" : "0");
	data += Tools::toString(wave);

	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlGetRandomSpawn.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(data);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));

	CCHttpClient::getInstance()->send(request);
	request->release();

}

void ServerCommunication::getPlayerNum(CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlGetPlayerNum.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::getTeamsizeConfig(CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlGetTeamsizeConfig.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::sendFeedback(std::string feedbackString, CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlSendFeedback.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(feedbackString);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::sendLog(std::string data, CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlLogger.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::getServerinformation(CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlGetServerInformation.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape("");
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::queueMe(int raceId, std::string playerName, std::string randomRaceString, CCObject* pTarget, SEL_HttpResponse pSelector) {
	//1;123-132-41-12-412-423-1-412-1;Klaus
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlQueue.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data="
			+ escape(Tools::toString(raceId) + ";" + randomRaceString + ";" + playerName);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));

	CCHttpClient::getInstance()->send(request);
	request->release();
	CCLOG("Send data: %s", data.c_str());
}

void ServerCommunication::sendGameStat(GameStat gameStat) {
	/** Server returns:
	 * updated: ok
	 */

	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(_urlSetGamestat.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(gameStat.toString());
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	CCHttpClient::getInstance()->send(request);
	request->release();

	CCLOG("Sending gamestat: %s", gameStat.toString().c_str());
}

/**
 *	getting the game stats of other players
 */
void ServerCommunication::getGameStats(CCObject* pTarget, SEL_HttpResponse pSelector) {
	CCHttpRequest* request = new CCHttpRequest();
	request->setResponseCallback(pTarget, pSelector);
	request->setUrl(_urlGetGamestats.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=";
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	// write the post data
	CCHttpClient::getInstance()->send(request);
	request->release();
	CCLOG("Sending get gamestat");

}

void ServerCommunication::sendGameInfo(std::string info) {
	/** Server returns:
	 * updated: ok
	 */
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(_urlSendGameInfo.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(info);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	CCHttpClient::getInstance()->send(request);
	request->release();
	CCLOG("Sending game info");
}

void ServerCommunication::sendOldestOfflineLog() {
	/** Server returns:
	 * updated: ok
	 */
	std::string progress = OfflineLogger::getInstance()->getOldestGameLogAndClear();
	if (progress == ""){
		CCLOG("No offline log available");
		return; //no log available
	}

	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(_urlSendOfflineLog.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
	//request->setResponseCallback(this, httpresponse_selector(ServerCommunication::printResultCallback));

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(progress);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::sendGameProgress() {
	/** Server returns:
	 * updated: ok
	 */
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(_urlSendGameProgress.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	AchievementSystem as = AchievementSystem();
	//sending game stats for statistics
	GameStatistics gs = GameStatistics();
	std::string statString = gs.getLocalGameStatisticString();

	std::vector<Achievement*> achs = as.getAchievements();
	std::string achString = "";
	//TODO BUILD STAT STRING
	for (unsigned int i = 0; i < achs.size(); i++) {
		if (achs[i]->isAchieved()) {
			achString += Tools::toString(achs[i]->getPoints()) + " - " + Tools::toString(achs[i]->getTimeAchieved()) + " - " + achs[i]->getDescription() + "\n";
		}
	}

	std::string delemiter = "<end/>\n";
	std::string progress = statString + delemiter + Tools::toString(gs.getLocalGamesStarted()) + delemiter + Tools::toString(gs.getNetworkGamesStarted())
			+ delemiter + Tools::toString(gs.getNetworkGamesWon()) + delemiter + Tools::toString(gs.getNetworkGamesLost()) + delemiter
			+ Tools::toString(gs.getLocalMultiplayerGamesStarted()) + delemiter + Tools::toString(gs.getLocalMultiplayerGamesFinished()) + delemiter
			+ Tools::toString(gs.getSecondsPlayed()) + delemiter + Tools::toString(as.getPoints()) + delemiter + achString + delemiter;

	std::string data = "deviceID=" + escape(NetworkSingleton::getInstance()->_deviceId) + "&data=" + escape(progress);
	// write the post data
	const char* postData = data.c_str();
	request->setRequestData(postData, strlen(postData));
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ServerCommunication::printResultCallback(CCHttpClient *sender, CCHttpResponse *response) {
	//usage: request->setResponseCallback(this, httpresponse_selector(ServerCommunication::printResultCallback));
	if (!response) {
		CCLog("Response missing");
	} else if (!response->isSucceed()) {
		CCLog("Response failed");
	} else {
		CCLog("RESULT:\n%s", Tools::toString(response->getResponseData()).c_str());
	}
}

std::string ServerCommunication::escape(std::string data) {
	std::replace(data.begin(), data.end(), '&', ' ');
	std::replace(data.begin(), data.end(), '?', ' ');
	return data;
}
