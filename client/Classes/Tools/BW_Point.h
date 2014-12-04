/*
 * Point.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef POINT_H_
#define POINT_H_

#include "cocos2d.h"

class BW_Point {
public:
	float x, y;
	~BW_Point();
	BW_Point();
	BW_Point(float x, float y);
	BW_Point(cocos2d::CCPoint p);
	float distanceTo(BW_Point other);
	bool operator==(BW_Point const& other);
};

#endif /* POINT_H_ */
