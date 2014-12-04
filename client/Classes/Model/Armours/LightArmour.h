/*
 * NormalArmour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef LightArmour_H_
#define LightArmour_H_

#include "../DamageType.h"
#include "Armour.h"

class LightArmour: public Armour {
public:
	LightArmour();
	virtual ~LightArmour();
	virtual float realHeal(float heal, DamageType healType);
	virtual std::string getArmourInfo();

	virtual Armour* clone();
};

#endif /* LightArmour_H_ */
