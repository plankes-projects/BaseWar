/*
 * DamageTypes.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef DAMAGETYPE_H_
#define DAMAGETYPE_H_

enum DamageType {
	//if you add something here, add it at Tools::toString too
	//NORMAL must be first!!
	//CHAOS must be last!!
	NORMAL, MAGIC, PIERCING, CHAOS
};


#endif /* DAMAGETYPE_H_ */
