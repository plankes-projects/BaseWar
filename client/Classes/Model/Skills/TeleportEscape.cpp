/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "TeleportEscape.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../../View/Animations/SmokeBomb.h"

TeleportEscape::~TeleportEscape() {
}

std::string TeleportEscape::getSkillInfo() {
	return "Teleport escape. (At: " + Tools::toString((int) (_onPercentLife * 100)) + "% HP; ";
}

TeleportEscape::TeleportEscape(float manaCost, float rechargeTime, float range, float onPercentLife) :
		Skill(manaCost, rechargeTime) {
	_range = range;
	_onPercentLife = onPercentLife;
}

Skill* TeleportEscape::clone() {
	return new TeleportEscape(_manaCost, _rechargeTime, _range, _onPercentLife);
}

bool TeleportEscape::perform(Unit* owner) {

	//launching at x% and if we are in combat
	if (owner->getHitpoints() <= 0 || owner->getHitpoints() / owner->getMaxHitpoints() > _onPercentLife || !owner->isAttacked())
		return false;

	float smokeBombHeight = owner->getContentSize().getHeight();
	float smokeBombYDiff = (owner->getContentSize().getHeight() - smokeBombHeight) / 2;
	BW_Point smokeBombPos = owner->getPositionBW();
	smokeBombPos.y -= smokeBombYDiff;
	(new SmokeBomb())->animateAt(owner->getParent(), smokeBombPos, smokeBombHeight, false);

	//teleport unit back
	BW_Point newPos = owner->getPositionBW();
	float intConvert = 10000;
	float y = _range / 2 * intConvert;
	y = Tools::random(-y, y) / intConvert;

	newPos.y += y;

	//calculate a^2+b^2 = c^2
	float x = (_range * _range) - (y * y);
	if (x < 0)
		x *= -1;

	if (owner->getLookingTo() == LEFT)
		newPos.x += sqrt(x);
	else
		newPos.x -= sqrt(x);

	owner->setPosition(newPos);

	smokeBombPos = newPos;
	smokeBombPos.y -= smokeBombYDiff;
	(new SmokeBomb())->animateAt(owner->getParent(), smokeBombPos, smokeBombHeight, false);

	return true;
}
