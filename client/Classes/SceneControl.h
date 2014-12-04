/*
 * SceneControl.h
 *
 *  Created on: May 17, 2013
 *      Author: planke
 */

#ifndef SCENECONTROL_H_
#define SCENECONTROL_H_

#include "cocos2d.h"
#include "Model/GameMode.h"
#include "Model/ArmyType.h"

class SceneControl {
public:
	static cocos2d::CCScene* gameScene(float gameSpeed, GameMode mode);
	static cocos2d::CCScene* raceScene(GameMode mode, std::string name, ArmyType player);
	static cocos2d::CCScene* startMenuScene();
	static cocos2d::CCScene* loadingSpritesScene();
	static cocos2d::CCScene* achievementScene();
	static cocos2d::CCScene* statsScene();
	static cocos2d::CCScene* feedbackScene();
	static cocos2d::CCScene* queueScene(int raceId, std::string playerName);
	static cocos2d::CCScene* gameOverScene(bool won);

	static void replaceScene(cocos2d::CCScene* newScene, bool forward);

private:
	SceneControl();
	virtual ~SceneControl();
};

#endif /* SCENECONTROL_H_ */
