/*
 * BW_Sprite.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef BW_SPRITE_H_
#define BW_SPRITE_H_

#include "../Tools/BW_Point.h"
#include "../Model/ArmyType.h"
#include "../Tools/BW_Size.h"
#include "../Tools/BW_Rect.h"
#include "SpriteAnimationType.h"
#include "Healthbar.h"

#include "cocos2d.h"
using namespace cocos2d;

class BW_Sprite {
public:
	BW_Sprite(std::string image, float height, bool initLable, bool fadeIn = true);
	virtual ~BW_Sprite();
	void showHealthBar(bool show);
	void setPosition(BW_Point p);
	BW_Point getPositionBW();
	float distanceTo(BW_Sprite* other);
	std::string getImage();
	BW_Size getContentSize();
	void addToDrawLayer();
	BW_Rect getRectangular();
	bool intersects(BW_Sprite* other);
	bool isActionsFinished();
	bool hits(BW_Sprite* other);
	// 0- total transparent, 255 not
	void setOpacity(float op);
	cocos2d::CCNode* getParent();
	cocos2d::CCNode* getNode();
	void setLookingTo(ArmyType armyType);
	void setLookingTo(BW_Point to, bool setAngle);
	ArmyType getLookingTo() {
		return _lookingTo;
	}

	/*
	 * 0..1 = 0..100%
	 */
	void increaseSizeBy(float percent);
	void decreaseSizeBy(float percent);

	/**
	 * we just remove a color again if the counter is at 0
	 * we only use 3 colors, because we do not want to mix color values
	 * eg:we only have to take care to hold blue and green if we remove red
	 */
	void addRedColor();
	void removeRedColor();
	void addGreenColor();
	void removeGreenColor();
	void addBlueColor();
	void removeBlueColor();
	void doIdle();
	void doNotKillSpriteAndSlowIdle();
	void updateMovePosition(float dt);

protected:
	void addChampionMark();
	void addMasterMark();

	void setHealthPointIndicatorArmyType(ArmyType armyType);
	void updateLifeDurationIndicator(float percent);

	void updateHitpointLabel(float percent);
	void updateManaLabel(float percent);
	void updateAttackAnimationSpeed(float percentOfAttackLoop = 0);
	void updateMoveAnimationSpeed();

	void spriteMoveTo(BW_Point location, float speed, bool setAngel);

	virtual void fillMoveAnimationPictures() = 0;
	virtual void fillIdleAnimationPictures() = 0;
	virtual void fillAttackAnimationPictures() = 0;

	virtual void initAnimationVars() = 0;

	void addMoveAnimationPicture(std::string image);
	void addIdleAnimationPicture(std::string image);
	void addAttackAnimationPicture(std::string image);
	void initAnimations();

	void setAnimation(SpriteAnimationType animationType, bool forceUpdate = false, float percent = 0);

	int _numMoveAnimationPictures;
	int _numIdleAnimationPictures;
	int _numAttackAnimationPicture;

	float _moveAnimationSpeed;
	float _moveAnimationSpeedUsed;
	float _idleAnimationSpeed;
	float _attackAnimationSpeed;
	SpriteAnimationType _currentAnimation;

	int _animationDoDmgPicture;
	BW_Rect _hitBox;

	std::string _image;
	float _height;

	cocos2d::CCSprite* _sprite;
	void addManaBarToHealthBar();
	void updateVisibleOfHealthBar();

	bool _isUnit;
private:
	bool _doNotKillSprite;
	float _currentLabelScale;
	float _currentScale;
	void updateColor();
	int _redColorCounter;
	int _greenColorCounter;
	int _blueColorCounter;
	ccColor3B _currentColor;
	void initMoveAnimation();
	void initIdleAnimation();
	void initAttackAnimation(float percent);

	std::vector<std::string> _moveAnimationImages;
	std::vector<std::string> _idleAnimationImages;
	std::vector<std::string> _attackAnimationImages;

	Healthbar* _labelHitpoints;
	ArmyType _lookingTo;
	void spriteMoveFinished(cocos2d::CCNode* sender);
	void updatePositionFromLable();
	BW_Point _movingTo;

	cocos2d::CCAction * _moveAnimationAction;
	cocos2d::CCAction * _idleAnimationAction;
	cocos2d::CCAction * _attackAnimationAction;

	float _speed;
};

#endif /* SPRITE_H_ */
