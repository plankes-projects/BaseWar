/*
 * Point.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "BW_Point.h"

BW_Point::BW_Point() {
	this->x = 0;
	this->y = 0;
}

BW_Point::BW_Point(float x, float y) {
	this->x = x;
	this->y = y;
}

BW_Point::BW_Point(cocos2d::CCPoint p) {
	this->x = p.x;
	this->y = p.y;
}

BW_Point::~BW_Point() {
}

float BW_Point::distanceTo(BW_Point other) {
	return sqrt( (other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y) );
}

bool BW_Point::operator==(BW_Point const& other) {
	return (this->x == other.x && this->y == other.y) ? true : false;
}
