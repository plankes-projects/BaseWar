/*
 * InGameGameLayer.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "InGameGameLayer.h"
#include "../../../Model/Model.h"
#include "../../../Tools/BW_Point.h"
#include "../../../Model/Player.h"
#include "../../../Tools/BW_Time.h"
#include "../../SpriteTags.h"
#include "../../CCAlertView.h"
#include "../../../Network/NetworkSingleton.h"
#include "../../../Constants.h"
#include "../../../Tools/Tools.h"
#include "../../../Sound/SoundHandler.h"
#include "../../../Network/ServerCommunication.h"
#include "../../../Tools/OfflineLogger.h"

#ifdef DEBUG
#include "../../../SceneControl.h"
#include "../../../Debug/BalanceSingleton.h"
#endif

bool InGameGameLayer::init() {
	//rgba
	//if (!CCLayerGradient::initWithColor(ccc4(24, 107, 39, 255), ccc4(212, 159, 93, 255), CCPoint(1, 0)))
	//return false;
	//if (!CCLayerColor::initWithColor(ccc4(218, 133, 66, 255)))
	//	return false;
	int red = Tools::random(100, 200);
	int green = Tools::random(100, 200);
	int blue = Tools::random(100, 200);
	if (!CCLayerColor::initWithColor(ccc4(red, green, blue, 255)))
		return false;
	this->setTouchEnabled(true);
	CCLOG("Field color (r|g|b): %i|%i|%i", red, green, blue);

	_autoCam = true; //	enable default
	_autoCamWatchingAtTarget = false;
	_sendLogInOrbit = 0;
	_maxSendLogInOrbit = 10;
	_playersAreForceAI = false;
	_lastLogTime = BW_Time::getMilliSecondsOverCached();

	Model::getInstance()->setInGameGameLayer(this);

	setContentSize(CCSize(Model::getInstance()->getFieldWidth(), Model::getInstance()->getFieldHeigth()));

	//need to do before init player because of income
	BW_Time::resetStopDiff();
	BW_Time::updateCachedMilliseconds();
	BW_Time::resetStartTime(Model::getInstance()->getGameSpeed());

	Model::getInstance()->initializePlayer();
	Model::getInstance()->initializeGameField();
	Model::getInstance()->initLastSpawn();

	setInitialPosition();

	this->schedule(schedule_selector(InGameGameLayer::update), 0.1f);
	this->schedule(schedule_selector(InGameGameLayer::spriteLocationUpdate));

	GameMode mode = Model::getInstance()->getGameMode();
	if (mode == NETWORK)
		this->schedule(schedule_selector(InGameGameLayer::networkUpdate), NETWORK_SYNC_DELAY);

	initBackground();

	_offlineLog = (LOG_OFFLINE_GAMES && (mode == PVP || mode == PVA));
	if (_offlineLog) {
		OfflineLogger::getInstance()->initNewGameLog();
		OfflineLogger::getInstance()->logInformationForCurrentGame("GAME START");
	}

	if (Model::getInstance()->getGameMode() == NETWORK) {
		ServerCommunication().sendGameInfo("GAME START");
	}

	return true;
}

void InGameGameLayer::setPositionAndAvoidLayerToLeaveScreen(float x, float y) {

	float scale = getScale();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float max_x = Model::getInstance()->getFieldWidth() * (scale - 1) / 2;
	float min_x = Model::getInstance()->getFieldWidth() * (-scale - 1) / 2 + winSize.width;

	if (Model::getInstance()->getFieldWidth() * scale < winSize.width) {
		float tmp = min_x;
		min_x = max_x;
		max_x = tmp;
	}

	float max_y = Model::getInstance()->getFieldHeigth() * (scale - 1) / 2;
	float min_y = Model::getInstance()->getFieldHeigth() * (-scale - 1) / 2 + winSize.height;

	if (Model::getInstance()->getFieldHeigth() * scale < winSize.height) {
		float tmp = min_y;
		min_y = max_y;
		max_y = tmp;
	}

	if (x < min_x)
		x = min_x;
	else if (x > max_x)
		x = max_x;

	if (y < min_y)
		y = min_y;
	else if (y > max_y)
		y = max_y;

	setPosition(x, y);
}

void InGameGameLayer::spriteLocationUpdate(float dt) {
	//do not put this in an extra thread!!
	// we set position of sprite here which is used by game logic
	// --> it is not atomar!!

	//move all units
	std::list<Unit*> units = Model::getInstance()->getMyArmy(LEFT)->getUnits();
	for (std::list<Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = (*it);
		unit->updateMovePosition(dt);
	}

	units = Model::getInstance()->getMyArmy(RIGHT)->getUnits();
	for (std::list<Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = (*it);
		unit->updateMovePosition(dt);
	}

	//move all projectiles
	std::list<Projectile*> projectiles = Model::getInstance()->getMyArmy(LEFT)->getProjectiles();
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* projectile = (*it);
		projectile->updateMovePosition(dt);
	}
	projectiles = Model::getInstance()->getMyArmy(RIGHT)->getProjectiles();
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* projectile = (*it);
		projectile->updateMovePosition(dt);
	}

	//move autocam if activated
	performAutocam(dt);
}

void InGameGameLayer::performAutocam(float dt) {
	if (!_autoCam)
		return;
	Unit* unit = Model::getInstance()->getMyArmy(Model::getInstance()->getAutoCamTarget())->getMostProgressUnit();
	if (unit == NULL)
		return;

	float speed = 1600;
	// we have another min speed, 3x run speed of this unit
	//	so we always catch it in time, even if it is a very fast unit
	float anotherSpeed = unit->getRunspeed() * 3;

	if (speed < anotherSpeed)
		speed = anotherSpeed;

	BW_Point watching = watchingAt();
	BW_Point wanted = unit->getPositionBW();
	float fullDist = watching.distanceTo(wanted);
	float stepDist = speed * dt;
	if (fullDist <= stepDist) {
		watchAt(wanted);
		_autoCamWatchingAtTarget = true;
		return;
	}
	float mult = stepDist / fullDist;

	float xDelta = mult * (wanted.x - watching.x);
	float yDelta = mult * (wanted.y - watching.y);

	watchAt(BW_Point(watching.x + xDelta, watching.y + yDelta));

	//we do not check for y because y will change all the time (unit moveing)
	_autoCamWatchingAtTarget = watchingAt().x == watching.x;
}

void InGameGameLayer::networkUpdate(float dt) {
	if (Model::getInstance()->getGameMode() != NETWORK)
		return; //all players left and we switched to local game

	NetworkSingleton::getInstance()->updateNetwork();
}

void InGameGameLayer::setAutoCam(bool autoCam) {
	_autoCam = autoCam;
}

bool InGameGameLayer::getAutoCam() {
	return _autoCam;
}

void InGameGameLayer::onLogCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (Model::getInstance()->getStartedAs() != NETWORK) {
		CCLOG("ERROR - This should only be called at a network game!");
		return;
	}

	//we do not want log games vs normal ai
	if (_playersAreForceAI)
		return;

	double now = BW_Time::getMilliSecondsOverCached();
	if ((now - _lastLogTime) > (Model::getInstance()->getSpawnSleep() * 2000)) {
		//connection fail since a whole spawn phase...
		//switch every play to force ai

		std::list<Player*> players = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
		for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
			if (Model::getInstance()->getThisPlayer() != (*it))
				(*it)->changeToForceOldAIPlayer();
		}
		players = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
		for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
			if (Model::getInstance()->getThisPlayer() != (*it))
				(*it)->changeToForceOldAIPlayer();
		}

		Model::getInstance()->setGameMode(PVA);

		_playersAreForceAI = true;
	}

	_sendLogInOrbit--;
	if (!response) {
		CCLog("server log failed - response missing");
		if (_sendLogInOrbit < _maxSendLogInOrbit) {
			_sendLogInOrbit++;
			ServerCommunication().sendLog(getSpawnLogData(), this, httpresponse_selector(InGameGameLayer::onLogCompleted));
		}
		return;
	}

	if (!response->isSucceed()) {
		CCLog("server log failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		if (_sendLogInOrbit < _maxSendLogInOrbit) {
			_sendLogInOrbit++;
			ServerCommunication().sendLog(getSpawnLogData(), this, httpresponse_selector(InGameGameLayer::onLogCompleted));
		}
		return;
	}

	std::string resp = Tools::toString(response->getResponseData());
	_lastLogTime = now;
	CCLOG("LOGGED Spawn data");
}

std::string InGameGameLayer::getSpawnLogData() {
	std::string data = "";
	data = "some log";

	return data;
}

void InGameGameLayer::spawnUnits() {
	if (!_playersAreForceAI && Model::getInstance()->getStartedAs() == NETWORK) {
		//we log before every spawn
		_sendLogInOrbit++;
		ServerCommunication().sendLog(getSpawnLogData(), this, httpresponse_selector(InGameGameLayer::onLogCompleted));
	}

	Model::getInstance()->updateLastSpawn();
	Model::getInstance()->getMyArmy(LEFT)->spawnUnits();
	Model::getInstance()->getMyArmy(RIGHT)->spawnUnits();

	//sort units by attack range
	Model::getInstance()->processAddedUnits();

	NetworkSingleton::getInstance()->clearSpawnPositionVectors();
	NetworkSingleton::getInstance()->increaseWaveNum();

	if (_offlineLog) {
		OfflineLogger::getInstance()->logSpawn();
	}
}

void InGameGameLayer::update(float dt) {
	//we are not allowed to put this function into an extra thread:
	//Don’t call any functions which invokes CCObject::retain(), CCObject::release() or CCObject::autorelease() ...
	// and attack (range, projectile) + spawn + effects + move to + remove undead + ... do call this functions...

	Model::getInstance()->update();

	if (Model::getInstance()->getNextSpawn() < 0)
		spawnUnits();

	//set z order -> draw order
	CCObject* obj;
	CCNode* node;
	float max_y = Model::getInstance()->getFieldHeigth();

	CCARRAY_FOREACH(getChildren(), obj)
	{
		node = (CCNode*) obj;
		if (node->getTag() != NOUNIT) {
			node->setZOrder((max_y - node->getPositionY() + node->getContentSize().height / 2 * node->getScaleY()) * 1000);
		}
	}
}

void InGameGameLayer::watchAt(BW_Point point) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float fieldWidth = Model::getInstance()->getFieldWidth();
	float fieldHeigth = Model::getInstance()->getFieldHeigth();

	float pos_x = (winSize.width - fieldWidth + getScaleX() * (fieldWidth - 2 * point.x)) / 2;
	float pos_y = (winSize.height - fieldHeigth + getScaleY() * (fieldHeigth - 2 * point.y)) / 2;

	setPositionAndAvoidLayerToLeaveScreen(pos_x, pos_y);
	upgradeBackground();
}

BW_Point InGameGameLayer::watchingAt() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float fieldWidth = Model::getInstance()->getFieldWidth();
	float fieldHeigth = Model::getInstance()->getFieldHeigth();

	float x = (winSize.width - fieldWidth + getScaleX() * fieldWidth - 2 * getPosition().x) / (2 * getScaleX());
	float y = (winSize.height - fieldHeigth + getScaleY() * fieldHeigth - 2 * getPosition().y) / (2 * getScaleY());

	return BW_Point(x, y);
}

float InGameGameLayer::setInitialPosition() {
	//	start zoomed in by factor 8 (6 times zoomIn btn touched -> 1.41^6 ~ 8)
	float scale = CCDirector::sharedDirector()->getWinSize().width / Model::getInstance()->getFieldWidth() * 6;
	if (Model::getInstance()->getGameMode() == TUTORIAL) {
		scale = CCDirector::sharedDirector()->getWinSize().width / Model::getInstance()->getFieldWidth();
	}

	setScale(scale);

	//set init position of auto cam

	if (_autoCam) {
		Unit* unit = Model::getInstance()->getMyArmy(Model::getInstance()->getAutoCamTarget())->getMostProgressUnit();
		if (unit == NULL) {
			CCLog("ERROR NO UNIT FOUND FOR INIT CAM POSITION");
			return scale;
		}
		watchAt(unit->getPositionBW());
	} else {
		watchAt(BW_Point(Model::getInstance()->getFieldWidth() / 2, Model::getInstance()->getFieldHeigth() / 2));
	}
	return scale;
}

void InGameGameLayer::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent) {
	initialPos = getPosition();

	//we only handle one finger touch here because android do net call this function at 2 finger init
	//2 finger init is done with _2fingerInit flag
	CCTouch* touch = (CCTouch*) pTouches->anyObject();
	initialTouch = touch->getLocation();

	_2fingerInit = false;
}

void InGameGameLayer::ccTouchesMoved(CCSet * pTouches, CCEvent* event) {
	if (pTouches->count() == 1) {
		if (_autoCam)
			return;
		if (!_2fingerInit) {
			//just do this if we have no 2 finger init
			// --> forcing the user to remove all fingers before moving after zoom
			//we move the full layer here
			CCTouch* touch = (CCTouch*) pTouches->anyObject();

			float x = initialPos.x + (touch->getLocation().x - initialTouch.x);
			float y = initialPos.y + (touch->getLocation().y - initialTouch.y);
			//avoid the screen middle to be outside of the gamefield

			setPositionAndAvoidLayerToLeaveScreen(x, y);
		}
	} else if (pTouches->count() == 2) {

		CCTouch* touch1 = (CCTouch*) pTouches->anyObject();
		pTouches->removeObject(touch1);
		CCTouch* touch2 = (CCTouch*) pTouches->anyObject();

		CCPoint middlePos = CCPoint(touch1->getLocation().x + (touch2->getLocation().x - touch1->getLocation().x) / 2,
				touch1->getLocation().y + (touch2->getLocation().y - touch1->getLocation().y) / 2);

		float touchDistance = BW_Point(touch1->getLocation()).distanceTo(BW_Point(touch2->getLocation()));
		if (!_2fingerInit) {
			_2fingerInit = true;
			initialTouch = middlePos;
			initialTouchDistance = touchDistance;
			initialZoom = getScale();
		} else {

			float diff = touchDistance / initialTouchDistance;

			//float x = initialPos.x + (middlePos.x - initialTouch.x);
			//float y = initialPos.y + (middlePos.y - initialTouch.y);

			zoom(initialZoom * diff);

			//setPosition(x * getScale() / (initialZoom * diff), y * getScale() / (initialZoom * diff));
			//touchDistance
			//TODO: zoom with movement
		}

	}
	upgradeBackground();
}

void InGameGameLayer::zoom(float zoom) {
	if (zoom > Model::getInstance()->getMaxZoom())
		zoom = Model::getInstance()->getMaxZoom();

	if (zoom < Model::getInstance()->getMinZoom())
		zoom = Model::getInstance()->getMinZoom();

	//if we zoom in, we have to look at the unit if autocam and if already watching it
	if (getScale() < zoom && _autoCam && _autoCamWatchingAtTarget) {
		Unit* unit = Model::getInstance()->getMyArmy(Model::getInstance()->getAutoCamTarget())->getMostProgressUnit();
		if (unit != NULL) {
			setScale(zoom);
			watchAt(unit->getPositionBW());
			return;
		}
	}

	BW_Point watching = watchingAt();
	setScale(zoom);
	watchAt(watching);
}

InGameGameLayer::~InGameGameLayer() {
	for (unsigned int i = 0; i < _backGroundSprites.size(); i++) {
		_backGroundSprites[i]->release();
	}
}

void InGameGameLayer::initBackground() {
	std::vector < std::map<std::string, float> > allbackGroundsets;
	std::vector < std::string > allBackgroundSounds;

	allBackgroundSounds.push_back("sound_battle1");
	std::map<std::string, float> backGroundset1;
	backGroundset1.insert(std::pair<std::string, float>("bg_tree1.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree2.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree3.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree4.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree5.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree6.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree7.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree8.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree9.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree10.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree11.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree12.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_tree13.png", 300));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush1.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush2.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush3.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush4.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush5.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush6.png", 100));
	backGroundset1.insert(std::pair<std::string, float>("bg_bush7.png", 100));
	allbackGroundsets.push_back(backGroundset1);

	allBackgroundSounds.push_back("sound_battle2");
	std::map<std::string, float> backGroundset2;
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree1.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree2.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree3.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree4.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree5.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree6.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_tree7.png", 300));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush1.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush2.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush3.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush4.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush5.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush6.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush7.png", 100));
	backGroundset2.insert(std::pair<std::string, float>("bg1_bush8.png", 100));
	allbackGroundsets.push_back(backGroundset2);

	int randomSet = Tools::random(0, allbackGroundsets.size() - 1);

	//start sound for this set
	SoundHandler::getInstance()->playBackground(allBackgroundSounds[randomSet]);

	//place images for this set
	std::map<std::string, float> backGroundSpritesNameAndHeight = allbackGroundsets[randomSet];
	//collect all keys for randomness
	std::vector < std::string > keys;
	for (std::map<std::string, float>::iterator it = backGroundSpritesNameAndHeight.begin(); it != backGroundSpritesNameAndHeight.end(); ++it) {
		keys.push_back(it->first);
	}

	_backgroundSpriteMaxWidth = 0;
	_backgroundSpriteMaxHeight = 0;
	float spaceRange = BACKGROUND_SPRITES_DISTANCE / 2.0f;
	cocos2d::CCSprite * sprite;

	for (int x = 0; x < Model::getInstance()->getFieldWidth(); x += BACKGROUND_SPRITES_DISTANCE) {
		for (int y = 0; y < Model::getInstance()->getFieldHeigth(); y += BACKGROUND_SPRITES_DISTANCE) {
			float used_x = Tools::random(x - spaceRange, x + spaceRange);
			float used_y = Tools::random(y - spaceRange, y + spaceRange);
			if (used_y < 0)
				used_y = 0;

			std::string randSprite = keys[Tools::random(0, keys.size() - 1)];
			float height = backGroundSpritesNameAndHeight[randSprite];
			height += Tools::random(0, height) - height / 2;

			sprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(randSprite.c_str()));
			sprite->setScale(height / sprite->getContentSize().height);
			sprite->setPosition(CCPoint(used_x, used_y + height / 2));
			sprite->setOpacity(200);
			sprite->retain();
			if (Tools::random(0, 1) == 1)
				sprite->setFlipX(true);
			_backGroundSprites.push_back(sprite);

			float width = sprite->getContentSize().width * sprite->getScale();

			if (_backgroundSpriteMaxWidth < width)
				_backgroundSpriteMaxWidth = width;

			if (_backgroundSpriteMaxHeight < height)
				_backgroundSpriteMaxHeight = height;
		}
	}
}

void InGameGameLayer::upgradeBackground() {
	BW_Rect shownRec = getShownRect();
	shownRec.leftDown.x -= _backgroundSpriteMaxWidth / 2 * getScale();
	shownRec.leftDown.y -= _backgroundSpriteMaxHeight / 2 * getScale();
	shownRec.rightUp.x += _backgroundSpriteMaxWidth / 2 * getScale();
	shownRec.rightUp.y += _backgroundSpriteMaxHeight / 2 * getScale();

	cocos2d::CCSprite * sprite;
	for (unsigned int i = 0; i < _backGroundSprites.size(); i++) {
		sprite = _backGroundSprites[i];
		if (shownRec.contains(BW_Point(sprite->getPosition().x, sprite->getPosition().y))) {
			if (sprite->getParent() == NULL)
				addChild(sprite);
		} else {
			if (sprite->getParent() != NULL)
				removeChild(sprite);
		}
	}
}

BW_Rect InGameGameLayer::getShownRect() {
	float scaleX = getScaleX();
	float scaleY = getScaleY();
	float fieldWidth = Model::getInstance()->getFieldWidth();
	float fieldHeigth = Model::getInstance()->getFieldHeigth();

	float x = (-fieldWidth / 2 - getPosition().x) / scaleX + fieldWidth / 2;
	float y = (-fieldHeigth / 2 - getPosition().y) / scaleY + fieldHeigth / 2;

	BW_Point leftDown = BW_Point(x, y);
	if (leftDown.x < 0)
		leftDown.x = 0;
	if (leftDown.y < 0)
		leftDown.y = 0;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	BW_Point rightUp = BW_Point(x + winSize.width / scaleX, y + winSize.height / scaleY);
	if (rightUp.x > fieldWidth)
		rightUp.x = fieldWidth;
	if (rightUp.y > fieldHeigth)
		rightUp.y = fieldHeigth;

	return BW_Rect(leftDown, rightUp);
}

void InGameGameLayer::showPlayerLeftMessage(std::string playerName) {
	std::string title = playerName + " has left";

	std::string content = "Computer will play for him.\n\n";

	std::list<Player*> players = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		content += (*it)->getNetworkDisplayName() + "\n";
	}

	content += "--------- vs ---------\n";

	players = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		content += (*it)->getNetworkDisplayName() + "\n";
	}

	CCAlertView *alert = CCAlertView::create(title.c_str(), content.c_str(), "Ok");

	this->getParent()->addChild(alert, 100);
}

