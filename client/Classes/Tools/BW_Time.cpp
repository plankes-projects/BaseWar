/*
 * Time.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "BW_Time.h"
#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

#include "../Model/Model.h"
BW_Time::BW_Time() {
}

BW_Time::~BW_Time() {
}

double BW_Time::getMilliSeconds() {
	//ALERT! OVERFLOW POSSIBLE!
	double ret = 0;
#ifdef WIN32
	ret = clock() * 1000 / CLOCKS_PER_SEC;
#else
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	ret = millisecs;
#endif

	ret *= _gameSpeed;
	return ret;
}

int BW_Time::getSeconds() {
	return time(0);
}
/*
std::string BW_Time::integerToTimeString(int time) {

	char *Weekdays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	time_t curSec = time;
	struct tm *curDate;
	char dateString[32];

	curDate = localtime(&curSec);
	sprintf(dateString, "%s-%02d-%02d-%02d-%02d-%02d-%d", Weekdays[curDate->tm_wday], curDate->tm_mday, curDate->tm_hour, curDate->tm_min,
			curDate->tm_sec, curDate->tm_mon + 1, curDate->tm_year + 1900);
	return dateString;
}*/

std::string BW_Time::integerToDayTimeString(int time) {
	time_t curSec = time;
	struct tm *curDate;
	char dateString[32];

	curDate = localtime(&curSec);
	sprintf(dateString, "%02d-%02d-%d", curDate->tm_mday, curDate->tm_mon + 1, curDate->tm_year + 1900);
	return dateString;
}

double BW_Time::_stopDiff = 0;
void BW_Time::stopCachedMilliseconds() {
	_lastStop = getMilliSeconds();
}

double BW_Time::_lastStop;
void BW_Time::startCachedMilliseconds() {
	_stopDiff += getMilliSeconds() - _lastStop;
}

void BW_Time::resetStopDiff() {
	_stopDiff = 0;
}

double BW_Time::getMilliSecondsCached() {
	return BW_Time::_now;
}

double BW_Time::_now;
void BW_Time::updateCachedMilliseconds() {
	BW_Time::_now = getMilliSeconds() - _stopDiff;
}
float BW_Time::_gameSpeed = 1; //standard value
double BW_Time::_start;
void BW_Time::resetStartTime(float gameSpeed) {
	if (_gameSpeed != gameSpeed) {
		_gameSpeed = gameSpeed;
		updateCachedMilliseconds();
	}
	_start = _now;
}

double BW_Time::getMilliSecondsOverCached() {
	return _now - _start;
}
