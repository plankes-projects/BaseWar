#include "InGameUnitInfo.h"

using namespace cocos2d;
#include "../../../Model/Model.h"
#include "../../../Tools/Tools.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Tools/BW_Rect.h"
#include "../../../Tools/BW_Point.h"
#include "../../CCAlertView.h"
#include "../../SpriteTags.h"
#include "../../../Constants.h"
#include <string>
#include "Tutorial.h"

bool InGameUnitInfo::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 100, 255, 255)))
		return false;
	this->setTouchEnabled(true); // if we touch it, we remove this layer
	//setOpacity(0); // 0- total transparent, 255 not

	return true;
}

InGameUnitInfo::~InGameUnitInfo() {
	if (_unitFactory->getArmyType() == LEFT) {
		InGameUnitInfo::_instanceRemoverLEFT = NULL;
	} else if (_unitFactory->getArmyType() == RIGHT) {
		InGameUnitInfo::_instanceRemoverRIGHT = NULL;
	}
}

InGameUnitInfo* InGameUnitInfo::_instanceRemoverLEFT = NULL;
InGameUnitInfo* InGameUnitInfo::_instanceRemoverRIGHT = NULL;

void InGameUnitInfo::removeOld(ArmyType type) {
	if (type == LEFT && InGameUnitInfo::_instanceRemoverLEFT != NULL) {
		InGameUnitInfo::_instanceRemoverLEFT->removeFromParentAndCleanup(true);
		InGameUnitInfo::_instanceRemoverLEFT = NULL;
	} else if (type == RIGHT && InGameUnitInfo::_instanceRemoverRIGHT != NULL) {
		InGameUnitInfo::_instanceRemoverRIGHT->removeFromParentAndCleanup(true);
		InGameUnitInfo::_instanceRemoverRIGHT = NULL;
	}
	this->removeFromParentAndCleanup(true);
}

void InGameUnitInfo::initButtons(UnitFactory* unitFactory, float button_length, float button_heigth, float padding,
		cocos2d::CCSize parentMenuLayerSize) {

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	const char* label_font = FONT_NAME;
	int x = 0;
	int y = 2;
	CCMenu* menu = CCMenu::create();

	_unitFactory = unitFactory;
	ArmyType type = _unitFactory->getArmyType();
	removeOld(type);
	if (type == LEFT) {
		InGameUnitInfo::_instanceRemoverLEFT = this;
	} else if (type == RIGHT) {
		InGameUnitInfo::_instanceRemoverRIGHT = this;
	}

	// this button shows the user knows which unit he is buying, further, the information dialog is displayed if the user presses the button
	// Unit-Info
	_unit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(unitFactory->getNormalButtonImage()),
			CCSprite::createWithSpriteFrameName(unitFactory->getSelectedButtonImage()), (CCObject*) this,
			menu_selector(InGameUnitInfo::unitInfoTouched));
	float scale = ViewTools::addToMenu(menu, _unit, button_length, button_heigth, x++, y, padding);

	_unitLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_unit->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_unit->addChild(_unitLabel);
	_unitLabel->setColor(PRICE_LABEL_COLOR_INFO);
	_unitLabel->setPosition(CCPoint(_unit->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_unitLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// buy unit
	_add = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("Buy1.png"), CCSprite::createWithSpriteFrameName("Buy2.png"),
			CCSprite::createWithSpriteFrameName("Buy3.png"), (CCObject*) unitFactory, menu_selector(UnitFactory::addOneUnitTransit));
	scale = ViewTools::addToMenu(menu, _add, button_length, button_heigth, x++, y, padding);

	_addLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_add->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_add->addChild(_addLabel);
	_addLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_addLabel->setPosition(CCPoint(_add->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_addLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// sell unit
	_sell = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("Sell1.png"), CCSprite::createWithSpriteFrameName("Sell2.png"),
			CCSprite::createWithSpriteFrameName("Sell3.png"), (CCObject*) unitFactory, menu_selector(UnitFactory::sellOneUnitTransit));
	scale = ViewTools::addToMenu(menu, _sell, button_length, button_heigth, x++, y, padding);

	_sellLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_sell->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_sell->addChild(_sellLabel);
	_sellLabel->setColor(PRICE_LABEL_COLOR_INFO);
	_sellLabel->setPosition(CCPoint(_sell->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_sellLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button--------------------------------------------------------------
	// return
	CCMenuItemSprite *_return = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return0.png"),
			CCSprite::createWithSpriteFrameName("return1.png"), (CCObject*) this, menu_selector(InGameUnitInfo::shutdownTouched));
	_return->setScaleX(button_length / _return->getContentSize().width);
	_return->setScaleY(button_heigth / _return->getContentSize().height);
	scale = ViewTools::addToMenu(menu, _return, button_length, button_heigth, x++, y, padding);

	y--;
	x = 0;

	// new button---------------------------------------------------------------
	// upgrade attack
	if (_unitFactory->getAttack()->isHeal()) {
		_attackup = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("HealUpgrade1.png"),
				CCSprite::createWithSpriteFrameName("HealUpgrade2.png"), CCSprite::createWithSpriteFrameName("HealUpgrade3.png"),
				(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeAttackTransit));
	} else {
		_attackup = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("AttackUpgrade1.png"),
				CCSprite::createWithSpriteFrameName("AttackUpgrade2.png"), CCSprite::createWithSpriteFrameName("AttackUpgrade3.png"),
				(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeAttackTransit));
	}
	scale = ViewTools::addToMenu(menu, _attackup, button_length, button_heigth, x++, y, padding);

	_attackupLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_attackup->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_attackup->addChild(_attackupLabel);
	_attackupLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_attackupLabel->setPosition(CCPoint(_attackup->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_attackupLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade manareg
	_rangeUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("RangeUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("RangeUpgrade2.png"), CCSprite::createWithSpriteFrameName("RangeUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeRangeTransit));
	scale = ViewTools::addToMenu(menu, _rangeUp, button_length, button_heigth, x++, y, padding);

	_rangeLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_rangeUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_rangeUp->addChild(_rangeLabel);
	_rangeLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_rangeLabel->setPosition(CCPoint(_rangeUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_rangeLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade armor
	_armourup = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ArmourUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ArmourUpgrade2.png"), CCSprite::createWithSpriteFrameName("ArmourUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeArmourTransit));
	scale = ViewTools::addToMenu(menu, _armourup, button_length, button_heigth, x++, y, padding);

	_armourupLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_armourup->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_armourup->addChild(_armourupLabel);
	_armourupLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_armourupLabel->setPosition(CCPoint(_armourup->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_armourupLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade health
	_hitPointsUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("HitpointsUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("HitpointsUpgrade2.png"), CCSprite::createWithSpriteFrameName("HitpointsUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeHitpointsTransit));
	scale = ViewTools::addToMenu(menu, _hitPointsUp, button_length, button_heigth, x++, y, padding);

	_hitPointsUpLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_hitPointsUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_hitPointsUp->addChild(_hitPointsUpLabel);
	_hitPointsUpLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_hitPointsUpLabel->setPosition(CCPoint(_hitPointsUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_hitPointsUpLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	y--;
	x = 0;

	// new button---------------------------------------------------------------
	// upgrade skill
	_skillUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("SkillUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("SkillUpgrade2.png"), CCSprite::createWithSpriteFrameName("SkillUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeSkillTransit));
	scale = ViewTools::addToMenu(menu, _skillUp, button_length, button_heigth, x++, y, padding);

	_skillLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_skillUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_skillUp->addChild(_skillLabel);
	_skillLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_skillLabel->setPosition(CCPoint(_skillUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_skillLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade mana
	_manaUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ManaUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ManaUpgrade2.png"), CCSprite::createWithSpriteFrameName("ManaUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeManaTransit));
	scale = ViewTools::addToMenu(menu, _manaUp, button_length, button_heigth, x++, y, padding);

	_manaUpLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_manaUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_manaUp->addChild(_manaUpLabel);
	_manaUpLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_manaUpLabel->setPosition(CCPoint(_manaUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_manaUpLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade manareg
	_manaRegUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ManaRegUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ManaRegUpgrade2.png"), CCSprite::createWithSpriteFrameName("ManaRegUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeManaRegTransit));
	scale = ViewTools::addToMenu(menu, _manaRegUp, button_length, button_heigth, x++, y, padding);

	_manaRegLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_manaRegUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_manaRegUp->addChild(_manaRegLabel);
	_manaRegLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_manaRegLabel->setPosition(CCPoint(_manaRegUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_manaRegLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	// new button---------------------------------------------------------------
	// upgrade speed
	_runSpeedUp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("RunSpeedUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("RunSpeedUpgrade2.png"), CCSprite::createWithSpriteFrameName("RunSpeedUpgrade3.png"),
			(CCObject*) unitFactory, menu_selector(UnitFactory::upgradeRunSpeedTransit));
	scale = ViewTools::addToMenu(menu, _runSpeedUp, button_length, button_heigth, x++, y, padding);

	_runSpeedUpLabel = CCLabelTTF::create("", label_font, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(_runSpeedUp->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	_runSpeedUp->addChild(_runSpeedUpLabel);
	_runSpeedUpLabel->setColor(PRICE_LABEL_COLOR_BUY);
	_runSpeedUpLabel->setPosition(CCPoint(_runSpeedUp->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_runSpeedUpLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	//	menu rows
	ViewTools::resizeMenu(menu, button_length, button_heigth, 4, 2, padding, padding / 2);
	float width = parentMenuLayerSize.width;
	float height = parentMenuLayerSize.height;

	//avoid push throw the buttons to underlying layer
	cocos2d::CCMenuItemSprite* fallbackAndBackground;
	fallbackAndBackground = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("unitInfoBackground.png"),
			CCSprite::createWithSpriteFrameName("unitInfoBackground.png"));
	fallbackAndBackground->setScaleX(width / fallbackAndBackground->getContentSize().width);
	fallbackAndBackground->setScaleY(height / fallbackAndBackground->getContentSize().height);

	CCMenu* menuFallback = CCMenu::create();
	menuFallback->addChild(fallbackAndBackground, 1);
	menuFallback->setPosition(width / 2, height / 2);

	addChild(menuFallback);
	update(1);
	addChild(menu);

	setContentSize(parentMenuLayerSize);
	menu->setPosition(padding, padding);
	setPosition(0, 0);

	this->schedule(schedule_selector(InGameUnitInfo::update), 0.1);
}

void InGameUnitInfo::update(float dt) {
	_unitLabel->setString(Tools::toString(_unitFactory->getSpawnNum()).c_str());

	_add->setEnabled(_unitFactory->isAddAble());
	_addLabel->setString(Tools::toString(_unitFactory->getAddCost()).c_str());

	_sell->setEnabled(_unitFactory->isSellAble());
	_sellLabel->setString(Tools::toString(_unitFactory->getSellPrice()).c_str());

	_attackup->setEnabled(_unitFactory->isAttackUpgradeAble());
	_attackupLabel->setString(Tools::toString(_unitFactory->getAttackUpCost()).c_str());

	_armourup->setEnabled(_unitFactory->isArmourUpgradeAble());
	_armourupLabel->setString(Tools::toString(_unitFactory->getArmourUpCost()).c_str());

	_hitPointsUp->setEnabled(_unitFactory->isHitPointsUpgradeAble());
	_hitPointsUpLabel->setString(Tools::toString(_unitFactory->getHitPointsUpCost()).c_str());

	_runSpeedUp->setEnabled(_unitFactory->isRunSpeedUpgradeAble());
	_runSpeedUpLabel->setString(Tools::toString(_unitFactory->getRunSpeedUpCost()).c_str());

	_skillUp->setEnabled(_unitFactory->isSkillUpgradeAble());
	_skillLabel->setString(Tools::toString(_unitFactory->getSkillUpCost()).c_str());

	_manaUp->setEnabled(_unitFactory->isManaUpgradeAble());
	_manaUpLabel->setString(Tools::toString(_unitFactory->getManaUpCost()).c_str());

	_manaRegUp->setEnabled(_unitFactory->isManaRegUpgradeAble());
	_manaRegLabel->setString(Tools::toString(_unitFactory->getManaRegUpCost()).c_str());

	_rangeUp->setEnabled(_unitFactory->isRangeUpgradeAble());
	_rangeLabel->setString(Tools::toString(_unitFactory->getRangeUpCost()).c_str());
}

void InGameUnitInfo::shutdownTouched(cocos2d::CCObject* pSender) {
	removeOld(_unitFactory->getArmyType());
}

void InGameUnitInfo::unitInfoTouched(cocos2d::CCObject* pSender) {
	ViewTools::showUnitInfo(_unitFactory, getParent()->getParent());
}
