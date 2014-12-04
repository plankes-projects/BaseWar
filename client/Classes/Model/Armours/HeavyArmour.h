/*
 * NormalArmour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef HeavyArmour_H_
#define HeavyArmour_H_

#include "../DamageType.h"
#include "Armour.h"

class HeavyArmour: public Armour {
public:
	HeavyArmour();
	virtual ~HeavyArmour();
	virtual float realHeal(float heal, DamageType healType);
	virtual std::string getArmourInfo();

	virtual Armour* clone();
};

#endif /* HeavyArmour_H_ */
