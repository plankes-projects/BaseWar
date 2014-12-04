/*
 * InGameSpawnMenu.h
 *
 *  Created on: 03.06.2013
 *      Author: Planke
 */

#ifndef INGAMESPAWNINFO_H_
#define INGAMESPAWNINFO_H_

#include "cocos2d.h"
using namespace cocos2d;

class InGameSpawnInfo: public cocos2d::CCLayerColor {
public:
	virtual bool init();
	CREATE_FUNC (InGameSpawnInfo);
private:
	void update(float dt);
	CCLabelTTF* _timeOverLabel;
	CCLabelTTF* _timeSpawnLabel;
};

#endif /* INGAMESPAWNINFO_H_ */
