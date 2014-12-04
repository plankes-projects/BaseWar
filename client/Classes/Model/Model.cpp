/*
 * Model.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "Model.h"
#include <cstddef>
#include "../Tools/BW_Point.h"
#include "../Tools/BW_Time.h"
#include "Units/Base.h"
#include "Units/Tower.h"
#include "../Network/NetworkSingleton.h"
#include "../SceneControl.h"
#include "../Tools/SupportHandler.h"

#include "cocos2d.h"	//just for display size
Model::Model() {
	_tutorialFinished = false;
	_thisPlayer = NULL;
	_gamePaused = false;
	_showhealthBar = true;
	_autoCamTarget = LEFT;
	_player1race = 0;
	_player2race = 0;
	BW_Time::resetStopDiff();
	BW_Time::updateCachedMilliseconds();
	BW_Time::resetStartTime(1);

	CCLOG("2");
	_fieldWidth = 10000;
	_fieldHeigth = 3000;
	_spawnSleep = 20;

	_minZoom = cocos2d::CCDirector::sharedDirector()->getWinSize().width / _fieldWidth;

	_maxZoom = _minZoom * 15.0;

	//init spawn point
	_leftArmy = new Army(LEFT, BW_Point(0, _fieldHeigth / 2));
	_rightArmy = new Army(RIGHT, BW_Point(_fieldWidth, _fieldHeigth / 2));
	_gameSpeed = 1; //standard = 1
	_middleControl = NONE;

	_difficulty = (Difficulty) CCUserDefault::sharedUserDefault()->getIntegerForKey("gameDifficulty", DIF_NORMAL);

	float worldWidth = cocos2d::CCDirector::sharedDirector()->getWinSize().width / (float) cocos2d::CCDevice::getDPI();

	//3.55633 inch = sony ericson arc s worldWidth
	if (SupportHandler::forceVeryBigUI()) {
		_GUIElementScaleMultiplicator = (float) cocos2d::CCDevice::getDPI() / 220.0f * 2.5f; //240 is sony ericson arc s dpi, i am developing on
	} else if (worldWidth > 1.5f * 3.55633f) { //same tablet detection used in loading layer!
	//tablet
		_GUIElementScaleMultiplicator = (float) cocos2d::CCDevice::getDPI() / 220.0f * 1.5f; //240 is sony ericson arc s dpi, i am developing on
	} else {
		//phone
		_GUIElementScaleMultiplicator = (float) cocos2d::CCDevice::getDPI() / 220.0f; //240 is sony ericson arc s dpi, i am developing on
	}
}

void Model::setPlayer1RaceId(int race) {
	_player1race = race;
}
void Model::setPlayer2RaceId(int race) {
	_player2race = race;
}

float Model::getGUIElementScaleMultiplicator() {
	return _GUIElementScaleMultiplicator;
}

void Model::updateLastSpawn() {
	//fix iOS bug of multiple spawns after putting the app to background
	while (getNextSpawn() < 0)
		_lastSpawn += _spawnSleep * 1000;
}

void Model::initLastSpawn() {
	_lastSpawn = BW_Time::getMilliSecondsCached();
}

void Model::initializeGameField() {
	Base* base = new Base();
	_leftArmy->addUnit(base);
	base->setPosition(BW_Point(base->getPositionBW().x + base->getContentSize().getWidth() * 2, _fieldHeigth / 2));

	base = new Base();
	_rightArmy->addUnit(base);
	base->setPosition(BW_Point(base->getPositionBW().x - base->getContentSize().getWidth() * 2, _fieldHeigth / 2));
	base->setLookingTo(LEFT);

	float towerLeftX = _fieldWidth / 4; // 25%
	float towerRigthX = _fieldWidth - towerLeftX; // 75%

	Tower* tower = new Tower();
	_leftArmy->addUnit(tower);
	tower->setPosition(BW_Point(towerLeftX, _fieldHeigth / 2));

	tower = new Tower();
	_rightArmy->addUnit(tower);
	tower->setPosition(BW_Point(towerRigthX, _fieldHeigth / 2));
	tower->setLookingTo(LEFT);
}

void Model::initializePlayer() {
	_startedAs = _gameMode;
	GameStatistics gs = GameStatistics();
	if (_gameMode == PVP) {
		_leftArmy->addPlayer(new Player("Left Player", Race::createRaceWithId(_player1race), false, false));
		_rightArmy->addPlayer(new Player("Right Player", Race::createRaceWithId(_player2race), false, false));
		gs.increaseLocalMultiplayerGamesStarted();
	} else if (_gameMode == PVA) {
		_thisPlayer = new Player("Left Player", Race::createRaceWithId(_player1race), false, false);
		_leftArmy->addPlayer(_thisPlayer);

		_rightArmy->addPlayer(new Player("Right Player", Race::createRaceWithId(_player2race), true, false));
		gs.increaseLocalGamesStarted();
	} else if (_gameMode == BALANCE_TEST) {
		_thisPlayer = new Player("Left Player", Race::createRaceWithId(_player1race), true, false);
		_leftArmy->addPlayer(_thisPlayer);

		_rightArmy->addPlayer(new Player("Right Player", Race::createRaceWithId(_player2race), true, false));
	} else if (_gameMode == TUTORIAL) {
		_thisPlayer = new Player("Left Player", Race::createRaceWithId(_player1race), false, false);
		_leftArmy->addPlayer(_thisPlayer);

		_rightArmy->addPlayer(new Player("Right Player", Race::createRaceWithId(_player2race), false, false));
		gs.increaseLocalGamesStarted();
	} else if (_gameMode == NETWORK) {
		CCLOG("init player");
		std::vector<GameStat> gameStats = NetworkSingleton::getInstance()->_gameStats;
		GameStat myGameStat = NetworkSingleton::getInstance()->_myGameStat;
		std::vector < std::string > names = NetworkSingleton::getInstance()->_names;
		int myId = myGameStat.getPlayerId();
		std::string myName = NetworkSingleton::getInstance()->_myName;

		int numberOfPlayers = names.size() + 1; //+1 because of me
		ArmyType firstPlayersArmy = myId < (numberOfPlayers / 2) ? LEFT : RIGHT;
		ArmyType secondPlayersArmy = myId < (numberOfPlayers / 2) ? RIGHT : LEFT;
		_thisPlayer = NULL;
		//create the players, we have ids 1-$numberOfPlayers
		for (int playerId = 0; playerId < numberOfPlayers; playerId++) {
			ArmyType armyType = (playerId < (numberOfPlayers / 2)) ? firstPlayersArmy : secondPlayersArmy;
			std::string armyString = armyType == LEFT ? "left" : "right";
			//search gamestat with the corresponding id
			if (playerId == myId) {
				Race* r = 0;

				if (myGameStat.getRace() == 0) {
					r = Race::buildRaceFromRandomizedRaceString(myGameStat.getRandomRaceString());
				} else {
					r = Race::createRaceWithId(myGameStat.getRace());
				}

				Player* player = new Player(myName, r, false, false);
				player->_networkPlayerId = playerId;
				_thisPlayer = player;
				getMyArmy(armyType)->addPlayer(_thisPlayer);
				CCLOG("Added me with id: %i to %s", playerId, armyString.c_str());
			} else {
				bool addedAPlayer = false;
				for (unsigned int gi = 0; gi < gameStats.size(); gi++) {
					if (gameStats[gi].getPlayerId() == playerId) {
						addedAPlayer = true;
						std::string name = names[gi];
						int raceId = gameStats[gi].getRace();

						Race* r = 0;
						if (raceId == 0) {
							r = Race::buildRaceFromRandomizedRaceString(gameStats[gi].getRandomRaceString());
						} else {
							r = Race::createRaceWithId(raceId);
						}
						if (r == 0) //invalid network message
							r = Race::createRaceWithId(1);
						Player* player = new Player(name, r, false, true);
						player->_networkPlayerId = playerId;
						// adding some gold to avoid too much async behavior
						// but do not give too much to minimize cheating possibilities
						player->applyNetworkPlayerGoldBuffers();
						getMyArmy(armyType)->addPlayer(player);
						CCLOG("Added player with id: %i to %s", playerId, armyString.c_str());
					}
				}
				if (!addedAPlayer)
					CCLOG("If you see this, there is a bug in Model.cpp. A player is missing...");
			}
			if (_thisPlayer == NULL || getMyArmy(LEFT)->getPlayers().size() == 0 || getMyArmy(RIGHT)->getPlayers().size() == 0) {
				//this kills the game!

				CCLOG("KNOWN BUG! KILLED THE GAME. left: %i, right: %i, _thisPlayer: %s", (int) getMyArmy(LEFT)->getPlayers().size(),
						(int) getMyArmy(RIGHT)->getPlayers().size(), (_thisPlayer == NULL ? "NULL" : "NOT NULL"));
				SceneControl::replaceScene(SceneControl::queueScene(myGameStat.getRace(), myName), true);
			}
		}
		gs.increaseNetworkGamesStarted();
	}
}

/**
 * time in milliseconds until next spawn
 */
double Model::getNextSpawn() {
	double nextSpawn = _lastSpawn + _spawnSleep * 1000;
	return nextSpawn - BW_Time::getMilliSecondsCached();
}

double Model::getYourNextSpawn() {
	double nextSpawn = getNextSpawn();
	//how much switches will ocur?
	std::list<Player*> players = getMyArmy(_thisPlayer->getArmyTyp())->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		if (_thisPlayer == (*it))
			break;
		nextSpawn += _spawnSleep * 1000;
	}
	return nextSpawn;
}

Model::~Model() {
	delete _leftArmy;
	delete _rightArmy;
}

float Model::getMaxZoom() {
	return _maxZoom;
}
float Model::getMinZoom() {
	return _minZoom;
}

double Model::getSpawnSleep() {
	return _spawnSleep;
}

GameMode Model::getGameMode() {
	return _gameMode;
}

void Model::setGameMode(GameMode gameMode) {
	_gameMode = gameMode;
}

InGameGameLayer* Model::getInGameGameLayer() {
	return _inGameGameLayer;
}

void Model::setInGameGameLayer(InGameGameLayer* inGameGameLayer) {
	this->_inGameGameLayer = inGameGameLayer;
}

Player* Model::getThisPlayer() {
	return _thisPlayer;
}
void Model::setThisPlayer(Player* player) {
	_thisPlayer = player;
}

Model* Model::_instance = NULL;
Model* Model::getInstance() {
	if (_instance == NULL)
		_instance = new Model();

	return _instance;
}

void Model::reset() {
	if (_instance == NULL)
		return; //nothing to do
	delete _instance;
	_instance = NULL;
}

float Model::getFieldWidth() {
	return _fieldWidth;
}
float Model::getFieldHeigth() {
	return _fieldHeigth;
}

void Model::addToRemoveUnit(Unit* unit) {
	_toRemoveUnit.push_back(unit);
}

void Model::addToRemoveProjectile(Projectile* projectile) {
	_toRemoveProjectile.push_back(projectile);
}

void Model::removeTheUndead() {
	Projectile* projectile;
	while (!_toRemoveProjectile.empty()) {

		projectile = _toRemoveProjectile.front();
		this->getMyArmy(projectile->getArmyType())->removeProjectile(projectile);
		_toRemoveProjectile.pop_front();
		delete projectile;
	}

	Unit* unit;
	while (!_toRemoveUnit.empty()) {

		unit = _toRemoveUnit.front();
		this->getMyArmy(unit->getArmyType())->removeUnit(unit);
		_toRemoveUnit.pop_front();
		delete unit;
	}
}

Army* Model::getMyArmy(ArmyType myArmyType) {
	return myArmyType == RIGHT ? _rightArmy : _leftArmy;
}
Army* Model::getEnemyArmy(ArmyType myArmyType) {
	return myArmyType == RIGHT ? _leftArmy : _rightArmy;
}

void Model::update() {
	BW_Time::updateCachedMilliseconds();
	updateMiddleControl();

	_leftArmy->update();
	_rightArmy->update();

	_leftArmy->killUndeadUnits();
	_rightArmy->killUndeadUnits();

	this->removeTheUndead();
}

void Model::updateMiddleControl() {
	int numberofRightUnitsInLeft = _rightArmy->getNumberOfUnitsInEnemyField();
	int numberofLeftUnitsinRight = _leftArmy->getNumberOfUnitsInEnemyField();

	if (numberofRightUnitsInLeft == 0 && numberofLeftUnitsinRight > 0)
		_middleControl = LEFT;
	else if (numberofLeftUnitsinRight == 0 && numberofRightUnitsInLeft > 0)
		_middleControl = RIGHT;
	else if(numberofLeftUnitsinRight != 0 && numberofRightUnitsInLeft != 0)
		_middleControl = NONE;
}

ArmyType Model::getMiddleControl() {
	return _middleControl;
}

float Model::getGameSpeed() {
	return _gameSpeed;
}

void Model::increasegameSpeed() {
	//this will not increase spawn time, because this time is set at the beginning
	_gameSpeed /= 0.8;
}
void Model::decreasegameSpeed() {
	//this will not decrease spawn time, because this time is set at the beginning
	_gameSpeed *= 0.8;
}

void Model::setGameSpeed(float gameSpeed) {
	_gameSpeed = gameSpeed;
}

void Model::processAddedUnits() {
	float minLeftPos = 0;
	float maxRightPos = 0;

	//find max position
	for (std::list<Unit*>::iterator it = _addedUnits.begin(); it != _addedUnits.end(); ++it) {
		Unit* unit = (*it);
		if (unit->getArmyType() == LEFT) {
			if (unit->getPositionBW().x < minLeftPos)
				minLeftPos = unit->getPositionBW().x;
		} else {
			if (unit->getPositionBW().x > maxRightPos)
				maxRightPos = unit->getPositionBW().x;
		}
	}

	//set new position due to attack range
	for (std::list<Unit*>::iterator it = _addedUnits.begin(); it != _addedUnits.end(); ++it) {
		Unit* unit = (*it);
		BW_Point newPos = unit->getPositionBW();

		if (unit->getArmyType() == LEFT)
			newPos.x = minLeftPos - unit->getAttack()->getAttackRange() / 4;
		else
			newPos.x = maxRightPos + unit->getAttack()->getAttackRange() / 4;

		unit->setPosition(newPos);
	}

	_addedUnits.clear();
}
