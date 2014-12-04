/*
 * Trigonometric.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "Trigonometric.h"

Trigonometric::Trigonometric() {
}

Trigonometric::~Trigonometric() {
}

float Trigonometric::toDegree(float radiant) {
	return radiant * 180 / 3.14159265;
}

/**
 * Returns 0 if intersections
 * Returns -1 if same m
 * Returns -2 if no intersections, circles out of range
 * Returns -3 if no intersection, circles includes another one
 */
int Trigonometric::intersectionsCircleCircle(BW_Point m1, float r1, BW_Point m2, float r2, BW_Point* i1, BW_Point* i2) {
	if (m1 == m2)
		return -1; // same point, no intersections

	float d, a, h, dx, dy;

	d = m1.distanceTo(m2);

	if (d > r1 + r2)
		return (-2); //no intersections

	dx = m2.x - m1.x;
	dy = m2.y - m1.y;

	a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);

	h = sqrt(r1 * r1 - a * a);

	i1->x = m1.x + (a / d) * dx - (h / d) * dy;
	i1->y = m1.y + (a / d) * dy + (h / d) * dx;

	i2->x = m1.x + (a / d) * dx + (h / d) * dy;
	i2->y = m1.y + (a / d) * dy - (h / d) * dx;

	//Returns -3 if no intersection, circles includes another one
	if(isnan(i1->x) || isnan(i1->y) || isnan(i2->x) || isnan(i2->y))
		return -3;

	return (0);
}
