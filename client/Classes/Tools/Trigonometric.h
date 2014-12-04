/*
 * Trigonometric.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef TRIGONOMETRIC_H_
#define TRIGONOMETRIC_H_

#include "BW_Point.h"

class Trigonometric {
public:
	Trigonometric();
	virtual ~Trigonometric();
	static float toDegree(float radiant);

	/**
	 * Returns 0 if intersections
	 * Returns -1 if same m
	 * Returns -2 if no intersections, circles out of range
	 * Returns -3 if no intersection, circles includes another one
	 */
	static int intersectionsCircleCircle(BW_Point m1, float r1, BW_Point m2, float r2, BW_Point* i1, BW_Point* i2);
};

#endif /* TRIGONOMETRIC_H_ */
