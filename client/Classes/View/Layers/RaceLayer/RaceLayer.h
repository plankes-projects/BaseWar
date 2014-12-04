#ifndef __RaceLayerLAYER_H__
#define __RaceLayerLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../../Model/GameMode.h"
#include "../../../Model/ArmyType.h"
#include "../../../Model/Difficulty.h"
USING_NS_CC_EXT;

class RaceLayer: public cocos2d::CCLayerColor {
public:
	static RaceLayer* create(GameMode mode, std::string name, ArmyType player);
	virtual bool init(GameMode mode, std::string name, ArmyType player);

	// a selector callback
	void backTouched(cocos2d::CCObject* pSender);
	void startTouched(cocos2d::CCObject* pSender);
	void setText();
	void setTutorialInfo();

	void addComputerDifficultyIfNeeded();
	// implement the "static node()" method manually

private:
	void start(int raceId);
	cocos2d::CCMenuItemSprite * _start;
	GameMode _mode;
	ArmyType _player;
	std::string _playerName;
	cocos2d::CCLabelTTF* _information;
	int _raceId;

	cocos2d::CCMenuItemFont *_difficulty;
	void toggleDifficulty(cocos2d::CCObject* pSender);

	std::string _difficultyPre;
	cocos2d::ccColor3B getDifficultyColor(Difficulty difficulty);

	float _menuPadding;
};

#endif // __STARTMENULAYER_H__
