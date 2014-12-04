#include "AchievementLayer.h"
#include "../../../SceneControl.h"
#include "../../../Model/Model.h"
#include "../../../Constants.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Tools/Tools.h"
#include "../../../AchievementSystem/AchievementSystem.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool AchievementLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 255)))
		return false;
	_achievementAchievementsLayer = NULL;

	//adding the background
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCSprite* backgroundArt = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("menuBackground.png"));

	float bgScale = winSize.height / backgroundArt->getContentSize().height;
	backgroundArt->setScale(bgScale);

	float diff = winSize.width - backgroundArt->getContentSize().width * bgScale;
	float x = backgroundArt->getContentSize().width / 2 * bgScale + diff / 2;
	float y = backgroundArt->getContentSize().height / 2 * bgScale;
	backgroundArt->setPosition(ccp(x, y));
	addChild(backgroundArt);

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	//back button
	int buttonHeight = 66 * scaleMult;
	int buttonLength = 66 * scaleMult;
	CCMenu* backButtonmenu = CCMenu::create();
	CCMenuItemSprite *backButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return0.png"),
			CCSprite::createWithSpriteFrameName("return1.png"), (CCObject*) this, menu_selector(AchievementLayer::backTouched));
	backButton->setScaleX(buttonLength / backButton->getContentSize().width);
	backButton->setScaleY(buttonHeight / backButton->getContentSize().height);
	backButtonmenu->addChild(backButton);

	CCMenuItemSprite *toggleModeButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("toggleGUI1.png"),
			CCSprite::createWithSpriteFrameName("toggleGUI2.png"), (CCObject*) this, menu_selector(AchievementLayer::toggleModeTouched));
	toggleModeButton->setScaleX(buttonLength / toggleModeButton->getContentSize().width);
	toggleModeButton->setScaleY(buttonHeight / toggleModeButton->getContentSize().height);
	backButtonmenu->addChild(toggleModeButton);

	float padding = 10 * scaleMult;
	backButtonmenu->alignItemsVerticallyWithPadding(padding);
	addChild(backButtonmenu);
	backButtonmenu->setPosition(CCPoint(winSize.width - buttonLength / 2, winSize.height - (buttonHeight * 2.0f + padding) / 2));

	float offsetDown = ViewTools::addVersionAndCreditInfo(this, scaleMult);
	ViewTools::addIdleUnitTo(this, offsetDown);

	return true;
}

void AchievementLayer::updateAchievement(AchievementUpdateType type) {
	if (_achievementAchievementsLayer != NULL)
		_achievementAchievementsLayer->removeFromParentAndCleanup(true);

	_type = type;

	std::vector<CCNode*> nodes;
	//we have to create a variable with the AchievementSystem
	//otherwise it will delete the content of the vector
	AchievementSystem achSys = AchievementSystem();
	std::vector<Achievement*> achievements = achSys.getAchievements();

	float achievement_width = 0;
	for (std::vector<Achievement*>::iterator achi = achievements.begin(); achi != achievements.end(); ++achi) {

		if (type == AUT_ACHIEVED && !(*achi)->isAchieved())
			continue;
		else if (type == AUT_NOT_ACHIEVED && (*achi)->isAchieved())
			continue;

		CCLayerColor* ach = ViewTools::createLayerFromAchievement(*achi);
		nodes.push_back(ach);
		if (achievement_width == 0) {
			achievement_width = ach->getContentSize().width;
		}
	}

	float scale = Model::getInstance()->getGUIElementScaleMultiplicator();
	float percent = (float) achSys.getPoints() / (float) achSys.getMaxPoints() * 100;
	std::string title = "Score: " + Tools::toString(achSys.getPoints()) + " (" + Tools::toString((int) percent) + "%)";
	_achievementAchievementsLayer = ScrollLayer::create(title.c_str(), nodes, achievement_width, 10 * scale);
	getParent()->addChild(_achievementAchievementsLayer);
}

void AchievementLayer::toggleModeTouched(cocos2d::CCObject* pSender) {
	AchievementUpdateType newType = AUT_ALL;
	if (_type == AUT_ALL)
		newType = AUT_NOT_ACHIEVED;
	else if (_type == AUT_ACHIEVED)
		newType = AUT_NOT_ACHIEVED;

	updateAchievement(newType);
}

void AchievementLayer::backTouched(CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}
