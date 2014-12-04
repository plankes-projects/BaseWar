/*
 * Trigonometric.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef SupportHandler_H_
#define SupportHandler_H_

class SupportHandler {
public:
	static bool isEditBoxSupported();
	static bool forceHighRes();
	static bool forceVeryBigUI();
private:

	SupportHandler();
	virtual ~SupportHandler();
};

#endif /* SupportHandler_H_ */
