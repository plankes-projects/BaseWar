/*
 * InGameSpawnMenu.cpp
 *
 *  Created on: 03.06.2013
 *      Author: Planke
 */

#include "InGameSpawnInfo.h"
#include "../../../Model/Model.h"
#include "../../../Tools/Tools.h"
#include "../../../Tools/BW_Time.h"
#include "../../../Constants.h"
#include <string>

using namespace cocos2d;

bool InGameSpawnInfo::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 100, 255, 255)))
		return false;

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float width = 200 * scaleMult;
	float height = 100 * scaleMult;
	setContentSize(CCSize(width, height));

	float fontSize = 18 * scaleMult;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	setPosition(0, winSize.height - height);

	_timeOverLabel = CCLabelTTF::create("-", FONT_NAME_BOLD, fontSize);
	addChild(_timeOverLabel);

	_timeSpawnLabel = CCLabelTTF::create("-", FONT_NAME_BOLD, fontSize);
	addChild(_timeSpawnLabel);

	update(0);
	this->schedule(schedule_selector(InGameSpawnInfo::update), 1.0f);
	setOpacity(0); // 0- total transparent, 255 not
	return true;
}

void InGameSpawnInfo::update(float dt) {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	//float width = getContentSize().width;
	float height = getContentSize().height;
	float lineHeight = _timeSpawnLabel->getContentSize().height;
	float padding = 10 * scaleMult;

	int sec = BW_Time::getMilliSecondsOverCached() / 1000 + 1;
	int min = sec / 60;
	sec = sec % 60;
	std::string secString = sec > 9 ? Tools::toString(sec) : "0" + Tools::toString(sec);

	_timeOverLabel->setString(("Time: " + Tools::toString(min) + ":" + secString).c_str());
	_timeOverLabel->setPosition(CCPoint(padding + _timeOverLabel->getContentSize().width / 2, height - (lineHeight / 2 + 5 * scaleMult)));

	//intern label top and bottom padding is 5
	//update spawntimer

	int nextSpawnInSec;
	std::string nextOrYour;
	if(Model::getInstance()->getStartedAs() == NETWORK){
		nextOrYour = "Your";
		nextSpawnInSec = (int) (Model::getInstance()->getYourNextSpawn() / 1000);

	} else{
		nextOrYour = "Next";
		nextSpawnInSec = (int) (Model::getInstance()->getNextSpawn() / 1000);
	}

	std::string nextSpawnString = nextOrYour + " wave: " + Tools::toString(nextSpawnInSec);
	_timeSpawnLabel->setString(nextSpawnString.c_str());
	_timeSpawnLabel->setPosition(CCPoint(padding + _timeSpawnLabel->getContentSize().width / 2, height - (lineHeight / 2 + lineHeight)));

}
