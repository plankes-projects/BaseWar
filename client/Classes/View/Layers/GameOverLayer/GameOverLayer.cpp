#include "GameOverLayer.h"
#include "../../../SceneControl.h"
#include "../../../Model/Model.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool GameOverLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 0, 0, 0)))
		return false;

	this->setTouchEnabled(true);

	return true;
}

void GameOverLayer::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}


void GameOverLayer::setWon(bool won){
	const char* image = won ? "you_win.png" : "game_over_page.png";

	cocos2d::CCSprite* backgroundArt = CCSprite::createWithSpriteFrame(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							image));

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float bgScale = winSize.width/backgroundArt->getContentSize().width;
	backgroundArt->setScale(bgScale);

	float diff = winSize.height - backgroundArt->getContentSize().height * bgScale;

	float x = backgroundArt->getContentSize().width / 2 * bgScale;
	float y = backgroundArt->getContentSize().height / 2 * bgScale + diff/2;
	backgroundArt->setPosition(ccp(x, y));

	addChild(backgroundArt);
}
