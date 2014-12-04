#include "LoadingSpriteLayer.h"

#include "../../../SceneControl.h"
#include <string>
#include <list>
#include "../../../Tools/Tools.h"
#include "../../../Model/Model.h"
#include "../../../Constants.h"
#include "../../../Tools/SupportHandler.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Sound/SoundHandler.h"

// on "init" you need to initialize your instance
bool LoadingSpriteLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 0, 0, 0)))
		return false;

	_backGroundProgress = NULL;
	//start music
	SoundHandler::getInstance()->playBackground("sound_menu1");

	//same tablet detection used in Model.cpp
	float worldWidth = (float) cocos2d::CCDirector::sharedDirector()->getWinSize().width / (float) cocos2d::CCDevice::getDPI();
	bool tablet = worldWidth > 1.5f * 3.55633f;
	bool highResScreen = cocos2d::CCDevice::getDPI() > 240;

	std::string resolution = "low";
	//resolution = "";	//force high res
	if (tablet) {
		CCLOG("Tablet detected, loading high res images.");
		resolution = ""; // hd
	} else if (highResScreen) {
		CCLOG("High DPI screen detected, loading high res images.");
		resolution = ""; // hd
	} else if (SupportHandler::forceHighRes()) {
		CCLOG("High res forced.");
		resolution = ""; // hd
	}

	_toLoad.push_back("ninja.plist");
	_toLoad.push_back("modernSoldier.plist");
	_toLoad.push_back("noScale.plist");

	_toLoad.push_back("unit_tower" + resolution + ".plist");
	_toLoad.push_back("unit_base" + resolution + ".plist");

	_toLoad.push_back("achievements" + resolution + ".plist");
	_toLoad.push_back("background1" + resolution + ".plist");
	_toLoad.push_back("background2" + resolution + ".plist");
	_toLoad.push_back("background3" + resolution + ".plist");

	_toLoad.push_back("buttons1" + resolution + ".plist");
	_toLoad.push_back("buttons2" + resolution + ".plist");
	_toLoad.push_back("buttons3" + resolution + ".plist");
	_toLoad.push_back("buttons4" + resolution + ".plist");
	_toLoad.push_back("buttons5" + resolution + ".plist");
	_toLoad.push_back("gui" + resolution + ".plist");

	_toLoad.push_back("effects1" + resolution + ".plist");
	_toLoad.push_back("effects2" + resolution + ".plist");
	_toLoad.push_back("projectiles" + resolution + ".plist");

	_toLoad.push_back("slime1" + resolution + ".plist");
	_toLoad.push_back("defector1" + resolution + ".plist");
	_toLoad.push_back("flare_troll1" + resolution + ".plist");
	_toLoad.push_back("flare_troll2" + resolution + ".plist");
	_toLoad.push_back("flare_skelet1" + resolution + ".plist");
	_toLoad.push_back("flare_skelet2" + resolution + ".plist");
	_toLoad.push_back("flare_antlion1" + resolution + ".plist");
	_toLoad.push_back("flare_antlion2" + resolution + ".plist");

	_toLoad.push_back("unit_archer1" + resolution + ".plist");
	_toLoad.push_back("unit_archer2" + resolution + ".plist");
	_toLoad.push_back("unit_baron1" + resolution + ".plist");
	_toLoad.push_back("unit_baron2" + resolution + ".plist");
	_toLoad.push_back("unit_darkSaber1" + resolution + ".plist");
	_toLoad.push_back("unit_darkSaber2" + resolution + ".plist");
	_toLoad.push_back("unit_tank1" + resolution + ".plist");
	_toLoad.push_back("unit_tank2" + resolution + ".plist");
	_toLoad.push_back("unit_minion1" + resolution + ".plist");
	_toLoad.push_back("unit_minion2" + resolution + ".plist");
	_toLoad.push_back("unit_pinkKnight1" + resolution + ".plist");
	_toLoad.push_back("unit_pinkKnight2" + resolution + ".plist");
	_toLoad.push_back("unit_pinkKnight3" + resolution + ".plist");
	_toLoad.push_back("unit_robot1" + resolution + ".plist");
	_toLoad.push_back("unit_robot2" + resolution + ".plist");
	_toLoad.push_back("unit_soldier1" + resolution + ".plist");
	_toLoad.push_back("unit_soldier2" + resolution + ".plist");
	_toLoad.push_back("unit_soldier3" + resolution + ".plist");
	_toLoad.push_back("unit_troll1" + resolution + ".plist");
	_toLoad.push_back("unit_viking1" + resolution + ".plist");

	_toLoad.push_back("botanic_archer1" + resolution + ".plist");
	_toLoad.push_back("botanic_archer2" + resolution + ".plist");
	_toLoad.push_back("botanic_daemon1" + resolution + ".plist");
	_toLoad.push_back("botanic_daemon2" + resolution + ".plist");
	_toLoad.push_back("botanic_daemon3" + resolution + ".plist");
	_toLoad.push_back("botanic_dog1" + resolution + ".plist");
	_toLoad.push_back("botanic_dog2" + resolution + ".plist");
	_toLoad.push_back("botanic_femmage1" + resolution + ".plist");
	_toLoad.push_back("botanic_femmage2" + resolution + ".plist");
	_toLoad.push_back("botanic_knight1" + resolution + ".plist");
	_toLoad.push_back("botanic_mage1" + resolution + ".plist");
	_toLoad.push_back("botanic_priest1" + resolution + ".plist");
	_toLoad.push_back("botanic_priest2" + resolution + ".plist");
	_toLoad.push_back("botanic_priest3" + resolution + ".plist");
	_toLoad.push_back("golem1" + resolution + ".plist");
	_toLoad.push_back("sheep" + resolution + ".plist");

	_count = _toLoad.size();
	_counter = 1; //need to start with 1 because we update AFTER load

	float guiMult = Model::getInstance()->getGUIElementScaleMultiplicator();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(("preLoad" + resolution + ".plist").c_str());
	ViewTools::setStandardBackgroundImage(this);

	_loadingCalled = false;
	setPercent(0);
	this->schedule(schedule_selector(LoadingSpriteLayer::load), 0.1);

	CCLabelTTF *gameTip = CCLabelTTF::create(getRandomgameTip().c_str(), FONT_NAME_BOLD, 25 * guiMult, CCSizeMake(winSize.width - 20.0f * guiMult, 0),
			kCCTextAlignmentCenter);

	float offset = ViewTools::addVersionAndCreditInfo(this, guiMult);

	gameTip->setColor(ccc3(0, 0, 0));
	gameTip->setPosition(ccp(winSize.width / 2, gameTip->getContentSize().height / 2 + offset));
	this->addChild(gameTip);

	//set loading image
	cocos2d::CCSprite* backGroundLoadImage = CCSprite::createWithSpriteFrame(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("basewarFontIcon.png"));
	backGroundLoadImage->setScale((winSize.width / 2) / backGroundLoadImage->getContentSize().width);
	backGroundLoadImage->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
	addChild(backGroundLoadImage);
	backGroundLoadImage->setOpacity(100);

	return true;
}

void LoadingSpriteLayer::load(float dt) {
	//only call this function once
	if (_loadingCalled)
		return;
	_loadingCalled = true;

	//loading is finished, go on with start scene
	if (_toLoad.empty()) {
		CCLOG("Replace scene with startscene");

		SceneControl::replaceScene(SceneControl::startMenuScene(), true);
		return;
	}

	const char* toLoad = _toLoad.front().c_str();
	CCLOG("Loading: %s", toLoad);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(toLoad);
	_toLoad.pop_front();
	_loadingCalled = false;

	setPercent(100 * (_counter++) / _count);
}

std::string LoadingSpriteLayer::getRandomgameTip() {

	std::vector < std::string > randomGameTip;

	//advertisement
	randomGameTip.push_back("Tell your friends about BaseWar.");
	randomGameTip.push_back(std::string("Discuss BaseWar at\n") + WEBSITE_LOCATION);
	randomGameTip.push_back(std::string("Do you have some features in mind?\nTell me at\n") + WEBSITE_LOCATION);
	randomGameTip.push_back(std::string("Like BaseWar at fb, g+, reddit at\n") + WEBSITE_LOCATION);
	randomGameTip.push_back("Already rated BaseWar?");

	//tactics
	randomGameTip.push_back("Build melee units with a lot of health to hold the front.");
	randomGameTip.push_back("Build range units with a high damage output.");
	randomGameTip.push_back("Hitpoints and armor upgrades are most useful at melee units.");
	randomGameTip.push_back("Do not forget to upgrade your units.");
	randomGameTip.push_back("Build units which counter enemy units (Hint: damage type).");
	randomGameTip.push_back("Do not underestimate the remove buff ability.");
	randomGameTip.push_back("Tanks are units at the front who take the damage.");
	randomGameTip.push_back("Damage dealer are units with a high damage output.");

	//we want this one to get a high chance ofshowing
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	randomGameTip.push_back(Tools::toString(UNITS_TO_ULTIMATE_UNIT) + " units of the same type merge into a champion unit with "+ Tools::toString(UNITS_TO_ULTIMATE_UNIT_MULT)+ "x strength.");
	//randomGameTip.push_back(Tools::toString(UNITS_TO_MASTER_UNIT) + " units of the same type merge into a master unit.");

	//explanation
	randomGameTip.push_back("Buffs are positive temporary effects.\nRemovable by some units.");
	randomGameTip.push_back("Debuffs are negative temporary effects.\nRemovable by some units.");
	randomGameTip.push_back("Observing enemie's damage type is very important.");

	//others
	randomGameTip.push_back("Have fun...");
	randomGameTip.push_back("There is a hidden secret..."); // no, there is no secret :P
	randomGameTip.push_back("Already won survival mode?");

	//jokes
	randomGameTip.push_back("Chuck Norris counted to infinity - twice.");
	randomGameTip.push_back("Change 4 letters in \"mama\" and you get \"beer\".");

	return randomGameTip.at(Tools::random(0, randomGameTip.size() - 1));
}

void LoadingSpriteLayer::setPercent(float percent) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	_backGroundProgress = CCProgressTimer::create(
			CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("basewarFontIcon.png")));
	_backGroundProgress->setType(kCCProgressTimerTypeBar);
	//    Setup for a bar starting from the left since the midpoint is 0 for the x
	_backGroundProgress->setMidpoint(ccp(0, 0));
	//    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
	_backGroundProgress->setBarChangeRate(ccp(1, 0));
	_backGroundProgress->setScale((winSize.width / 2) / _backGroundProgress->getContentSize().width);
	_backGroundProgress->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
	//left->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
	addChild(_backGroundProgress);
	CCProgressTo *to = CCProgressTo::create(0, percent);
	_backGroundProgress->runAction(to);

	//if we delete existing one, it "flackert" =)

}
