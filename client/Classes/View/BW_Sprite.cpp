/*
 * BW_Sprite.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "BW_Sprite.h"
#include <math.h>
#include "../Tools/Trigonometric.h"
#include "../Model/Model.h"
#include "../Tools/BW_Rect.h"
#include "../Tools/BW_Point.h"
#include "../Tools/Tools.h"
#include "SpriteTags.h"

BW_Sprite::BW_Sprite(std::string image, float height, bool initLable, bool fadeIn) {

	_doNotKillSprite = false;
	_isUnit = false;
	_redColorCounter = 0;
	_greenColorCounter = 0;
	_blueColorCounter = 0;
	_currentColor = ccc3(255, 255, 255);

	_numMoveAnimationPictures = 0;
	_numIdleAnimationPictures = 0;
	_numAttackAnimationPicture = 0;
	_animationDoDmgPicture = 0;

	this->_sprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(image.c_str()));
	_currentScale = height / getContentSize().getHeight();
	_sprite->retain();
	this->_sprite->setScale(_currentScale);
	//standard value if not replaced
	_hitBox = BW_Rect(getContentSize().getWidth() * _currentScale, getContentSize().getHeight() * _currentScale);
	this->_image = image;
	this->_height = height;
	this->_lookingTo = RIGHT;
	if (initLable) {
		_currentLabelScale = 0.8 / _currentScale;
		_labelHitpoints = Healthbar::create();
		_labelHitpoints->setScale(_currentLabelScale);
		_labelHitpoints->setPosition(CCPoint(_sprite->getContentSize().width / 2, _sprite->getContentSize().height + _labelHitpoints->getHeigth()));
		_sprite->addChild(_labelHitpoints);
	} else {
		this->_labelHitpoints = NULL;
		_currentLabelScale = 1;
	}
	_sprite->setTag(HASACTION); //we set the tag to detect stopped actions
	if (fadeIn)
		_sprite->runAction(CCFadeIn::create(0.1));

	_movingTo = BW_Point(-124.123, -124.123); //just set a random point
	//we will never use in the game
	_moveAnimationSpeed = 0.2; //standard values, should be overwritten at fillMoveAnimationPictures
	_moveAnimationSpeedUsed = 0.2; //standard values, should be overwritten at fillMoveAnimationPictures
	_idleAnimationSpeed = 0.2; //standard values, should be overwritten at fillIdleAnimationPictures
	_attackAnimationSpeed = 0.2; //standard values, should be overwritten at fillAttackAnimationPictures

	//speed = 2.8f / 14.0f This animation contains 14 frames, will continuous 2.8 seconds.
}
void BW_Sprite::doNotKillSpriteAndSlowIdle() {
	_doNotKillSprite = true;
	_idleAnimationSpeed *= 1.5;
}

void BW_Sprite::showHealthBar(bool show) {
	if (_labelHitpoints == NULL)
		return;
	_labelHitpoints->setVisible(show);
}

void BW_Sprite::addManaBarToHealthBar() {
	if (_labelHitpoints == NULL)
		return;
	_labelHitpoints->addManaBar();
}

void BW_Sprite::addChampionMark() {
	_labelHitpoints->addChampionMark();
}

void BW_Sprite::setHealthPointIndicatorArmyType(ArmyType armyType) {
	_labelHitpoints->setArmyType(armyType);
}

void BW_Sprite::addMasterMark() {
	_labelHitpoints->addMasterMark();
}

void BW_Sprite::updateColor() {

	ccColor3B newColor;
	if (_redColorCounter == 0 && _greenColorCounter == 0 && _blueColorCounter == 0) {
		newColor = ccc3(255, 255, 255);
	} else {
		int r = _redColorCounter > 0 ? 255 : 0;
		int g = _greenColorCounter > 0 ? 255 : 0;
		int b = _blueColorCounter > 0 ? 255 : 0;
		newColor = ccc3(r, g, b);
	}

	if (newColor.r != _currentColor.r || newColor.g != _currentColor.g || newColor.b != _currentColor.b) {
		_currentColor = newColor;
		_sprite->runAction(CCTintTo::create(0.3, _currentColor.r, _currentColor.g, _currentColor.b));
	}
}

void BW_Sprite::addRedColor() {
	_redColorCounter++;
	updateColor();
}
void BW_Sprite::removeRedColor() {
	_redColorCounter--;
	updateColor();
}
void BW_Sprite::addGreenColor() {
	_greenColorCounter++;
	updateColor();
}
void BW_Sprite::removeGreenColor() {
	_greenColorCounter--;
	updateColor();
}
void BW_Sprite::addBlueColor() {
	_blueColorCounter++;
	updateColor();
}
void BW_Sprite::removeBlueColor() {
	_blueColorCounter--;
	updateColor();
}

void BW_Sprite::increaseSizeBy(float percent) {
	_currentScale *= 1 + percent;
	_sprite->runAction(CCScaleTo::create(0.3, _currentScale));

	if (this->_labelHitpoints != NULL) {
		//label needs to hold the same size all the time
		_currentLabelScale /= 1 + percent;
		_labelHitpoints->runAction(CCScaleTo::create(0.3, _currentLabelScale));
	}
}
void BW_Sprite::decreaseSizeBy(float percent) {
	_currentScale /= 1 + percent;
	_sprite->runAction(CCScaleTo::create(0.3, _currentScale));

	if (this->_labelHitpoints != NULL) {
		//label needs to hold the same size all the time
		_currentLabelScale *= 1 + percent;
		_labelHitpoints->runAction(CCScaleTo::create(0.3, _currentLabelScale));
	}
}

cocos2d::CCNode* BW_Sprite::getNode() {
	return _sprite;
}

cocos2d::CCNode* BW_Sprite::getParent() {
	return _sprite->getParent();
}

void BW_Sprite::addMoveAnimationPicture(std::string image) {
	_moveAnimationImages.push_back(image);
	_numMoveAnimationPictures++;
}
void BW_Sprite::addIdleAnimationPicture(std::string image) {
	_idleAnimationImages.push_back(image);
	_numIdleAnimationPictures++;
}
void BW_Sprite::addAttackAnimationPicture(std::string image) {
	_attackAnimationImages.push_back(image);
	_numAttackAnimationPicture++;
}

void BW_Sprite::setOpacity(float op) {
	_sprite->setOpacity(op);
}

void BW_Sprite::initAnimations() {
	fillIdleAnimationPictures();
	fillMoveAnimationPictures();
	fillAttackAnimationPictures();

	initAnimationVars();

	_currentAnimation = SPRITEIMAGE;
}

void BW_Sprite::updateAttackAnimationSpeed(float percentOfAttackLoop) {
	if (_currentAnimation == ATTACK)
		setAnimation(ATTACK, true, percentOfAttackLoop);
}

void BW_Sprite::updateMoveAnimationSpeed() {
	if (_currentAnimation == MOVE)
		setAnimation(MOVE, true);
}

void BW_Sprite::initAttackAnimation(float percent) {
	if (percent > 1) {
		percent = 0;
	}
	// init attack animation
	CCSpriteFrame *frame;
	cocos2d::CCAnimation* attackAnimation = CCAnimation::create();
	int cut = _attackAnimationImages.size() * percent;

	for (std::vector<std::string>::iterator it = _attackAnimationImages.begin() + cut; it != _attackAnimationImages.end(); ++it) {
		frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((*it).c_str());
		attackAnimation->addSpriteFrame(frame);
	}

	for (std::vector<std::string>::iterator it = _attackAnimationImages.begin(); it != _attackAnimationImages.begin() + cut; ++it) {
		frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((*it).c_str());
		attackAnimation->addSpriteFrame(frame);
	}

	attackAnimation->setDelayPerUnit(_attackAnimationSpeed / Model::getInstance()->getGameSpeed());
	CCAnimate *animate = CCAnimate::create(attackAnimation);
	_attackAnimationAction = CCRepeatForever::create(animate);
}

void BW_Sprite::initIdleAnimation() {
	// init move animation

	CCSpriteFrame *frame;
	CCAnimation *animation = CCAnimation::create();
	for (std::vector<std::string>::iterator it = _idleAnimationImages.begin(); it != _idleAnimationImages.end(); ++it) {
		frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((*it).c_str());

		animation->addSpriteFrame(frame);
	}

	animation->setDelayPerUnit(_idleAnimationSpeed / Model::getInstance()->getGameSpeed());
	CCAnimate *animate = CCAnimate::create(animation);
	_idleAnimationAction = CCRepeatForever::create(animate);
}

void BW_Sprite::initMoveAnimation() {
	CCSpriteFrame *frame;
	cocos2d::CCAnimation* moveAnimation = CCAnimation::create();
	for (std::vector<std::string>::iterator it = _moveAnimationImages.begin(); it != _moveAnimationImages.end(); ++it) {
		frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((*it).c_str());

		moveAnimation->addSpriteFrame(frame);
	}

	moveAnimation->setDelayPerUnit(_moveAnimationSpeedUsed / Model::getInstance()->getGameSpeed());
	CCAnimate *animate = CCAnimate::create(moveAnimation);
	_moveAnimationAction = CCRepeatForever::create(animate);
}

/**
 * we cant reuse CCActions, this maybe comes in new versions of cocos2dx
 */
void BW_Sprite::setAnimation(SpriteAnimationType animationType, bool forceUpdate, float percent) {
	if (!forceUpdate && _currentAnimation == animationType)
		return;

	switch (_currentAnimation) {
	case SPRITEIMAGE:
		break;
	case MOVE:
		_sprite->stopAction(_moveAnimationAction);
		break;
	case IDLE:
		_sprite->stopAction(_idleAnimationAction);
		break;
	case ATTACK:
		_sprite->stopAction(_attackAnimationAction);
		break;
	default:
		CCLOG("Bug in Sprite! Error 123");
	}

	_currentAnimation = animationType;

	switch (_currentAnimation) {
	case SPRITEIMAGE:
		break;
	case MOVE:
		initMoveAnimation();
		_sprite->runAction(_moveAnimationAction);
		break;
	case IDLE:
		initIdleAnimation();
		_sprite->runAction(_idleAnimationAction);
		break;
	case ATTACK:
		initAttackAnimation(percent);
		_sprite->runAction(_attackAnimationAction);
		break;
	default:
		CCLOG("Bug in Sprite! Error 124");
	}
}

BW_Sprite::~BW_Sprite() {

	if (_doNotKillSprite) {
		_sprite->release();
		return;
	}
	_sprite->stopAllActions();
	CCActionInterval* removeAction = CCFadeOut::create(0.25);
	CCSequence* seq = CCSequence::create(removeAction, CCCallFunc::create(_sprite, callfunc_selector(CCSprite::removeFromParent)), NULL);
	_sprite->runAction(seq);
	_sprite->release();

	//healthbar removes itself
	//no need to delete _labelHitpoints
	//  its an CCObject and is added to the sprite.
	//  if sprite is deleted, this is also deleted
}

void BW_Sprite::updateLifeDurationIndicator(float percent) {
	if (_labelHitpoints == NULL)
		return;

	this->_labelHitpoints->updateLifeDurationIndicator(percent);
}

void BW_Sprite::updateManaLabel(float percent) {
	if (_labelHitpoints == NULL)
		return;

	_labelHitpoints->updateManaIndicator(percent);
}

void BW_Sprite::updateHitpointLabel(float percent) {
	if (_labelHitpoints == NULL)
		return;

	_labelHitpoints->updateHealthIndicator(percent);
}

void BW_Sprite::updateMovePosition(float dt) {
	if (_currentAnimation != MOVE)
		return;

	BW_Point currentPosition = getPositionBW();

	float moveDistance = _speed * dt * 0.85; //multiply by 0.85 to achieve the same speed as the old move to version
	float maxDist = getPositionBW().distanceTo(_movingTo);
	BW_Point newPos;
	if (moveDistance >= maxDist) {
		newPos = _movingTo;
		spriteMoveFinished(_sprite);
	} else {
		float mult = moveDistance / maxDist;

		float x = _movingTo.x - currentPosition.x;
		float y = _movingTo.y - currentPosition.y;

		float xDelta = x * mult;
		float yDelta = y * mult;
		newPos = BW_Point(getPositionBW().x + xDelta, getPositionBW().y + yDelta);
	}

	if (!_isUnit) {
		//on projectiles there is no walk collition
		setPosition(newPos);
		return;
	}

	//we are a unit here, consider walk collition

	//get nearest unit to this point
	std::list<Unit*> allUnits = Model::getInstance()->getMyArmy(LEFT)->getUnits();
	std::list<Unit*> rightUnits = Model::getInstance()->getMyArmy(RIGHT)->getUnits();
	allUnits.insert(allUnits.end(), rightUnits.begin(), rightUnits.end());

	//get nearest position of other units
	BW_Point nearest;
	float minDist = -1;
	for (std::list<Unit*>::iterator it = allUnits.begin(); it != allUnits.end(); ++it) {
		if ((*it) == this)
			continue;

		BW_Point currentPos = (*it)->getPositionBW();
		float currentDist = newPos.distanceTo(currentPos);
		if (minDist == -1 || currentDist < minDist) {
			minDist = currentDist;
			nearest = currentPos;
		}
	}

	//check nearest position
	float minDistanceToOtherUnits = 50;
	if (minDist == -1 || minDist > minDistanceToOtherUnits) {
		//no problem
		setPosition(newPos);
		return;
	}

	//calculate intersections
	BW_Point i1, i2;
	int res = Trigonometric::intersectionsCircleCircle(getPositionBW(), moveDistance, nearest, minDistanceToOtherUnits, &i1, &i2);

	/**
	 * Returns 0 if intersections
	 * Returns -1 if same m
	 * Returns -2 if no intersections
	 */

	if (res == 0) {
		//intersections found
		newPos = _movingTo.distanceTo(i1) < _movingTo.distanceTo(i2) ? i1 : i2;
	} else if (res == -3 || res == -1) {
		//circle includes another one, move away
		BW_Point directionPoint;
		if (res == -3) {
			directionPoint.x = currentPosition.x - (nearest.x - currentPosition.x);
			directionPoint.y = currentPosition.y - (nearest.y - currentPosition.y);
		} else {
			//same points, move away in random direction
			//to be easy, just move in run direction
			directionPoint.x = Tools::random(0, Model::getInstance()->getFieldWidth());
			directionPoint.y = Tools::random(0, Model::getInstance()->getFieldHeigth());
		}
		// now scale this to moveDistance
		float scale = moveDistance / directionPoint.distanceTo(currentPosition);
		float distX = currentPosition.x - directionPoint.x;
		float distY = currentPosition.y - directionPoint.y;

		newPos.x -= distX * scale;
		newPos.y -= distY * scale;
	}

	//if (res == -2) no problem
	setPosition(newPos);

}

void BW_Sprite::setPosition(BW_Point p) {
	this->_sprite->setPosition(ccp(p.x, p.y));
}
BW_Point BW_Sprite::getPositionBW() {
	return BW_Point(this->_sprite->getPosition().x, this->_sprite->getPosition().y);
}

float BW_Sprite::distanceTo(BW_Sprite* other) {
	return this->getPositionBW().distanceTo(other->getPositionBW());
}

std::string BW_Sprite::getImage() {
	return this->_image;
}

void BW_Sprite::addToDrawLayer() {
	Model::getInstance()->getInGameGameLayer()->addChild(_sprite);
}

BW_Rect BW_Sprite::getRectangular() {
	float leftDownX = this->getPositionBW().x - (this->getContentSize().getWidth() / 2.0);
	float leftDownY = this->getPositionBW().y - (this->getContentSize().getHeight() / 2.0f);
	return BW_Rect(BW_Point(leftDownX, leftDownY), BW_Point(leftDownX + this->getContentSize().getWidth(), leftDownY + this->getContentSize().getHeight()));
}

bool BW_Sprite::intersects(BW_Sprite* other) {
	return this->getRectangular().intersect(other->getRectangular());
}

bool BW_Sprite::hits(BW_Sprite* other) {
	other->_hitBox.setPosition(other->getPositionBW());
	return this->getRectangular().intersect(other->_hitBox);
}

void BW_Sprite::doIdle() {
	setAnimation(IDLE);
}

void BW_Sprite::spriteMoveTo(BW_Point location, float speed, bool setAngle) {
	_movingTo = location;
	setLookingTo(_movingTo, setAngle);

	setAnimation(MOVE);
	_speed = speed * Model::getInstance()->getGameSpeed();
}

void BW_Sprite::spriteMoveFinished(CCNode* sender) {
	sender->setTag(ACTIONSTOPPED); //we set the tag to detect stopped actions
}

bool BW_Sprite::isActionsFinished() {
	return _sprite->getTag() == ACTIONSTOPPED ? true : false;
}

void BW_Sprite::setLookingTo(ArmyType armyType) {
	if (armyType == LEFT) {
		_lookingTo = LEFT;
		_sprite->setFlipX(true);
	} else {
		_lookingTo = RIGHT;
		_sprite->setFlipX(false);
	}
}

void BW_Sprite::setLookingTo(BW_Point to, bool setAngle) {

	//change direction (we always start with a picture looking to the right)
	ArmyType lookingTo = (this->getPositionBW().x < to.x) ? RIGHT : LEFT;
	if (this->_lookingTo != lookingTo) {
		setLookingTo(lookingTo);
	}

	if (setAngle) {
		//rotate the projectile
		//alpha = arcsin(a/H)
		//H = distance
		//a = p2.y - p1.y
		float degree = Trigonometric::toDegree(asin((to.y - this->getPositionBW().y) / this->getPositionBW().distanceTo(to)));

		if (this->getPositionBW().x > to.x) {
			this->_sprite->setRotation(degree);
		} else
			this->_sprite->setRotation(-degree);
	}
}

void BW_Sprite::updateVisibleOfHealthBar() {
	if (_labelHitpoints != NULL && Model::getInstance()->isShowHealthbar() != _labelHitpoints->isVisible())
		_labelHitpoints->setVisible(!_labelHitpoints->isVisible());
}

BW_Size BW_Sprite::getContentSize() {
	return BW_Size(this->_sprite->getContentSize() * this->_sprite->getScale());
}
