#include "RaceLayer.h"

using namespace cocos2d;
#include "../../../Model/Model.h"
#include "../../../SceneControl.h"
#include "../../../Constants.h"
#include "../../../Tools/Tools.h"
#include "../../../Tools/GameStatistics.h"
#include "../../CCConfirmView.h"
#include "../../CCAlertView.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Debug/BalanceSingleton.h"
#include "../../../AchievementSystem/AchievementSystem.h"

RaceLayer* RaceLayer::create(GameMode mode, std::string name, ArmyType player) {
	RaceLayer *pRet = new RaceLayer();
	if (pRet && pRet->init(mode, name, player)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

// on "init" you need to initialize your instance
bool RaceLayer::init(GameMode mode, std::string name, ArmyType player) {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	if (!CCLayerColor::initWithColor(ccc4(100, 0, 0, 0)))
		return false;

	_player = player;
	_mode = mode;
	_playerName = name;
	_difficultyPre = "";
	_menuPadding = 100;

	float button_length = 66 * scaleMult;
	float button_heigth = 66 * scaleMult;
	float padding = 10 * scaleMult;

	//setting background
	ViewTools::setStandardBackgroundImage(this);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCMenu* raceMenu = CCMenu::create();
	int numRaces = 0;
	int i = 1;
	//ad normal races 1 ... x and race 0 if no race is left
	for (;; i++) {

		int raceId = i;
		Race* race = Race::createRaceWithId(raceId);
		if (race == 0) {
			raceId = 0; //random unit race
			race = Race::createRaceWithId(raceId);
			if (race == 0) {
				CCLOG("[ERROR] BUG FOUND IN RACELAYER.CPP GAME WILL CRASH!");
			}
		}

		CCMenuItemSprite *raceButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(race->getRaceButton0().c_str()),
				CCSprite::createWithSpriteFrameName(race->getRaceButton1().c_str()), (CCObject*) this, menu_selector(RaceLayer::startTouched));
		raceButton->setTag(raceId);

		int pos = raceId - 1;
		if (pos < 0) {
			pos = i - 1;
		}
		ViewTools::addToMenu(raceMenu, raceButton, button_length, button_heigth, pos, 1, padding);
		delete race;
		numRaces++;
		if (raceId == 0)
			break;
	}

	//add special races START_ID_FOR_AI_RACES...x --> AI ONLY
	int numSpecialRaces = 0;
	if (_mode == BALANCE_TEST || (_mode == PVA && _player == RIGHT)) {
		//this is a AI player!
		//add AI only races
		int raceId = START_ID_FOR_AI_RACES;		//AI races start with id START_ID_FOR_AI_RACES
		for (;; raceId++) {
			Race* race = Race::createRaceWithId(raceId);
			if (race == 0) {
				break;
			}

			CCMenuItemSprite *raceButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(race->getRaceButton0().c_str()),
					CCSprite::createWithSpriteFrameName(race->getRaceButton1().c_str()), (CCObject*) this, menu_selector(RaceLayer::startTouched));
			raceButton->setTag(raceId);
			ViewTools::addToMenu(raceMenu, raceButton, button_length, button_heigth, numRaces, 1, padding);
			delete race;
			numRaces++;
			numSpecialRaces++;
		}
	}

	//add random button START----------------
	CCMenuItemSprite *raceButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("random0.png"),
			CCSprite::createWithSpriteFrameName("random1.png"), (CCObject*) this, menu_selector(RaceLayer::startTouched));
	raceButton->setScaleX(button_length / raceButton->getContentSize().width);
	raceButton->setScaleY(button_heigth / raceButton->getContentSize().height);
	int random;
	if (AchievementSystem().getPoints() < NEEDED_POINTS_FOR_SPECIAL_AI_RACES)
		numSpecialRaces = 0;

	if (AchievementSystem().getPoints() < NEEDED_POINTS_FOR_RANDOM_UNIT_RACE) {
		random = Tools::random(1, i - 1 + numSpecialRaces);
	} else {
		random = Tools::random(0, i - 1 + numSpecialRaces); //with random unit race
	}
	//this is a AI only race
	if (random >= i) {
		//they start from id START_ID_FOR_AI_RACES
		random = START_ID_FOR_AI_RACES + random - i;
	}

	raceButton->setTag(random);
	ViewTools::addToMenu(raceMenu, raceButton, button_length, button_heigth, numRaces / 2.0f - (1.0f / 2.0f), 0, padding);
	//add random button END----------------

	ViewTools::resizeMenu(raceMenu, button_length, button_heigth, numRaces, 2, padding, 0);

	raceMenu->setPosition(CCPoint(winSize.width / 2 - raceMenu->getContentSize().width / 2, winSize.height / 2 - raceMenu->getContentSize().height / 2));

	addChild(raceMenu);

	float offsetDown = ViewTools::addVersionAndCreditInfo(this, scaleMult);
	ViewTools::addIdleUnitTo(this, offsetDown);

	//add back button
	CCMenu* backButtonmenu = CCMenu::create();
	CCMenuItemSprite *back = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return0.png"), CCSprite::createWithSpriteFrameName("return1.png"),
			(CCObject*) this, menu_selector(RaceLayer::backTouched));
	back->setScaleX(button_length / back->getContentSize().width);
	back->setScaleY(button_heigth / back->getContentSize().height);
	backButtonmenu->addChild(back);
	addChild(backButtonmenu);
	backButtonmenu->setPosition(CCPoint(winSize.width - button_length / 2, winSize.height - button_heigth / 2));

	setText();
	setTutorialInfo();
	addComputerDifficultyIfNeeded();

	return true;
}

void RaceLayer::addComputerDifficultyIfNeeded() {

	if (_mode != PVA || _player != RIGHT)
		return;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	_difficulty = CCMenuItemFont::create((_difficultyPre + Tools::toString(Model::getInstance()->getDifficulty())).c_str(), this,
			menu_selector(RaceLayer::toggleDifficulty));
	_difficulty->setFontSizeObj(50 * scaleMult);
	_difficulty->setFontName(FONT_NAME);
	_difficulty->setColor(getDifficultyColor(Model::getInstance()->getDifficulty()));

	CCMenu* menu = CCMenu::create();
	menu->addChild(_difficulty);
	menu->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 - _difficulty->getContentSize().height / 2 - _menuPadding * scaleMult));

	addChild(menu);
}

cocos2d::ccColor3B RaceLayer::getDifficultyColor(Difficulty difficulty) {

	switch (difficulty) {
	case DIF_EASY:
		return ccc3(255, 255, 0);
	case DIF_NORMAL:
		return ccc3(0, 255, 0);
	case DIF_HARD:
		return ccc3(255, 100, 0);
	case DIF_VERYHARD:
	case DIF_HELL:
		return ccc3(255, 0, 0);
	default:
		CCLOG("ERROR: INVALID DIFFICULTY @ RaceLayer.cpp!");
	}
	return ccc3(0, 0, 0);
}

void RaceLayer::toggleDifficulty(cocos2d::CCObject* pSender) {
	Difficulty newDif = DIF_EASY;
	switch (Model::getInstance()->getDifficulty()) {
	case DIF_EASY:
		newDif = DIF_NORMAL;
		break;
	case DIF_NORMAL:
		newDif = DIF_HARD;
		break;
	case DIF_HARD:
		newDif = DIF_VERYHARD;
		break;
	case DIF_VERYHARD:
		newDif = DIF_HELL;
		break;
	case DIF_HELL:
		newDif = DIF_EASY;
		break;
	default:
		CCLOG("ERROR: INVALID DIFFICULTY @ RaceLayer.cpp!");
	}

	Model::getInstance()->setDifficulty(newDif);

	_difficulty->setString((_difficultyPre + Tools::toString(newDif)).c_str());
	_difficulty->setColor(getDifficultyColor(newDif));
}

void RaceLayer::setTutorialInfo() {
	if (_mode == TUTORIAL) {
		if (_player == LEFT) {
			std::string content = "The race represents a set of units to fight against your enemy.\n \n";
			content += "The dice button will choose a random race for you.\n \n";
			content += "The button with the arrows will take random units from all available races to generate a mixed race.";

			CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");
			addChild(alert, 100);
		} else {
			std::string content = "Now you have to select a race for your enemy.";

			CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");
			addChild(alert, 100);
		}
	}
}
void RaceLayer::startTouched(cocos2d::CCObject* pSender) {
	cocos2d::CCNode* node = (cocos2d::CCNode*) pSender;

	if (_mode == TUTORIAL && node->getTag() != 1) {
		CCConfirmView *alert = CCConfirmView::create("Not unlocked yet", "Only race 1 is unlocked at the tutorial.", "Ok");
		addChild(alert, 100);
		return;
	}

	if (node->getTag() == 0) {
		//random chaos race
		if (AchievementSystem().getPoints() < NEEDED_POINTS_FOR_RANDOM_UNIT_RACE) {
			addChild(ViewTools::createNotUnlockedMessage(NEEDED_POINTS_FOR_RANDOM_UNIT_RACE), 100);
			return;
		}
	}
	if (node->getTag() >= START_ID_FOR_AI_RACES) {
		//random AI races
		if (AchievementSystem().getPoints() < NEEDED_POINTS_FOR_SPECIAL_AI_RACES) {
			addChild(ViewTools::createNotUnlockedMessage(NEEDED_POINTS_FOR_SPECIAL_AI_RACES), 100);
			return;
		}
	}

	//AI races start with id START_ID_FOR_AI_RACES

	start(node->getTag());
}

void RaceLayer::backTouched(cocos2d::CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}

void RaceLayer::setText() {

	std::string str = "";

	switch (_mode) {
	case PVP:
		if (_player == LEFT) {
			str = "Please select a race for the left player:";
		} else {
			str = "Please select a race for the right player to start:";
		}
		break;
	case BALANCE_TEST:
		if (_player == LEFT) {
			str = "Please select a race for the left computer:";
		} else {
			str = "Please select a race for the right computer:";
		}
		break;
	case PVA:
	case TUTORIAL:
		if (_player == LEFT) {
			str = "Please select your race:";
		} else {
			str = "Please select a race for the computer to start:";
		}
		break;
	case NETWORK:
		str = "Please select your race to start:";
		break;
	default:
		CCLOG("ERROR in RaceLayer. Invalid game mode detected. Error: 30");
	}

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_information = CCLabelTTF::create("", FONT_NAME, 25 * scaleMult);
	_information->setString(str.c_str());
	_information->setColor(ccc3(0, 0, 0));
	_information->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 + _information->getContentSize().height / 2 + _menuPadding * scaleMult));
	addChild(_information);
}

void RaceLayer::start(int raceid) {
	switch (_mode) {
	case PVP:
	case PVA:
	case TUTORIAL:
		if (_player == LEFT) {
			Model::getInstance()->setPlayer1RaceId(raceid);
			SceneControl::replaceScene(SceneControl::raceScene(_mode, _playerName, RIGHT), true);
		} else {
			Model::getInstance()->setPlayer2RaceId(raceid);
			SceneControl::replaceScene(SceneControl::gameScene(1, _mode), true);
		}
		break;
	case BALANCE_TEST:
		if (_player == LEFT) {
			BalanceSingleton::getInstance()->reset();
			BalanceSingleton::getInstance()->setRace1(raceid);
			Model::getInstance()->setPlayer1RaceId(raceid);
			SceneControl::replaceScene(SceneControl::raceScene(_mode, _playerName, RIGHT), true);
		} else {
			Model::getInstance()->setPlayer2RaceId(raceid);
			BalanceSingleton::getInstance()->setRace2(raceid);
			SceneControl::replaceScene(SceneControl::gameScene(BALANCE_TEST_GAME_SPEED, _mode), true);
		}
		break;
	case NETWORK:
		SceneControl::replaceScene(SceneControl::queueScene(raceid, _playerName.c_str()), true);
		break;
	default:
		CCLOG("ERROR in RaceLayer. Invalid game mode detected. Error: 40");
	}

}
