#include "StartMenuLayer.h"

using namespace cocos2d;
#include "../../../Model/Model.h"
#include "../../../SceneControl.h"
#include "../../../Constants.h"
#include "../../../Tools/SupportHandler.h"
#include "../../CCConfirmView.h"
#include "../../CCAlertView.h"
#include "../../../Tools/Tools.h"
#include "../../../Debug/BalanceSingleton.h"
#include "../../../AchievementSystem/AchievementSystem.h"
#include "../../../AchievementSystem/AchievementFeedback.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Sound/SoundHandler.h"

// on "init" you need to initialize your instance
bool StartMenuLayer::init() {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	if (!CCLayerColor::initWithColor(ccc4(100, 0, 0, 0)))
		return false;

	SoundHandler::getInstance()->playBackground("sound_menu1");
	float button_length = 66 * scaleMult;
	float button_heigth = 66 * scaleMult;
	float padding = 10 * scaleMult;

	_soundOnString = "on";
	_soundOffString = "off";

	//setting background
	ViewTools::setStandardBackgroundImage(this);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemSprite *startOneVsComp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pVsAI0.png"),
			CCSprite::createWithSpriteFrameName("pVsAI1.png"), (CCObject*) this, menu_selector(StartMenuLayer::oneVsCompTouched));
	startOneVsComp->setScaleX(button_length / startOneVsComp->getContentSize().width);
	startOneVsComp->setScaleY(button_heigth / startOneVsComp->getContentSize().height);

	CCMenuItemSprite *startOneVsOne = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pPvP0.png"),
			CCSprite::createWithSpriteFrameName("pPvP1.png"), (CCObject*) this, menu_selector(StartMenuLayer::oneVsOneTouched));
	startOneVsOne->setScaleX(button_length / startOneVsOne->getContentSize().width);
	startOneVsOne->setScaleY(button_heigth / startOneVsOne->getContentSize().height);

	CCMenuItemSprite *startOneVsOneX3 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pInternet0.png"),
			CCSprite::createWithSpriteFrameName("pInternet1.png"), (CCObject*) this, menu_selector(StartMenuLayer::networkGameTouched));
	startOneVsOneX3->setScaleX(button_length / startOneVsOneX3->getContentSize().width);
	startOneVsOneX3->setScaleY(button_heigth / startOneVsOneX3->getContentSize().height);

	CCMenuItemSprite *achievement = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("achievementButton1.png"),
			CCSprite::createWithSpriteFrameName("achievementButton2.png"), (CCObject*) this, menu_selector(StartMenuLayer::achievementTouched));
	achievement->setScaleX(button_length / achievement->getContentSize().width);
	achievement->setScaleY(button_heigth / achievement->getContentSize().height);

	CCMenuItemSprite *stats = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pStats0.png"), CCSprite::createWithSpriteFrameName("pStats1.png"),
			(CCObject*) this, menu_selector(StartMenuLayer::statsTouched));
	stats->setScaleX(button_length / stats->getContentSize().width);
	stats->setScaleY(button_heigth / stats->getContentSize().height);

	CCMenuItemSprite *feedback = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pFeedback0.png"),
			CCSprite::createWithSpriteFrameName("pFeedback1.png"), (CCObject*) this, menu_selector(StartMenuLayer::feedbackTouched));
	feedback->setScaleX(button_length / feedback->getContentSize().width);
	feedback->setScaleY(button_heigth / feedback->getContentSize().height);

#ifdef DEBUG
	CCMenuItemSprite *balancegame = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pStats0.png"),
			CCSprite::createWithSpriteFrameName("pStats1.png"), (CCObject*) this, menu_selector(StartMenuLayer::startBalanceGameTouched));
	balancegame->setScaleX(button_length / balancegame->getContentSize().width);
	balancegame->setScaleY(button_heigth / balancegame->getContentSize().height);

	this->schedule(schedule_selector(StartMenuLayer::startNextRoundIfNeeded), 1);
#endif

	CCMenu* menu = CCMenu::create();
	menu->addChild(startOneVsComp, 1);
	menu->addChild(startOneVsOne, 1);
	menu->addChild(startOneVsOneX3, 1);
	menu->addChild(stats, 1);
	menu->addChild(achievement, 1);
	menu->addChild(feedback, 1);

#ifdef DEBUG
	menu->addChild(balancegame, 1);
#endif

	menu->alignItemsHorizontallyWithPadding(padding);
	addChild(menu);

	CCMenu* shutdownmenu = CCMenu::create();
	CCMenuItemSprite *shutdown = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("shutdown0.png"),
			CCSprite::createWithSpriteFrameName("shutdown1.png"), (CCObject*) this, menu_selector(StartMenuLayer::shutdownTouched));
	shutdown->setScaleX(button_length / shutdown->getContentSize().width);
	shutdown->setScaleY(button_heigth / shutdown->getContentSize().height);
	shutdownmenu->addChild(shutdown);

	CCMenuItemSprite *enableSound = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("soundEnable0.png"),
			CCSprite::createWithSpriteFrameName("soundEnable1.png"), (CCObject*) this, menu_selector(StartMenuLayer::enableSoundTouched));
	enableSound->setScaleX(button_length / enableSound->getContentSize().width);
	enableSound->setScaleY(button_heigth / enableSound->getContentSize().height);

	float scale = button_heigth / enableSound->getContentSize().height;
	_enableSoundLabel = CCLabelTTF::create(SoundHandler::getInstance()->isSoundEnabled() ? _soundOnString.c_str() : _soundOffString.c_str(), FONT_NAME,
			PRICE_LABEL_FONT_SIZE / scale * scaleMult, CCSize(enableSound->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult),
			kCCTextAlignmentCenter);
	enableSound->addChild(_enableSoundLabel);
	_enableSoundLabel->setColor(SoundHandler::getInstance()->isSoundEnabled() ? ccc3(0, 255, 0) : ccc3(255, 0, 0));
	_enableSoundLabel->setPosition(CCPoint(enableSound->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_enableSoundLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);
	shutdownmenu->addChild(enableSound);

	shutdownmenu->alignItemsVerticallyWithPadding(padding);
	addChild(shutdownmenu);
	shutdownmenu->setPosition(CCPoint(winSize.width - button_length / 2, winSize.height - (button_heigth * 2.0f + padding) / 2));

	CCMenuItemFont* tutorial = CCMenuItemFont::create("Tutorial", this, menu_selector(StartMenuLayer::tutorialTouched));
	tutorial->setFontSizeObj(28 * scaleMult);
	tutorial->setFontName(FONT_NAME);
	tutorial->setColor(ccc3(0, 0, 255)); //red
	menu = CCMenu::create(tutorial, NULL);
	addChild(menu);
	menu->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 - (button_heigth / 1.5 + padding)));

	if (SupportHandler::isEditBoxSupported()) {
		CCScale9Sprite* tmp = CCScale9Sprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("inputText.png"));

		_playerNameTextBox = CCEditBox::create(CCSize(300, 60), tmp);

		_playerNameTextBox->setPosition(ccp(winSize.width / 2, winSize.height / 2 - (tutorial->getContentSize().height + button_heigth / 1.5 + 3 * padding)));
		_playerNameTextBox->setFontColor(ccWHITE);
		_playerNameTextBox->setPlaceHolder("Name");

		std::string name = CCUserDefault::sharedUserDefault()->getStringForKey("playername", "");
		_playerNameTextBox->setText(name.c_str());
		_playerNameTextBox->setMaxLength(10);

		_playerNameTextBox->setReturnType(kKeyboardReturnTypeDone);
		_playerNameTextBox->setInputMode(kEditBoxInputModeSingleLine);

		_playerNameTextBox->setScale(scaleMult);
		//m_pEditName->setDelegate(this);
		addChild(_playerNameTextBox);
	}

	//init message
	float label_font_size = 25 * scaleMult;
	const char* label_font = FONT_NAME;
	ccColor3B color = ccc3(0, 0, 0);

	float offsetDown = ViewTools::addVersionAndCreditInfo(this, scaleMult);

	ViewTools::addIdleUnitTo(this, offsetDown);

	//build it once to update the achievements.
	// we make it here because this is the central place where we will land after doing anything
	AchievementSystem a = AchievementSystem();
	float achHeight = 0;
	std::vector<Achievement*> newAch = a.getAchievements();
	if (newAch.size() != 0 && newAch.front()->isAchieved()) {
		CCLayerColor* layer = ViewTools::createLayerFromAchievement(newAch.front());
		achHeight = layer->getContentSize().height;
		addChild(layer);
		layer->setPosition(CCPoint(winSize.width / 2 - layer->getContentSize().width / 2, winSize.height / 2 + button_heigth / 2 + padding));
	}

	std::string s = "Newest achievement:";
#ifdef DEBUG
	s = "!!! DEBUG BUILD - DO NOT RELEASE THIS !!!\n" + s;
#endif

	cocos2d::CCLabelTTF* information = CCLabelTTF::create(s.c_str(), label_font, label_font_size);
	information->setColor(color);
	information->setPosition(
			CCPoint(winSize.width / 2, information->getContentSize().height / 2 + achHeight + winSize.height / 2 + button_heigth / 2 + 2 * padding));
	this->addChild(information);

	int numTutorialUsed = CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorialShowed", 0);
	if (numTutorialUsed == 0) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorialShowed", 1);
		CCUserDefault::sharedUserDefault()->flush();
		CCConfirmView *alert = CCConfirmView::create("Tutorial", "Check out the tutorial", "Start", "Skip", this,
				callfuncO_selector(StartMenuLayer::tutorialTouched), callfuncO_selector(StartMenuLayer::voidTouched));
		addChild(alert, 100);
	} else {
		//show give feedback
		std::string lastFeedBackForVersion = CCUserDefault::sharedUserDefault()->getStringForKey(LAST_FEEDBACK_FOR_VERSION, "");
		if (lastFeedBackForVersion != VERSION) {
			//check time diff
			std::string key = std::string("feedbackShowTresh") + Tools::toString(SHOW_GIVE_FEEDBACK_EVERY_SECONDS);
			int secondsPlayed = GameStatistics().getSecondsPlayed();
			int tresh = secondsPlayed / SHOW_GIVE_FEEDBACK_EVERY_SECONDS;
			int showTresh = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str(), 0);

			if (showTresh < tresh) {
				CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), tresh);
				CCUserDefault::sharedUserDefault()->flush();
				int points = (new AchievementFeedback())->getPoints();
				std::string q = std::string("Want to get ") + Tools::toString(points) + " points?\nGive some feedback.";

				CCConfirmView *alert = CCConfirmView::create("Feedback", q.c_str(), "Now", "Later", this,
						callfuncO_selector(StartMenuLayer::feedbackTouched), callfuncO_selector(StartMenuLayer::voidTouched));
				addChild(alert, 100);
			}
		}
	}

	return true;
}
void StartMenuLayer::tutorialTouched(cocos2d::CCObject* pSender) {
	if (SupportHandler::isEditBoxSupported()) {
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", _playerNameTextBox->getText());
		CCUserDefault::sharedUserDefault()->flush();
	}

	SceneControl::replaceScene(SceneControl::raceScene(TUTORIAL, "Tut", LEFT), true);
}

void StartMenuLayer::shutdownTouched(cocos2d::CCObject* pSender) {
	if (SupportHandler::isEditBoxSupported()) {
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", _playerNameTextBox->getText());
		CCUserDefault::sharedUserDefault()->flush();
	}
	CCUserDefault::sharedUserDefault()->flush();
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void StartMenuLayer::voidTouched(cocos2d::CCObject* pSender) {

}

void StartMenuLayer::networkGameTouched(cocos2d::CCObject* pSender) {
	if (AchievementSystem().getPoints() < NEEDED_POINTS_FOR_NETWORK_GAME) {
		addChild(ViewTools::createNotUnlockedMessage(NEEDED_POINTS_FOR_NETWORK_GAME), 100);
		return;
	}

	std::string name = "";
	if (SupportHandler::isEditBoxSupported()) {
		name = _playerNameTextBox->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", name);
		CCUserDefault::sharedUserDefault()->flush();
	} else {
		//wrote the playername in another way... proj.linux/main.cpp
		name = CCUserDefault::sharedUserDefault()->getStringForKey("playername", "");
	}
	if (name == "")
		name = "Player";

	//we are not allowed to use ; in a networkgame name because the server splits the string by ;
	for (unsigned int i = 0; i < name.size(); i++) {
		if (name[i] == ';')
			name[i] = '_';
	}

	SceneControl::replaceScene(SceneControl::raceScene(NETWORK, name, LEFT), true);
}

void StartMenuLayer::oneVsOneTouched(cocos2d::CCObject* pSender) {
	std::string name = "";
	if (SupportHandler::isEditBoxSupported()) {
		name = _playerNameTextBox->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", name);
		CCUserDefault::sharedUserDefault()->flush();
	}
	if (name == "")
		name = "Player";

	SceneControl::replaceScene(SceneControl::raceScene(PVP, name, LEFT), true);
}

void StartMenuLayer::oneVsCompTouched(cocos2d::CCObject* pSender) {
	CCAlertView::removeAlertViewIfPresent();

	std::string name = "";
	if (SupportHandler::isEditBoxSupported()) {
		name = _playerNameTextBox->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", name);
		CCUserDefault::sharedUserDefault()->flush();
	}
	if (name == "")
		name = "Player";

	SceneControl::replaceScene(SceneControl::raceScene(PVA, name, LEFT), true);
}

void StartMenuLayer::achievementTouched(cocos2d::CCObject* pSender) {
	if (SupportHandler::isEditBoxSupported()) {
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", _playerNameTextBox->getText());
		CCUserDefault::sharedUserDefault()->flush();
	}
	SceneControl::replaceScene(SceneControl::achievementScene(), true);
}

void StartMenuLayer::statsTouched(cocos2d::CCObject* pSender) {
	if (SupportHandler::isEditBoxSupported()) {
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", _playerNameTextBox->getText());
		CCUserDefault::sharedUserDefault()->flush();
	}
	SceneControl::replaceScene(SceneControl::statsScene(), true);
}

void StartMenuLayer::feedbackTouched(cocos2d::CCObject* pSender) {
	if (SupportHandler::isEditBoxSupported()) {
		CCUserDefault::sharedUserDefault()->setStringForKey("playername", _playerNameTextBox->getText());
		CCUserDefault::sharedUserDefault()->flush();
	}
	SceneControl::replaceScene(SceneControl::feedbackScene(), true);
}

void StartMenuLayer::enableSoundTouched(cocos2d::CCObject* pSender) {
	bool sound = !SoundHandler::getInstance()->isSoundEnabled();
	SoundHandler::getInstance()->enableSound(sound);

	_enableSoundLabel->setColor(sound ? ccc3(0, 255, 0) : ccc3(255, 0, 0));
	_enableSoundLabel->setString(sound ? _soundOnString.c_str() : _soundOffString.c_str());
}

#ifdef DEBUG
void StartMenuLayer::startBalanceGameTouched(cocos2d::CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::raceScene(BALANCE_TEST, "name", LEFT), true);
}

void StartMenuLayer::startNextRoundIfNeeded(float dt) {
	if(BalanceSingleton::getInstance() && BalanceSingleton::getInstance()->isAnotherRound()) {
		Model::getInstance()->reset();
		Model::getInstance()->setPlayer1RaceId(BalanceSingleton::getInstance()->getRaceId1());
		Model::getInstance()->setPlayer2RaceId(BalanceSingleton::getInstance()->getRaceId2());
		SceneControl::replaceScene(SceneControl::gameScene(BALANCE_TEST_GAME_SPEED, BALANCE_TEST), true);
	}
}
#endif
