/*
 * NormalArmour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef LifeRegArmour_H_
#define LifeRegArmour_H_

#include "../DamageType.h"
#include "Armour.h"

class LifeRegArmour: public Armour {
public:
	LifeRegArmour(int hitPointsPerSeconds);
	virtual ~LifeRegArmour();
	virtual float realHeal(float heal, DamageType healType);
	virtual std::string getArmourInfo();
	virtual void update(Unit* owner);


	virtual Armour* clone();
private:
	int _hitPointsPerSeconds;
	double _lastUpdate;
};

#endif /* NORMALARMOUR_H_ */
