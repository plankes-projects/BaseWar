/*
 * Rect.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "BW_Rect.h"

BW_Rect::BW_Rect() {
	this->leftDown = this->rightUp = BW_Point(0, 0);
}

BW_Rect::BW_Rect(float width, float height) {
	this->leftDown = BW_Point(0, 0);
	this->rightUp = BW_Point(width, height);
}

BW_Rect::BW_Rect(BW_Point leftDown, BW_Point rightUp) {
	this->leftDown = leftDown;
	this->rightUp = rightUp;
}

BW_Rect::BW_Rect(BW_Point leftDown, float width, float height) {
	this->leftDown = leftDown;
	this->rightUp = BW_Point(leftDown.x + width, leftDown.y + height);
}

BW_Rect::BW_Rect(float x, float y, float width, float height) {
	this->leftDown = BW_Point(x, y);
	this->rightUp = BW_Point(leftDown.x + width, leftDown.y + height);
}

float BW_Rect::getWidth() {
	return rightUp.x - leftDown.x;
}
float BW_Rect::getHeight() {
	return rightUp.y - leftDown.y;
}

void BW_Rect::setPosition(BW_Point newPosition) {
	float width = getWidth();
	float height = getHeight();

	leftDown = BW_Point(newPosition.x - width / 2, newPosition.y - height / 2);
	rightUp = BW_Point(leftDown.x + width, leftDown.y + height);
}

BW_Point BW_Rect::getPositionBW() {
	return BW_Point(leftDown.x + getWidth() / 2, leftDown.y + getHeight() / 2);
}

BW_Rect::~BW_Rect() {
	// TODO Auto-generated destructor stub
}

bool BW_Rect::intersect(BW_Rect other) {
	return !(other.leftDown.x > rightUp.x || other.rightUp.x < leftDown.x
			|| other.rightUp.y < leftDown.y || other.leftDown.y > rightUp.y);
}

bool BW_Rect::contains(BW_Point p) {
	float width = getWidth();
	float height = getHeight();
	if (	   p.x < (leftDown.x + width)
			&& p.x > (leftDown.x)
			&& p.y < (leftDown.y + height)
            && p.y > (leftDown.y))
		return true;
	else
		return false;
}
