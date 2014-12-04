/*
 * BW_Animation.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "BW_Animation.h"
#include "../SpriteTags.h"
#include "../../Tools/BW_Time.h"
#include "../../Tools/Tools.h"
#include "../../Tools/ViewTools.h"
#include "../../Model/Model.h"
#include "../../Sound/SoundHandler.h"
using namespace cocos2d;

BW_Animation::BW_Animation() {
	_animationDelay = 0.1;
	_sprite = NULL;
	_opacity = 255; //MAX
	_animationSound = "";
}

BW_Animation::~BW_Animation() {
}

void BW_Animation::addAnimationPicture(std::string image) {
	_animationImages.push_back(image);
}
void BW_Animation::animateAt(CCNode* parent, BW_Point position, float height, bool randomRotation = false) {
	initAnimation();

	_sprite = CCSprite::createWithSpriteFrameName(_animationImages.front().c_str());
	_sprite->setOpacity(_opacity);
	_sprite->setTag(NOUNIT);
	_sprite->setScale(height / _sprite->getContentSize().height);
	parent->addChild(_sprite, 9999999);
	_sprite->setPosition(ccp(position.x, position.y));

	if (randomRotation) {
		_sprite->setRotation(Tools::random(0, 360));
	}

	CCCallFuncN* f = CCCallFuncN::create(this, callfuncN_selector(BW_Animation::animationEnd));

	_sprite->runAction(CCSequence::create(_animationAction, f, NULL));
	if(_animationSound != "" && ViewTools::isGameFieldSpriteOnScreen(_sprite))
		SoundHandler::getInstance()->playEffect(_animationSound);

}
void BW_Animation::animationEnd(CCNode * sender) {
	if (_sprite != NULL)
		_sprite->removeFromParentAndCleanup(true);
}

void BW_Animation::initAnimation() {
	// init attack animation
	CCSpriteFrame *frame;
	CCAnimation *animation = CCAnimation::create();
	for (std::list<std::string>::iterator it = _animationImages.begin(); it != _animationImages.end(); ++it) {
		frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((*it).c_str());

		animation->addSpriteFrame(frame);
	}

	animation->setDelayPerUnit(_animationDelay / Model::getInstance()->getGameSpeed());
	_animationAction = CCAnimate::create(animation);
}
