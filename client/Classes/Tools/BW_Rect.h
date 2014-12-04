/*
 * Rect.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef RECT_H_
#define RECT_H_

#include "BW_Point.h"

class BW_Rect {
public:
	BW_Rect();
	BW_Rect(BW_Point leftDown, BW_Point rightUp);
	BW_Rect(BW_Point leftDown, float width, float height);
	BW_Rect(float width, float height);
	BW_Rect(float x, float y, float width, float height);
	virtual ~BW_Rect();
	bool intersect(BW_Rect other);
	bool contains(BW_Point p);
	BW_Point leftDown;
	BW_Point rightUp;
	float getWidth();
	float getHeight();
	void setPosition(BW_Point newPosition);
	BW_Point getPositionBW();
};

#endif /* RECT_H_ */
