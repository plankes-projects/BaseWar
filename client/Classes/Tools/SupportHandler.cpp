/*
 * Trigonometric.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "SupportHandler.h"

SupportHandler::SupportHandler() {

}

SupportHandler::~SupportHandler() {

}

bool SupportHandler::forceVeryBigUI() {
#ifdef LINUX
	return true;
#else
	return false;
#endif
}

bool SupportHandler::forceHighRes() {
#ifdef LINUX
	return true;
#else
	return false;
#endif
}

bool SupportHandler::isEditBoxSupported() {
#ifdef LINUX
	return false;
#else
	return true;
#endif
}
