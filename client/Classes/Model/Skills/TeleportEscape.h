/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef TeleportEscape_H_
#define TeleportEscape_H_

#include "Skill.h"
class TeleportEscape: public Skill {
public:
	virtual ~TeleportEscape();
	TeleportEscape(float manaCost, float rechargeTime, float range, float onPercentLife);
	Skill* clone();
	virtual std::string getSkillInfo();
protected:
	bool virtual perform(Unit* owner);
private:
	float _range;
	float _onPercentLife;
	void upgradeSkill(){
		_range *= _upgradeMultiplier;
		_onPercentLife *= _upgradeMultiplier;
	}
};

#endif
