/*
 * LifeLeechAttack.h
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#ifndef SuizideAttack_H_
#define SuizideAttack_H_

#include "Attack.h"

class SuizideAttack: public Attack {
public:
	virtual ~SuizideAttack();
	SuizideAttack(float splashRange, float damage);
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
	float _splashRange;
	int _numberOfHitTargets;
};

#endif /* LIFELEECHATTACK_H_ */
