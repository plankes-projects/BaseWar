/*
 * Size.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef SIZE_H_
#define SIZE_H_

#include "cocos2d.h"

class BW_Size {
private:
	float height, width;
public:
	BW_Size() : height(0), width(0) {};
	virtual ~BW_Size();
	
	float getHeight();
	float getWidth();

	BW_Size(float height, float width);
	BW_Size(cocos2d::CCSize s);
};

#endif /* SIZE_H_ */
