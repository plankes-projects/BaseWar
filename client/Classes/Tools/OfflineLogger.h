/*
 * Point.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef OfflineLogger_H_
#define OfflineLogger_H_

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class OfflineLogger {
public:
	static OfflineLogger* getInstance();
	void initNewGameLog();
	void logSpawn();
	void logInformationForCurrentGame(std::string information);
	std::string getOldestGameLogAndClear();

private:

	static OfflineLogger* _instance;
	OfflineLogger();

	int _currentLogSpotNumber;
	std::string _currentLogSpot;
	std::string _currentGameLog;
	std::string _lineDelemiter;
	std::string _timeDelemiter;
	std::string _gameStatDelemiter;
	std::string _lastLog;
};

#endif /* POINT_H_ */
