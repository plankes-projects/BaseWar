/*
 * NormalArmour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef NormalArmour_H_
#define NormalArmour_H_

#include "../DamageType.h"
#include "Armour.h"

class NormalArmour: public Armour {
public:
	NormalArmour();
	virtual ~NormalArmour();
	virtual float realHeal(float heal, DamageType healType);
	virtual std::string getArmourInfo();

	virtual Armour* clone();
};

#endif /* NORMALARMOUR_H_ */
