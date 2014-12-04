/*
 * BW_Animation.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef BW_ANIMATION_H_
#define BW_ANIMATION_H_

#include "cocos2d.h"
#include "../../Tools/BW_Point.h"

class BW_Animation: public cocos2d::CCNode {
public:
	BW_Animation();
	virtual ~BW_Animation();
	void animateAt(cocos2d::CCNode* parent, BW_Point position, float height, bool randomRotation);
protected:
	void initAnimation();
	void addAnimationPicture(std::string image);
	virtual void fillAnimationPictures() = 0;
	float _animationDelay;
	float _opacity;
	std::string _animationSound;
private:
	void animationEnd(cocos2d::CCNode * sender);
	std::list<std::string> _animationImages;
	cocos2d::CCFiniteTimeAction * _animationAction;
	cocos2d::CCSprite* _sprite;
};

#endif /* ANIMATION_H_ */

/**
 *
 CCSpriteFrame *frame;
 CCAnimation *animation = CCAnimation::create();
 char str[100] = { 0 };
 for (int i = 1; i <= 6; ++i) {
 sprintf(str, "fieryBall%d.png", i);
 frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
 str);
 animation->addSpriteFrame(frame);
 }

 animation->setDelayPerUnit(0.1);
 CCAnimate *animate = CCAnimate::create(animation);
 cocos2d::CCAction* sprite_animation = CCRepeatForever::create(animate);
 */
