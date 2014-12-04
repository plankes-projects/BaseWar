#include "InGamePauseLayer.h"
#include "../../../Tools/BW_Time.h"
#include "../../../Model/Model.h"
#include "../../../Constants.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool InGamePauseLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 0)))
		return false;

	//stop push through
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenu* fallback = CCMenu::create();
	CCMenuItemSprite* button = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pauseLayerColor.png"),
			CCSprite::createWithSpriteFrameName("pauseLayerColor.png"), (CCObject*) this, menu_selector(InGamePauseLayer::resumeTouched));
	button->setScaleX(winSize.width / button->getContentSize().width);
	button->setScaleY(winSize.height / button->getContentSize().height);
	fallback->addChild(button, 1);
	fallback->setTouchPriority(-30000);

	fallback->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(fallback);

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	cocos2d::CCLabelTTF* information = CCLabelTTF::create("Tap to continue...", FONT_NAME, 30 * scaleMult);
	information->setColor(ccc3(255, 255, 255));
	information->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
	this->addChild(information);

	pauseGame();
	return true;
}

void InGamePauseLayer::pauseGame() {
	Model::getInstance()->setGamePaused(true);
	BW_Time::stopCachedMilliseconds();
	CCDirector::sharedDirector()->pause();
}

void InGamePauseLayer::resumeTouched(CCObject* pSender) {
	CCDirector::sharedDirector()->resume();
	BW_Time::startCachedMilliseconds();
	Model::getInstance()->setGamePaused(false);
	this->removeFromParentAndCleanup(true);
}
