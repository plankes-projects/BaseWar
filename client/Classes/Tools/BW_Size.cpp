/*
 * Size.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "BW_Size.h"

BW_Size::~BW_Size() {
	// TODO Auto-generated destructor stub
}

BW_Size::BW_Size(float height, float width) {
	this->height = height;
	this->width = width;
}

BW_Size::BW_Size(cocos2d::CCSize s) {
	this->height = s.height;
	this->width = s.width;
}

float BW_Size::getHeight() {
	return height;
}

float BW_Size::getWidth() {
	return width;
}