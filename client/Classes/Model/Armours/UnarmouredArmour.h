/*
 * NormalArmour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef UnarmouredArmour_H_
#define UnarmouredArmour_H_

#include "../DamageType.h"
#include "Armour.h"

class UnarmouredArmour: public Armour {
public:
	UnarmouredArmour();
	virtual ~UnarmouredArmour();
	virtual float realHeal(float heal, DamageType healType);
	virtual std::string getArmourInfo();

	virtual Armour* clone();
};

#endif /* UnarmouredArmour_H_ */
