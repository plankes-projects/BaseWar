/*
 * InGameBottomMenuLayer.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "InGameBottomMenuLayer.h"
#include "../../../Model/Units/UnitFactory.h"
#include "../../../Model/Model.h"
#include "../../../Model/Units/Tier.h"
#include "../../../Tools/Tools.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Tools/BW_Rect.h"
#include "../../../Tools/BW_Point.h"
#include "../../../Constants.h"
#include "InGameUnitInfo.h"
#include "Tutorial.h"
using namespace cocos2d;

bool InGameBottomMenuLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 255)))
		return false;
	this->setTouchEnabled(true); // if we touch it, we remove this layer

	_unitInfoCalled = false;
	_unitInfoMenuCalled = false;

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	_speed = 600 * scaleMult;
	_button_length = 66 * scaleMult;
	_button_heigth = 66 * scaleMult;
	_padding = 10 * scaleMult;

	Model::getInstance()->getThisPlayer()->getRace();
	_owner = Model::getInstance()->getThisPlayer();

	CCMenu* tierMenu = this->createTierMenu();
	tierMenu->setPosition(CCPoint(_padding, _padding));

	// resizing this layer to the menus
	float width = tierMenu->getContentSize().width;
	float menuHeight = tierMenu->getContentSize().height;
	setContentSize(CCSize(width, menuHeight));

	//add the show menu button
	CCMenu* menuButton = CCMenu::create();
	CCMenuItemSprite* button = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("menu1.png"), CCSprite::createWithSpriteFrameName("menu2.png"),
			(CCObject*) this, menu_selector(InGameBottomMenuLayer::menuTouched));
	button->setScale(66 / button->getContentSize().width * scaleMult);
	_menuButtonWidth = button->getContentSize().width * button->getScale();
	menuButton->addChild(button, 1);

	//set position
	if (_owner->getArmyTyp() == LEFT) {
		menuButton->setPosition(button->getContentSize().width / 2 * button->getScale(), menuHeight + button->getContentSize().height / 2 * button->getScale());
		_shownLocation = ccp(0, 0);
		_hiddenLocation = ccp(0, -menuHeight);
	} else {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		_shownLocation = ccp(winSize.width - width - (UNITS_INFO_OVERLAY_WIDTH * scaleMult), 0);
		_hiddenLocation = ccp(winSize.width - width - (UNITS_INFO_OVERLAY_WIDTH * scaleMult), -menuHeight);

		menuButton->setPosition(width - button->getContentSize().width / 2 * button->getScale(),
				menuHeight + button->getContentSize().height / 2 * button->getScale());
	}

	setPosition(_hiddenLocation);
	_menuIsHidden = true;

	//avoid push throw the buttons to underlying layer
	cocos2d::CCMenuItemSprite* fallbackAndBackground;
	fallbackAndBackground = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("inGameBottomMenuBackground.png"),
			CCSprite::createWithSpriteFrameName("inGameBottomMenuBackground.png"));
	fallbackAndBackground->setScaleX(width / fallbackAndBackground->getContentSize().width);
	fallbackAndBackground->setScaleY(menuHeight / fallbackAndBackground->getContentSize().height);

	CCMenu* menuFallback = CCMenu::create();
	menuFallback->addChild(fallbackAndBackground, 1);
	menuFallback->setPosition(width / 2, menuHeight / 2);

	addChild(menuFallback);

	addChild(tierMenu);
	addChild(menuButton);

	_gold_label = CCLabelTTF::create("", FONT_NAME, 25 * scaleMult);
	_gold_label->setColor(ccc3(255, 255, 255));

	addChild(_gold_label);

	updateGoldLabel(1);

	//setOpacity(0); // 0- total transparent, 255 not
	this->schedule(schedule_selector(InGameBottomMenuLayer::update), 0.1);
	this->schedule(schedule_selector(InGameBottomMenuLayer::updateGoldLabel), 0.25); // every second is enough - TTFlabel upgrade is very slow! (hm, 250ms?)

	return true;
}

CCMenu* InGameBottomMenuLayer::createTierMenu() {
	CCMenu* menu = CCMenu::create();

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	const char* label_font = FONT_NAME;
	ccColor3B color = ccc3(255, 215, 0);
	int maxXAnz = 0;
	float scale = 0;

	for (int tier = Tier1; tier <= Tier3; tier++) {
		int posx = 0;
		int posY = 2;
		if (tier == Tier2)
			posY = 1;
		else if (tier == Tier3)
			posY = 0;

		if (tier == Tier1) {
			//the first button is for income upgrade
			_incomeUpdateButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("IncomeUp1.png"),
					CCSprite::createWithSpriteFrameName("IncomeUp2.png"), CCSprite::createWithSpriteFrameName("IncomeUp3.png"), (CCObject*) _owner,
					menu_selector(Player::upgradeIncomeTransit));
			_incomeUpdateButton->setEnabled(false);

			scale = ViewTools::addToMenu(menu, _incomeUpdateButton, _button_length, _button_heigth, posx++, posY, _padding);

			_incomeUpdateButtonLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
					CCSize(_incomeUpdateButton->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
			_incomeUpdateButton->addChild(_incomeUpdateButtonLabel);
			_incomeUpdateButtonLabel->setColor(color);
			_incomeUpdateButtonLabel->setPosition(CCPoint(_incomeUpdateButton->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));

			_incomeUpdateButtonLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);
		} else if (tier == Tier2) {
			_tier2Button = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pUnlock1.png"), CCSprite::createWithSpriteFrameName("pUnlock2.png"),
					CCSprite::createWithSpriteFrameName("pUnlock3.png"), (CCObject*) this, menu_selector(InGameBottomMenuLayer::updateToTier2));
			_tier2Button->setEnabled(false);

			scale = ViewTools::addToMenu(menu, _tier2Button, _button_length, _button_heigth, posx++, posY, _padding);

			_tier2ButtonLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
					CCSize(_tier2Button->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);

			_tier2Button->addChild(_tier2ButtonLabel);
			_tier2ButtonLabel->setColor(color);
			_tier2ButtonLabel->setPosition(CCPoint(_tier2Button->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
			_tier2ButtonLabel->setString(Tools::toString((int) _owner->getTier2Cost()).c_str());

			_tier2ButtonLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);
		} else {
			_tier3Button = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pUnlock1.png"), CCSprite::createWithSpriteFrameName("pUnlock2.png"),
					CCSprite::createWithSpriteFrameName("pUnlock3.png"), (CCObject*) this, menu_selector(InGameBottomMenuLayer::updateToTier3));
			_tier3Button->setEnabled(false);
			_tier3Button->setVisible(false);

			scale = ViewTools::addToMenu(menu, _tier3Button, _button_length, _button_heigth, posx++, posY, _padding);

			_tier3ButtonLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
					CCSize(_tier3Button->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
			_tier3Button->addChild(_tier3ButtonLabel);
			_tier3ButtonLabel->setColor(color);
			_tier3ButtonLabel->setPosition(CCPoint(_tier3Button->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
			_tier3ButtonLabel->setString(Tools::toString((int) _owner->getTier3Cost()).c_str());
			_tier3ButtonLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);
		}

		CCMenuItemSprite* button;
		UnitFactory* buttonTarget;
		std::vector<UnitFactory*> unitFactories = _owner->getRace()->getUnitFactories((Tier) tier);

		for (std::vector<UnitFactory*>::iterator it = unitFactories.begin(); it != unitFactories.end(); ++it) {
			buttonTarget = (*it);

			button = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(buttonTarget->getNormalButtonImage()),
					CCSprite::createWithSpriteFrameName(buttonTarget->getSelectedButtonImage()),
					CCSprite::createWithSpriteFrameName(buttonTarget->getDisabledButtonImage()), (CCObject*) this,
					menu_selector(InGameBottomMenuLayer::showUnitLayerTouched));

			scale = ViewTools::addToMenu(menu, button, _button_length, _button_heigth, posx++, posY, _padding);

			cocos2d::CCLabelTTF* numLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
					CCSizeMake(button->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
			numLabel->setColor(ccc3(0, 255, 0));
			numLabel->setPosition(CCPoint(button->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
			numLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

			button->addChild(numLabel); //has to have index 0 in node
			button->addChild(buttonTarget); //has to have index 1 in node
			_unitButtonList.push_back(button);

			if (maxXAnz < posx)
				maxXAnz = posx;
		}
	}
	ViewTools::resizeMenu(menu, _button_length, _button_heigth, maxXAnz, 3, _padding, _padding / 2);
	return menu;
}

void InGameBottomMenuLayer::updateToTier2(CCObject* pSender) {
	if (_owner->upgradeToTier2()) {
		//replace this button with upgrade tower
		_tier2Button->setVisible(false);
		_tier3Button->setVisible(true);
	}
}
void InGameBottomMenuLayer::updateToTier3(CCObject* pSender) {
	if (_owner->upgradeToTier3()) {
		//replace this button with upgrade base
		_tier3Button->setVisible(false);
	}
}

void InGameBottomMenuLayer::showUnitLayerTouched(CCObject* pSender) {
	//the factory is the last object
	UnitFactory* fact = (UnitFactory*) ((CCMenuItemSprite*) pSender)->getChildren()->lastObject();
	InGameUnitInfo *layer = InGameUnitInfo::create();
	layer->initButtons(fact, _button_length, _button_heigth, _padding, getContentSize());
	addChild(layer);

	if (!_unitInfoCalled && Model::getInstance()->getGameMode() == TUTORIAL) {
		_unitInfoCalled = true;
		(new Tutorial(getParent()))->unitInfo();
	}
}

void InGameBottomMenuLayer::menuTouched(CCObject* pSender) {
	CCPoint targetLocation = _menuIsHidden ? _shownLocation : _hiddenLocation;
	float distance = BW_Point(this->getPosition()).distanceTo(BW_Point(targetLocation));

	this->stopAllActions();
	this->runAction(CCSequence::create(CCMoveTo::create(distance / _speed, targetLocation), NULL));

	if (!_unitInfoMenuCalled && _menuIsHidden && Model::getInstance()->getGameMode() == TUTORIAL) {
		_unitInfoMenuCalled = true;
		(new Tutorial(getParent()))->unitMenuInfo();
	}

	_menuIsHidden = !_menuIsHidden;
}

void InGameBottomMenuLayer::update(float dt) {
	_incomeUpdateButton->setEnabled(_owner->isIncomeUpgradeAble());
	_incomeUpdateButtonLabel->setString(Tools::toString(_owner->getincomeUpgradeCost()).c_str());

	if (_owner->getCurrentTier() == Tier1) {
		_tier2Button->setEnabled(_owner->isToTier2Able());
		_tier2ButtonLabel->setString(Tools::toString((int) _owner->getTier2Cost()).c_str());
	}
	if (_owner->getCurrentTier() == Tier1 || _owner->getCurrentTier() == Tier2) {
		_tier3Button->setEnabled(_owner->isToTier3Able());
		_tier3ButtonLabel->setString(Tools::toString((int) _owner->getTier3Cost()).c_str());
	}

	for (std::list<CCMenuItemSprite*>::iterator it = _unitButtonList.begin(); it != _unitButtonList.end(); ++it) {

		CCMenuItemSprite* button = (*it);
		CCArray * childs = button->getChildren();
		unsigned int childCount = childs->count();

		cocos2d::CCLabelTTF* numLabel = (cocos2d::CCLabelTTF*) childs->objectAtIndex(childCount - 2);
		UnitFactory* factory = (UnitFactory*) childs->objectAtIndex(childCount - 1);
		bool active = factory->getTier() <= factory->getOwner()->getCurrentTier();

		if (active != button->isEnabled())
			button->setEnabled(active);

		if (active) {
			numLabel->setString(Tools::toString(factory->getSpawnNum()).c_str());
		}
	}
}

void InGameBottomMenuLayer::updateGoldLabel(float dt) {
	float gps = _owner->getGoldPerSeconds();
	_gold_label->setString(("Gold: " + Tools::toString((int) _owner->getGold()) + " (" + Tools::toString(gps, 1) + " g/s)").c_str());
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float padding = 10 * scaleMult;
	float x_pos =
			_owner->getArmyTyp() == LEFT ?
					_menuButtonWidth + padding + _gold_label->getContentSize().width / 2 * _gold_label->getScaleX() :
					getContentSize().width - _gold_label->getContentSize().width * _gold_label->getScaleX() / 2 - _menuButtonWidth - padding;

	_gold_label->setPosition(CCPoint(x_pos, getContentSize().height + _gold_label->getContentSize().height / 2 * _gold_label->getScaleY()));
}
