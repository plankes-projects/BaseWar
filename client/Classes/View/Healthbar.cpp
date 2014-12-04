/*
 * BW_Animation.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "Healthbar.h"
using namespace cocos2d;

bool Healthbar::init() {
	if (!CCLayer::init())
		return false;

	_hpColorMaxGreen = 255;
	_hpColorMaxRed = 255;
	_hpColorBlue = 0;
	_hpColorAlpha = 255;

	_hb_width = _mb_width = 100;
	_hb_height = 6;
	_mb_height = _hb_height / 2;

	_lifeDuration_width = _hb_width;
	_lifeDuration_height = 3;

	_barPadding = 0;

	_foreGroundBoarderSpace = 1;
	_hb_foreground = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("healthbar1.png"));
	_hb_foreground->setScaleX((_hb_width + _foreGroundBoarderSpace) / _hb_foreground->getContentSize().width);
	_hb_foreground->setScaleY((_hb_height + _foreGroundBoarderSpace) / _hb_foreground->getContentSize().height);
	_hb_foreground->setOpacity(150);

	_hb_health = CCLayerColor::create(ccc4(0, _hpColorMaxGreen, _hpColorBlue, _hpColorAlpha));
	_hb_health->setContentSize(CCSize(_hb_width, _hb_height));

	_lifeduration = CCLayerColor::create(ccc4(50, 50, 50, _hpColorAlpha));
	_lifeduration->setContentSize(CCSize(_lifeDuration_width, _lifeDuration_height));

	_orig_health_content_width = _hb_health->getContentSize().width;
	_orig_health_content_height = _hb_health->getContentSize().height;

	_orig_lifeDuration_content_width = _lifeduration->getContentSize().width;
	_orig_lifeDuration_content_height = _lifeduration->getContentSize().height;

	//positioning the bars
	_hp_y_pos = _lifeDuration_height / 2 + _hb_height / 2 + _barPadding;
	_hb_health->setPosition(CCPoint(-_hb_health->getContentSize().width / 2, _hp_y_pos - _hb_health->getContentSize().height / 2));
	_hb_foreground->setPosition(CCPoint(0, _hp_y_pos));
	_lifeduration->setPosition(CCPoint(-_lifeduration->getContentSize().width / 2, -_lifeduration->getContentSize().height / 2));

	addChild(_hb_health);
	addChild(_hb_foreground);

	addChild(_lifeduration);
	setContentSize(CCSize(0, 0));

	_mb_mana = NULL;

	return true;
}

void Healthbar::setArmyType(ArmyType armyType) {

	if (armyType == LEFT) {
		_hb_health->setColor(ccc3(50, 255, _hpColorBlue));
	} else {

		_hb_health->setColor(ccc3(255, 50, _hpColorBlue));
	}
}

void Healthbar::addManaBar() {
	if (_mb_mana != NULL)
		return;

	cocos2d::CCSprite* mana_foreground = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("healthbar1.png"));
	mana_foreground->setScaleX((_mb_width + _foreGroundBoarderSpace) / mana_foreground->getContentSize().width);
	mana_foreground->setScaleY((_mb_height + _foreGroundBoarderSpace) / mana_foreground->getContentSize().height);
	mana_foreground->setOpacity(150);

	_hp_y_pos = _lifeDuration_height / 2 + _hb_height / 2 + _mb_height + 2 * _barPadding;
	_mana_y_pos = _lifeDuration_height / 2 + _mb_height / 2 + _barPadding;

	mana_foreground->setPosition(CCPoint(0, _mana_y_pos));

	_mb_mana = CCLayerColor::create(ccc4(0, 0, 255, _hpColorAlpha));
	_mb_mana->setContentSize(CCSize(_mb_width, _mb_height));
	_mb_mana->setPosition(CCPoint(-_mb_mana->getContentSize().width / 2, _mana_y_pos - _mb_mana->getContentSize().height / 2));

	_orig_mana_content_width = _mb_mana->getContentSize().width;
	_orig_mana_content_height = _mb_mana->getContentSize().height;

	addChild(_mb_mana);
	addChild(mana_foreground);

	//repositioning hp bar
	_hb_health->setPosition(CCPoint(-_hb_health->getContentSize().width / 2, _hp_y_pos - _hb_health->getContentSize().height / 2));
	_hb_foreground->setPosition(CCPoint(0, _hp_y_pos));
}

void Healthbar::addChampionMark() {
	cocos2d::CCSprite* championMark = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("championMark.png"));
	float scale = getHeigth() / championMark->getContentSize().height * 1.5;
	championMark->setScale(scale);
	championMark->setPosition(CCPoint(-_hb_width / 2, getHeigth() / 2));
	addChild(championMark);
}

void Healthbar::addMasterMark() {
	cocos2d::CCSprite* masterMark = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("masterMark.png"));
	float scale = getHeigth() / masterMark->getContentSize().height * 1.5;
	;
	masterMark->setScale(scale);
	masterMark->setPosition(CCPoint(-_hb_width / 2, getHeigth() / 2));
	addChild(masterMark);
}

void Healthbar::updateHealthIndicator(float percent) {
	if (percent > 1)
		percent = 1;
	else if (percent < 0)
		percent = 0;

	float newLength = _orig_health_content_width * percent;
	float newXPos = -(_orig_health_content_width - newLength) / 2.0f;
	_hb_health->setContentSize(CCSize(newLength, _hb_height));
	_hb_health->setPosition(CCPoint(newXPos - _hb_health->getContentSize().width / 2, _hp_y_pos - _hb_health->getContentSize().height / 2));

	/*
	 int red = _hpColorMaxRed * (1 - percent);
	 int green = _hpColorMaxGreen * percent;

	 _hb_health->setColor(ccc3(red, green, _hpColorBlue));
	 */
}

void Healthbar::updateManaIndicator(float percent) {
	if (_mb_mana == NULL)
		return;

	if (percent > 1)
		percent = 1;
	else if (percent < 0)
		percent = 0;

	float newLength = _orig_mana_content_width * percent;
	float newXPos = -(_orig_mana_content_width - newLength) / 2.0f;
	_mb_mana->setContentSize(CCSize(newLength, _mb_height));
	_mb_mana->setPosition(CCPoint(newXPos - _mb_mana->getContentSize().width / 2, _mana_y_pos - _mb_mana->getContentSize().height / 2));
}

void Healthbar::updateLifeDurationIndicator(float percent) {
	if (percent > 1)
		percent = 1;
	else if (percent < 0)
		percent = 0;

	float newLength = _orig_lifeDuration_content_width * percent;
	float newXPos = -(_orig_lifeDuration_content_width - newLength) / 2.0f;
	_lifeduration->setContentSize(CCSize(newLength, _lifeDuration_height));
	_lifeduration->setPosition(CCPoint(newXPos - _lifeduration->getContentSize().width / 2, -_lifeduration->getContentSize().height / 2));
}

float Healthbar::getHeigth() {
	return _hb_height + _barPadding + _lifeDuration_height + (_mb_mana == NULL ? 0 : _mb_height);
}
