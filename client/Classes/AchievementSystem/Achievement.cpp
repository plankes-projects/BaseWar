/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "Achievement.h"
#include "../Tools/BW_Time.h"

#include "cocos2d.h"
using namespace cocos2d;

Achievement::Achievement(std::string id, std::string image, std::string title, std::string description, int points) {
	_persistentLoadSaveId = "achievement" + id;
	_image = image;
	_title = title;
	_description = description;
	_points = points;

	//if id == "" we will load it manually in the inheranted class
	if (id.size() != 0)
		_timeAchieved = getAchievementAchievedPersistent();
	else
		_timeAchieved = -1;
}

bool Achievement::updateAchievement() {
	if (_timeAchieved == -1 && isAchievedInternCalculation()) {
		//something went wrong at setting this achievement as achieved.
		//do it now!
		setAchievedNow();
		return true;
	}
	return false;
}

Achievement::~Achievement() {
}

void Achievement::setAchievedNow() {
	int sec = BW_Time::getSeconds();
	setAchievementAchievedPersistent(sec);
	_timeAchieved = sec; // time achieved needs to be set after persistent!!
}

void Achievement::setAchievementAchievedPersistent(int time) {
	if (isAchieved())
		return;

	CCUserDefault::sharedUserDefault()->setIntegerForKey(_persistentLoadSaveId.c_str(), time);
	CCUserDefault::sharedUserDefault()->flush();
}

int Achievement::getAchievementAchievedPersistent() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(_persistentLoadSaveId.c_str(), -1);
}
