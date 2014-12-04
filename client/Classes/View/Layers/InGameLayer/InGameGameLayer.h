/*
 * InGameGameLayer.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef INGAMEGAMELAYER_H_
#define INGAMEGAMELAYER_H_

#include "../../../Tools/BW_Point.h"
#include "../../../Tools/BW_Rect.h"
#include <vector>

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class InGameGameLayer: public cocos2d::CCLayerColor {
public:
	virtual bool init();
	float setInitialPosition();
	void watchAt(BW_Point point);
	BW_Point watchingAt();
	void setAutoCam(bool autoCam);
	bool getAutoCam();
	CREATE_FUNC (InGameGameLayer);
	~InGameGameLayer();
	BW_Rect getShownRect();
	void zoom(float scale);
	void showPlayerLeftMessage(std::string playerName);
private:
	bool _playersAreForceAI;
	int _sendLogInOrbit;
	int _maxSendLogInOrbit;
	double _lastLogTime;
	cocos2d::CCPoint initialTouch;
	cocos2d::CCPoint initialPos;
	float initialTouchDistance;
	float initialZoom;
	virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
	void onLogCompleted(CCHttpClient *sender, CCHttpResponse *response);
	std::string getSpawnLogData();

	void setPositionAndAvoidLayerToLeaveScreen(float x, float y);
	void update(float dt);
	void networkUpdate(float dt);
	void spriteLocationUpdate(float dt);
	void spawnUnits();
	bool _autoCam;
	bool _autoCamWatchingAtTarget;
	bool _2fingerInit;
	void upgradeBackground();
	void initBackground();
	void performAutocam(float dt);

	std::vector<CCSprite*> _backGroundSprites;
	float _backgroundSpriteMaxWidth;
	float _backgroundSpriteMaxHeight;

	bool _offlineLog;
};

#endif /* INGAMEGAMELAYER_H_ */
