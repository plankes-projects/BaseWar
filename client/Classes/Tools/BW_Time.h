/*
 * Timer.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef BW_TIME_H_
#define BW_TIME_H_

#include <string>

class BW_Time {
public:
	BW_Time();
	virtual ~BW_Time();
	static double getMilliSecondsCached();
	static double getMilliSeconds();
	static int getSeconds();
	static void updateCachedMilliseconds();
	static void stopCachedMilliseconds();
	static void startCachedMilliseconds();
	static void resetStopDiff();

	static double getMilliSecondsOverCached();
	static void resetStartTime(float gameSpeed = 1);

	//static std::string integerToTimeString(int time);
	static std::string integerToDayTimeString(int time);
private:
	static double _now;

	static double _stopDiff;
	static double _lastStop;
	static double _start;

	static float _gameSpeed;
};

#endif /* BW_TIME_H_ */
