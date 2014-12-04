/*
 * Model.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "Attacks/Attack.h"
#include "Units/Unit.h"
#include "Army.h"
#include <list>
#include "../View/Layers/InGameLayer/InGameGameLayer.h"
#include "GameMode.h"
#include "../Tools/GameStatistics.h"
#include "Difficulty.h"

class Model {
public:
	Model();
	virtual ~Model();
	void initializePlayer();
	void initializeGameField();
	void hideHealthBars();
	static Model* getInstance();
	static void reset();
	float getFieldWidth();
	float getFieldHeigth();
	void addToRemoveUnit(Unit* unit);
	void addToRemoveProjectile(Projectile* projectile);
	Army* getMyArmy(ArmyType myArmyType);
	Army* getEnemyArmy(ArmyType myArmyType);
	void update();
	InGameGameLayer* getInGameGameLayer();
	void setInGameGameLayer(InGameGameLayer* inGameGameLayer);
	Player* getThisPlayer();
	void setThisPlayer(Player* player);
	float getGameSpeed();
	float getGUIElementScaleMultiplicator();

	void increasegameSpeed();
	void decreasegameSpeed();
	void setGameSpeed(float gameSpeed);

	ArmyType getMiddleControl();

	GameMode getGameMode();
	void setGameMode(GameMode gameMode);

	double getSpawnSleep();

	float getMaxZoom();
	float getMinZoom();

	void updateLastSpawn();
	void initLastSpawn();
	double getNextSpawn();
	double getYourNextSpawn();

	void setPlayer1RaceId(int id);
	void setPlayer2RaceId(int id);

	bool isTutorialFinished() const {
		return _tutorialFinished;
	}

	void setTutorialFinished(bool tutorialFinished) {
		_tutorialFinished = tutorialFinished;
	}

	bool isGamePaused() const {
		return _gamePaused;
	}

	void setGamePaused(bool gamePaused) {
		_gamePaused = gamePaused;
	}

	ArmyType getAutoCamTarget() {
		return _autoCamTarget;
	}

	void setAutoCamTarget(ArmyType army) {
		_autoCamTarget = army;
	}

	GameMode getStartedAs() {
		return _startedAs;
	}

	void setDifficulty(Difficulty difficulty) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey("gameDifficulty", difficulty);
		CCUserDefault::sharedUserDefault()->flush();
		_difficulty = difficulty;
	}

	Difficulty getDifficulty() {
		return _difficulty;
	}

	void setShowHealthBar(bool show) {
		_showhealthBar = show;
	}

	bool isShowHealthbar() {
		return _showhealthBar;
	}

	void addAddedUnit(Unit* unit) {
		_addedUnits.push_back(unit);
	}

	void processAddedUnits();

private:
	std::list<Unit*> _addedUnits;

	bool _gamePaused;

	GameMode _startedAs;
	ArmyType _autoCamTarget;
	int _player1race;
	int _player2race;

	float _GUIElementScaleMultiplicator;

	void updateMiddleControl();

	std::list<Unit*> _toRemoveUnit;
	std::list<Projectile*> _toRemoveProjectile;

	InGameGameLayer* _inGameGameLayer;

	Army* _leftArmy;
	Army* _rightArmy;
	ArmyType _middleControl;

	float _fieldWidth;
	float _fieldHeigth;
	static Model* _instance;
	void removeTheUndead();
	float _minZoom;
	float _maxZoom;

	Player* _thisPlayer;
	float _gameSpeed;
	double _spawnSleep;
	GameMode _gameMode;
	double _lastSpawn;

	bool _showhealthBar;

	Difficulty _difficulty;

	bool _tutorialFinished;
};

#endif /* MODEL_H_ */
