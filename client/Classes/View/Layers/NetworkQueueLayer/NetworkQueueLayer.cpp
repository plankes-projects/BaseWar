#include "NetworkQueueLayer.h"

#include "../../../Model/Model.h"
#include "../../../SceneControl.h"
#include "../../../Network/ServerCommunication.h"
#include "../../../Network/NetworkSingleton.h"
#include "../../../Tools/Tools.h"
#include "../../../Tools/BW_Time.h"
#include "../../../Model/GameMode.h"
#include "../../../Constants.h"
#include "../../../Tools/ViewTools.h"
#include "../../../AchievementSystem/AchievementSystem.h"

// on "init" you need to initialize your instance
bool NetworkQueueLayer::init() {
	Model::getInstance()->setGameMode(NETWORK);
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 255)))
		return false;

	_gameStarted = false;

	//adding the background
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCSprite* backgroundArt = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("menuBackground.png"));

	float bgScale = winSize.height / backgroundArt->getContentSize().height;
	backgroundArt->setScale(bgScale);

	float diff = winSize.width - backgroundArt->getContentSize().width * bgScale;
	float x = backgroundArt->getContentSize().width / 2 * bgScale + diff / 2;
	float y = backgroundArt->getContentSize().height / 2 * bgScale;
	backgroundArt->setPosition(ccp(x, y));
	addChild(backgroundArt);

	float button_length = 66 * scaleMult;
	float button_heigth = 66 * scaleMult;
	float padding = 10 * scaleMult;

	_leaveQueue = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return0.png"), CCSprite::createWithSpriteFrameName("return1.png"),
			(CCObject*) this, menu_selector(NetworkQueueLayer::leaveQueueTouched));
	_leaveQueue->setScaleX(button_length / _leaveQueue->getContentSize().width);
	_leaveQueue->setScaleY(button_heigth / _leaveQueue->getContentSize().height);

	CCMenu* menu = CCMenu::create();
	menu->addChild(_leaveQueue, 1);
	menu->alignItemsHorizontallyWithPadding(padding);
	addChild(menu);

	_sendingRequest = false;
	_foundGame = false;

	_countDown = CCLabelTTF::create("", FONT_NAME, 150 * scaleMult);
	_countDown->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	_countDown->setColor(ccc3(74, 24, 147));
	addChild(_countDown);

	//init server message
	float label_font_size = 25 * scaleMult;
	const char* label_font = FONT_NAME;
	ccColor3B color = ccc3(0, 0, 0);

	_serverMessage = CCLabelTTF::create("Connecting to basewar.net ...", label_font, label_font_size);
	_serverMessage->setColor(color);
	_serverMessage->setPosition(ccp(winSize.width / 2, winSize.height / 2 - (button_heigth / 2 + padding + _serverMessage->getContentSize().height / 2)));
	this->addChild(_serverMessage);

	_serverInformation = CCLabelTTF::create("", label_font, label_font_size);
	_serverInformation->setColor(color);
	this->addChild(_serverInformation);
	_loadedServerInformation = false;
	_loadedPlayerNumInformation = false;
	_loadedTeamsizeConfig = false;
	_maxSecToStart = -1; //will be set by server request
	_playerSet = false;
	_teamsizeConfig = 1;

	_queueTimeInSec = BW_Time::getSeconds();
	_numOfPlayersOnline = -1;

	ServerCommunication().sendGameProgress();

	this->schedule(schedule_selector(NetworkQueueLayer::update), 1);

	float offsetDown = ViewTools::addVersionAndCreditInfo(this, scaleMult);
	ViewTools::addIdleUnitTo(this, offsetDown);

	return true;
}

void NetworkQueueLayer::update(float dt) {
	if (_foundGame)
		gameCounterPart();
	else
		queuePart();
}

void NetworkQueueLayer::gameCounterPart() {
	if (!_playerSet) {
		_playerSet = true;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
		std::string vsStringPartOne = "";
		std::string vsStringPartZwo = "";
		std::string vsString = "";
		bool second = false;
		bool playerAtSecond = false;
		std::vector < std::string > names = NetworkSingleton::getInstance()->_names;

		int playerIdWrittenLast = -1;
		bool gotMyName = false;
		int numGamestats = NetworkSingleton::getInstance()->_gameStats.size();
		for (int i = 0; i < numGamestats; i++) {
			GameStat current = NetworkSingleton::getInstance()->_gameStats[i];
			int id = current.getPlayerId();
			if (id == (playerIdWrittenLast + 1)) {
				vsString = names[i] + "\n";
			} else {
				gotMyName = true;
				playerAtSecond = second;
				i--; //we need to use this id again
				vsString = _playerName + "\n";
			}

			if (second)
				vsStringPartZwo += vsString;
			else
				vsStringPartOne += vsString;

			playerIdWrittenLast++;
			if (playerIdWrittenLast + 1 == (numGamestats + 1) / 2)
				second = true;
		}
		if (!gotMyName) { //my name was at the end, add it
			vsStringPartZwo += _playerName + "\n";
			playerAtSecond = second;
		}

		if (playerAtSecond) {
			vsString = vsStringPartZwo + "--- vs ---\n" + vsStringPartOne;
		} else {
			vsString = vsStringPartOne + "--- vs ---\n" + vsStringPartZwo;
		}
		cocos2d::CCLabelTTF* vsInfo = CCLabelTTF::create(vsString.c_str(), FONT_NAME, 25 * scaleMult);
		vsInfo->setColor(ccc3(0, 0, 0));
		vsInfo->setPosition(ccp(vsInfo->getContentSize().width / 2 + 10 * scaleMult, winSize.height / 2));
		this->addChild(vsInfo);
	}

	int start = NetworkSingleton::getInstance()->_gameStart;
	int now = BW_Time::getSeconds();
	if (start < now) {
		//fixes a bug on slower devices which results in a double call of this
		if (_gameStarted)
			return;
		_gameStarted = true;
		NetworkSingleton::getInstance()->_myName = _playerName;
		SceneControl::replaceScene(SceneControl::gameScene(1, NETWORK), true);
		return;
	}

	int left = start - now;
	if (left == 0)
		_countDown->setString("Fight!");
	else
		_countDown->setString(Tools::toString(left).c_str());

	CCFiniteTimeAction *scale1 = CCScaleTo::create(0.15f, 1.3f);
	CCFiniteTimeAction *scale2 = CCScaleTo::create(0.1f, 0.7f);
	CCFiniteTimeAction *scale3 = CCScaleTo::create(0.05f, 1.0f);
	_countDown->runAction(CCSequence::create(scale1, scale2, scale3, NULL));
}

void NetworkQueueLayer::onQueueCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response)
		return completeQueueRequest(-1, "");

	if (!response->isSucceed()) {
		CCLog("server get queue response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return completeQueueRequest(-1, "");
	}

	std::string resp = Tools::toString(response->getResponseData());

	CCLOG("queue resp: %s", resp.c_str());

	//CCLOG("string begin ---------");
	//CCLOG(resp.c_str());
	//CCLOG("string end   ---------");
	float recieve_time = BW_Time::getMilliSeconds();

	//looking at it line by line
	std::istringstream f(resp);
	std::string line;
	std::getline(f, line);
	std::string firstServerLine = line;
	if (firstServerLine == "")
		return completeQueueRequest(-3, firstServerLine);

	/** Server returns:
	 joined queue: 0
	 in queue: 1
	 Game init:
	 -18
	 0:1+klaus
	 1:1+klaus
	 2:1+123-132-41-12-412-423-1-412-1+klaus
	 3:1+klaus
	 4:1+klaus
	 5
	 */

	NetworkSingleton *ns = NetworkSingleton::getInstance();
	ns->reset();

	int addLines = -1;
	while (std::getline(f, line)) {
		//empty line is last
		if (line == "")
			break;;

		addLines++;
		//CCLOG("line: %s", line.c_str());
		std::size_t found = line.find(":");
		if (found == std::string::npos) {
			//this is my id
			if (ns->_myGameStat.getPlayerId() == -1) {
				int id = atoi(line.c_str());
				if (id != addLines) //invalid
						{
					//CCLOG("inv 1: %i != %i", id, addLines);
					return completeQueueRequest(-2, firstServerLine);
				}
				ns->_myGameStat.setRace(_raceId);
				ns->_myGameStat.setPlayerId(id);
				ns->_myGameStat.setRandomRaceString(_randomRaceString);
				continue;
			} else {
				//invalid
				CCLOG("inv 2: ");
				return completeQueueRequest(-3, firstServerLine);
			}
		} else if (found != 1) {
			//invalid
			CCLOG(line.c_str(), 1);
			return completeQueueRequest(-4, firstServerLine);
		}

		found = line.find("+");
		if (found != 3) {
			CCLOG("inv 4");
			return completeQueueRequest(-5, firstServerLine);
		}

		GameStat gs = GameStat();
		int id = line[0] - '0';
		if (id != addLines) { //invalid
			CCLOG("inv 5: %i != %i", id, addLines);
			return completeQueueRequest(-6, firstServerLine);
		}

		gs.setPlayerId(id);
		int raceId = line[2] - '0';
		gs.setRace(raceId);
		std::string name = line.substr(found + 1);
		if (raceId == 0) {
			//there must be a randomRaceString
			//2:1+123-132-41-12-412-423-1-412-1+klaus
			found = name.find("+");
			if (found == std::string::npos) {
				CCLOG("inv 6");
				return completeQueueRequest(-6, firstServerLine);
			}
			std::string randomRaceString = name.substr(0, found);
			gs.setRandomRaceString(randomRaceString);
			CCLOG("Name: %s", name.c_str());
			name = name.substr(found + 1);
			CCLOG("RandomRaceString: %s", randomRaceString.c_str());
		}

		ns->_names.push_back(name);
		ns->_gameStats.push_back(gs);
	}

	//check number of players
	if (ns->_names.size() == 0)
		return completeQueueRequest(0, firstServerLine);
	if ((ns->_names.size() + 1) % 2 != 0) //+1 because of my id
			{
		CCLOG("inv 7");
		return completeQueueRequest(-7, firstServerLine);
	}

	//calc starttime
	int delay = (recieve_time - _send_time) / 2000;

	//first line is the time to start number
	ns->_gameStart = BW_Time::getSeconds() + atoi(firstServerLine.c_str()) - delay;

	/*
	 CCLOG("read some data: -----------");
	 CCLOG("start: %i", ns->_gameStart);
	 CCLOG("myid: %i", ns->_myGameStat.getPlayerId());
	 for(int i = 0; i < ns->_names.size(); i++)
	 {
	 CCLOG("Name: %s, id: %i, race: %i", ns->_names[i].c_str(), ns->_gameStats[i].getPlayerId(),  ns->_gameStats[i].getRace());
	 }
	 CCLOG("-----------");
	 */

	return completeQueueRequest(2, firstServerLine);
}

void NetworkQueueLayer::initAINetworkPlayers() {
	int allPlayerSize = _teamsizeConfig * 2;
	if (allPlayerSize < 2 || allPlayerSize > 6)
		allPlayerSize = 6;

	NetworkSingleton *ns = NetworkSingleton::getInstance();
	ns->reset();
	ns->_myGameStat.setRace(_raceId);
	ns->_myGameStat.setPlayerId(Tools::random(0, allPlayerSize - 1));
	ns->_myGameStat.setRandomRaceString(_randomRaceString);

	//get number of possible races
	int maxPossibleRaceId = 0;
	for (Race* r = Race::createRaceWithId(maxPossibleRaceId); r != 0; maxPossibleRaceId++, r = Race::createRaceWithId(maxPossibleRaceId)) {
		delete r;
	}
	maxPossibleRaceId--; //remove not existing id

	//generate random player names

	std::vector < std::string > playerNames = getFakePlayerNames();

	for (int id = 0; id < allPlayerSize; id++) {
		if (ns->_myGameStat.getPlayerId() == id)
			continue;

		GameStat gs = GameStat();
		gs.setPlayerId(id);

		gs.setRace(Tools::random(0, maxPossibleRaceId));
		gs.setRandomRaceString(gs.getRace() == 0 ? Race::createRandomizedRaceString() : "");

		int fakePlayerNameIndex = Tools::random(0, playerNames.size() - 1);
		ns->_names.push_back(playerNames[fakePlayerNameIndex]);
		playerNames.erase(playerNames.begin() + fakePlayerNameIndex);

		ns->_gameStats.push_back(gs);
	}

	ns->_gameStart = BW_Time::getSeconds() + Tools::random(8, 10);
}

void NetworkQueueLayer::completeQueueRequest(int code, std::string firstServerLine) {
	if (code == -1) {
		_serverMessage->setString("Could not connect to the server (-1)");
		CCLOG("Could not connect to the server! (-1)");
	} else if (code < -1) {
		std::string msg = "Server communication error (" + Tools::toString(code) + ")";
		_serverMessage->setString(msg.c_str());
		CCLOG(msg.c_str(), 1);
	} else if (code == 2) {
		CCLOG("Filled the NetworkSingleton! Game created.");
		_leaveQueue->removeFromParentAndCleanup(true);
		_serverMessage->removeFromParentAndCleanup(true);
		_serverMessage = NULL;
		NetworkSingleton::getInstance()->fakeNetworkGame = false;
		_foundGame = true;

	} else if (code == 0) {
		std::string waiting = Tools::toString(_teamsizeConfig) + " vs " + Tools::toString(_teamsizeConfig);
		std::string numOfPlayer = "\nPlayers online: " + (_numOfPlayersOnline == -1 ? "-" : Tools::toString(_numOfPlayersOnline));

		//Do something with firstServerLine. currently its 1/2

		int secLeft = _queueTimeInSec - BW_Time::getSeconds() + _maxSecToStart;
		std::string estimatedTime = "\nEstimated seconds until start: " + (secLeft < 0 ? "0" : Tools::toString(secLeft));
		_serverMessage->setString((waiting + numOfPlayer + estimatedTime).c_str());

		if (secLeft <= 0) {
			initAINetworkPlayers();
			_leaveQueue->removeFromParentAndCleanup(true);
			_serverMessage->removeFromParentAndCleanup(true);
			_serverMessage = NULL;
			NetworkSingleton::getInstance()->fakeNetworkGame = true;
			_foundGame = true;
		}
	}

	if (_serverMessage != NULL) {
		float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
		float padding = 10 * scaleMult;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		float button_heigth = 66 * scaleMult;
		_serverMessage->setPosition(ccp(winSize.width / 2, winSize.height / 2 - (button_heigth / 2 + padding + _serverMessage->getContentSize().height / 2)));
	}

	_sendingRequest = false;
}

void NetworkQueueLayer::onTeamsizeConfigCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response) {
		CCLog("server team size info response failed - response missing");
		_loadedTeamsizeConfig = false;
		return;
	}

	if (!response->isSucceed()) {
		CCLog("server team size info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		_loadedTeamsizeConfig = false;
		return;
	}

	std::string resp = Tools::toString(response->getResponseData());
	_teamsizeConfig = atoi(resp.c_str());

	//we are sure to have server connection here
	for (int i = 0; i < NUMBER_OF_OFFLINE_GAMES_SEND_AT_ONE_TIME; i++)
		ServerCommunication().sendOldestOfflineLog();
}

void NetworkQueueLayer::onPlayerNumInfoCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response) {
		CCLog("server player num info response failed - response missing");
		_loadedPlayerNumInformation = false;
		return;
	}

	if (!response->isSucceed()) {
		CCLog("server player num info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		_loadedPlayerNumInformation = false;
		return;
	}

	std::string resp = Tools::toString(response->getResponseData());
	std::vector < std::string > playerNumEstimatedTime = Tools::splitStringBy(resp, '-');
	if (playerNumEstimatedTime.size() != 2) {
		CCLOG("[ERROR] Playernum should has the format 'PlayerNum-EstimatedSec'");
		_loadedPlayerNumInformation = false;
		return;
	}
	_numOfPlayersOnline = atoi(playerNumEstimatedTime[0].c_str());
	_maxSecToStart = atoi(playerNumEstimatedTime[1].c_str());
}

void NetworkQueueLayer::onGetServerInfoCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	if (!response) {
		CCLog("server get info response failed - response missing");
		_loadedServerInformation = false;
		return;
	}

	if (!response->isSucceed()) {
		CCLog("server get info response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		_loadedServerInformation = false;
		return;
	}

	std::string resp = Tools::toString(response->getResponseData());
	std::string basewarflag = "<basewarQueueInfo>";
	if (resp.compare(0, basewarflag.size(), basewarflag) != 0)
		return; //fail script

	resp.erase(0, resp.find('\n') + 1); //remove first line with server info flag

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float padding = 10 * scaleMult;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float button_heigth = 66 * scaleMult;
	_serverInformation->setString(resp.c_str());
	_serverInformation->setPosition(
			ccp(winSize.width / 2, winSize.height / 2 + (button_heigth / 2 + padding + _serverInformation->getContentSize().height / 2)));
}

void NetworkQueueLayer::queuePart() {
	if (_sendingRequest)
		return;

	ServerCommunication sc = ServerCommunication();
	if (!_loadedServerInformation) {
		_loadedServerInformation = true;
		sc.getServerinformation(this, httpresponse_selector(NetworkQueueLayer::onGetServerInfoCompleted));
	}

	if (!_loadedPlayerNumInformation) {
		_loadedPlayerNumInformation = true;
		sc.getPlayerNum(this, httpresponse_selector(NetworkQueueLayer::onPlayerNumInfoCompleted));
	}

	if (!_loadedTeamsizeConfig) {
		_loadedTeamsizeConfig = true;
		sc.getTeamsizeConfig(this, httpresponse_selector(NetworkQueueLayer::onTeamsizeConfigCompleted));
	}

	//we reset the queued time, do we will not have an invalid seconds in queue time
	if (_maxSecToStart == -1)
		_queueTimeInSec = BW_Time::getSeconds();

	//first request is send 5 seconds after we joined (and first server contact)
	if (BW_Time::getSeconds() - _queueTimeInSec < 5) {
		if (_maxSecToStart != -1) {
			completeQueueRequest(0, Tools::toString(BW_Time::getSeconds() - _queueTimeInSec));
		}
		return;
	}

	_sendingRequest = true;
	_send_time = BW_Time::getMilliSeconds();
	sc.queueMe(_raceId, _playerName, _randomRaceString, this, httpresponse_selector(NetworkQueueLayer::onQueueCompleted));
}

void NetworkQueueLayer::leaveQueueTouched(CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}
void NetworkQueueLayer::setPlayerName(std::string playerName) {
	_playerName = playerName;
}
void NetworkQueueLayer::setRaceId(int raceId) {
	_raceId = raceId;
	_randomRaceString = "";
	if (_raceId == 0) {
		_randomRaceString = Race::createRandomizedRaceString();
	}
}

std::vector<std::string> NetworkQueueLayer::getFakePlayerNames() {
	std::vector < std::string > fakePlayerName;

	//add some self defined names
	fakePlayerName.push_back("Rok");
	fakePlayerName.push_back("Rock");
	fakePlayerName.push_back("Shoroa");
	fakePlayerName.push_back("Töpfchen");

	//add names from a generator
	fakePlayerName.push_back("Doozmolt");
	fakePlayerName.push_back("Gunkumb");
	fakePlayerName.push_back("Froof");
	fakePlayerName.push_back("Froob");
	fakePlayerName.push_back("Glunphball");
	fakePlayerName.push_back("Clunuzz");
	fakePlayerName.push_back("Gogwook");
	fakePlayerName.push_back("Womph");
	fakePlayerName.push_back("Punt");
	fakePlayerName.push_back("Kuckbun");
	fakePlayerName.push_back("Gobult");
	fakePlayerName.push_back("Donkpidiot");
	fakePlayerName.push_back("Porkfrub");
	fakePlayerName.push_back("Jumfelch");
	fakePlayerName.push_back("Poltmorm");
	fakePlayerName.push_back("Glunt");
	fakePlayerName.push_back("Bookdip");
	fakePlayerName.push_back("Klomphnump");
	fakePlayerName.push_back("Frelchung");
	fakePlayerName.push_back("Cloron");
	fakePlayerName.push_back("Wongfumph");
	fakePlayerName.push_back("Jidiotgunph");
	fakePlayerName.push_back("Gobglunk");
	fakePlayerName.push_back("Donkun");
	fakePlayerName.push_back("Doofmonk");
	fakePlayerName.push_back("Blokjog");
	fakePlayerName.push_back("Blumphgoof");
	fakePlayerName.push_back("Thobclown");
	fakePlayerName.push_back("Brolt");
	fakePlayerName.push_back("Porkjunb");
	fakePlayerName.push_back("Funph");
	fakePlayerName.push_back("Pobbeef");
	fakePlayerName.push_back("Gunnit");
	fakePlayerName.push_back("Kubfrorm");
	fakePlayerName.push_back("Hunphthimble");
	fakePlayerName.push_back("Grubbrok");
	fakePlayerName.push_back("Bumbdidiot");
	fakePlayerName.push_back("Gunankle");
	fakePlayerName.push_back("Nuck");
	fakePlayerName.push_back("Dobong");
	fakePlayerName.push_back("Bump");
	fakePlayerName.push_back("Borkmuck");
	fakePlayerName.push_back("Kult");
	fakePlayerName.push_back("Doowad");
	fakePlayerName.push_back("Ghoog");
	fakePlayerName.push_back("Nonkdip");
	fakePlayerName.push_back("Gob");
	fakePlayerName.push_back("Puzz");
	fakePlayerName.push_back("Fuck");
	fakePlayerName.push_back("Dulfclot");
	fakePlayerName.push_back("Grunkum");
	fakePlayerName.push_back("Bunkghub");
	fakePlayerName.push_back("Ghunkook");
	fakePlayerName.push_back("Pormbeef");
	fakePlayerName.push_back("Ghomph");
	fakePlayerName.push_back("Clumbob");
	fakePlayerName.push_back("Dultbridiot");
	fakePlayerName.push_back("Jolph");
	fakePlayerName.push_back("Fomph");
	fakePlayerName.push_back("Pob");
	fakePlayerName.push_back("Gunbloaf");
	fakePlayerName.push_back("Grorg");
	fakePlayerName.push_back("Klok");
	fakePlayerName.push_back("Porgoo");
	fakePlayerName.push_back("Thelchgog");
	fakePlayerName.push_back("Mulfflolt");
	fakePlayerName.push_back("Porgknock");
	fakePlayerName.push_back("Moob");
	fakePlayerName.push_back("Hoobomph");
	fakePlayerName.push_back("Huck");
	fakePlayerName.push_back("Mummunb");
	fakePlayerName.push_back("Puzz");
	fakePlayerName.push_back("Klumbdooz");
	fakePlayerName.push_back("Fonk");
	fakePlayerName.push_back("Mok");
	fakePlayerName.push_back("Dolphpolph");
	fakePlayerName.push_back("Flolt");
	fakePlayerName.push_back("Pook");
	fakePlayerName.push_back("Florkporon");
	fakePlayerName.push_back("Clok");
	fakePlayerName.push_back("Gunph");
	fakePlayerName.push_back("Flonk");
	fakePlayerName.push_back("Didiot");
	fakePlayerName.push_back("Clookgoof");
	fakePlayerName.push_back("Moron");
	fakePlayerName.push_back("Thugtwit");
	fakePlayerName.push_back("Fumb");
	fakePlayerName.push_back("Dolph");
	fakePlayerName.push_back("Thumb");
	fakePlayerName.push_back("Moohead");
	fakePlayerName.push_back("Blok");
	fakePlayerName.push_back("Koogfumble");
	fakePlayerName.push_back("Flob");
	fakePlayerName.push_back("Kub");
	fakePlayerName.push_back("Dok");
	fakePlayerName.push_back("Klunph");
	fakePlayerName.push_back("Midiot");
	fakePlayerName.push_back("Brubankle");
	fakePlayerName.push_back("Bonk");
	fakePlayerName.push_back("Mumpface");
	fakePlayerName.push_back("Pulf");
	fakePlayerName.push_back("Tholphknuckle");
	fakePlayerName.push_back("Dunphdip");
	fakePlayerName.push_back("Fuck");
	fakePlayerName.push_back("Ghunbum");
	fakePlayerName.push_back("Nuntgong");
	fakePlayerName.push_back("Fumb");
	fakePlayerName.push_back("Mongborm");
	fakePlayerName.push_back("Floronbrumb");
	fakePlayerName.push_back("Clorgdumb");
	fakePlayerName.push_back("Ponkknock");
	fakePlayerName.push_back("Clunoof");
	fakePlayerName.push_back("Buboo");
	fakePlayerName.push_back("Jidiotgluzz");
	fakePlayerName.push_back("Fumphbult");
	fakePlayerName.push_back("Borgug");
	fakePlayerName.push_back("Muzz");
	fakePlayerName.push_back("Bung");
	fakePlayerName.push_back("Book");
	fakePlayerName.push_back("Rynot");
	fakePlayerName.push_back("Modup");
	fakePlayerName.push_back("Xofili");
	fakePlayerName.push_back("Hinserererwor");
	fakePlayerName.push_back("Achsertorech");
	fakePlayerName.push_back("Naldyn");
	fakePlayerName.push_back("Emelm");
	fakePlayerName.push_back("Hiwar");
	fakePlayerName.push_back("Kimvia");
	fakePlayerName.push_back("Mostai");
	fakePlayerName.push_back("Imlorrad");
	fakePlayerName.push_back("Steyz");
	fakePlayerName.push_back("Hipolaugh");
	fakePlayerName.push_back("Warbom");
	fakePlayerName.push_back("Trandtin");
	fakePlayerName.push_back("Eldkel");
	fakePlayerName.push_back("Ormiss");
	fakePlayerName.push_back("Colen");
	fakePlayerName.push_back("Veryis");
	fakePlayerName.push_back("Ardnque");
	fakePlayerName.push_back("Kinnsul");
	fakePlayerName.push_back("Lyedralye");
	fakePlayerName.push_back("Quedusk");
	fakePlayerName.push_back("Schoath");
	fakePlayerName.push_back("Cesoh");
	fakePlayerName.push_back("Iaro");
	fakePlayerName.push_back("Lerkin");
	fakePlayerName.push_back("Ineadves");
	fakePlayerName.push_back("Chryorran");
	fakePlayerName.push_back("Anysu");
	fakePlayerName.push_back("Endean");
	fakePlayerName.push_back("Timan");
	fakePlayerName.push_back("Whialeen");
	fakePlayerName.push_back("Untind");
	fakePlayerName.push_back("Poiqche");
	fakePlayerName.push_back("Iaez");
	fakePlayerName.push_back("Ranl");
	fakePlayerName.push_back("Eldalemor");
	fakePlayerName.push_back("Tiald");
	fakePlayerName.push_back("Telyecer");
	fakePlayerName.push_back("Rodeld");
	fakePlayerName.push_back("Yvory");
	fakePlayerName.push_back("Blitor");
	fakePlayerName.push_back("Zebese");
	fakePlayerName.push_back("Naertough");
	fakePlayerName.push_back("Emche");
	fakePlayerName.push_back("Ittor");
	fakePlayerName.push_back("Radrako");
	fakePlayerName.push_back("Lerrothper");
	fakePlayerName.push_back("Cheor");
	fakePlayerName.push_back("Drianan");
	fakePlayerName.push_back("Atidel");
	fakePlayerName.push_back("Ustia");
	fakePlayerName.push_back("Untther");
	fakePlayerName.push_back("Sulwor");
	fakePlayerName.push_back("Ranris");
	fakePlayerName.push_back("Fotad");
	fakePlayerName.push_back("Ranbel");
	fakePlayerName.push_back("Essgar");
	fakePlayerName.push_back("Lyeip");
	fakePlayerName.push_back("Rodol");
	fakePlayerName.push_back("Danest");
	fakePlayerName.push_back("Yuemest");
	fakePlayerName.push_back("Adado");
	fakePlayerName.push_back("Enrothdar");
	fakePlayerName.push_back("Darzton");
	fakePlayerName.push_back("Rayet");
	fakePlayerName.push_back("Agehwar");
	fakePlayerName.push_back("Vorald");
	fakePlayerName.push_back("Engissard");
	fakePlayerName.push_back("Burtonine");
	fakePlayerName.push_back("Nalbur");
	fakePlayerName.push_back("Quetorton");
	fakePlayerName.push_back("Chab");
	fakePlayerName.push_back("Engem");
	fakePlayerName.push_back("Dradarmor");
	fakePlayerName.push_back("Dentasa");
	fakePlayerName.push_back("Aughirray");
	fakePlayerName.push_back("Bataf");
	fakePlayerName.push_back("Ilbani");
	fakePlayerName.push_back("Urnn");
	fakePlayerName.push_back("Titser");
	fakePlayerName.push_back("Theusmos");
	fakePlayerName.push_back("Aldack");
	fakePlayerName.push_back("Estunti");
	fakePlayerName.push_back("Untangsam");
	fakePlayerName.push_back("Serach");
	fakePlayerName.push_back("Lergkin");
	fakePlayerName.push_back("Zorary");
	fakePlayerName.push_back("Rasec");
	fakePlayerName.push_back("Burset");
	fakePlayerName.push_back("Querothrak");
	fakePlayerName.push_back("Swyves");
	fakePlayerName.push_back("Kindel");
	fakePlayerName.push_back("Enat");
	fakePlayerName.push_back("Urnade");
	fakePlayerName.push_back("Athine");
	fakePlayerName.push_back("Serozy");
	fakePlayerName.push_back("Ineone");
	fakePlayerName.push_back("Rayelmmorechdyn");
	fakePlayerName.push_back("Iatane");
	fakePlayerName.push_back("Worona");
	fakePlayerName.push_back("Lathunt");
	fakePlayerName.push_back("Untalt");
	fakePlayerName.push_back("Ankel");
	fakePlayerName.push_back("Ghagom");
	fakePlayerName.push_back("Waraldiss");
	fakePlayerName.push_back("Marosian");
	fakePlayerName.push_back("Lisirrasya");
	fakePlayerName.push_back("Sondilsanya");
	fakePlayerName.push_back("Lisadar");
	fakePlayerName.push_back("Marirrala");
	fakePlayerName.push_back("Sondim");
	fakePlayerName.push_back("Chrundosia");
	fakePlayerName.push_back("Lassosia");
	fakePlayerName.push_back("Iskossa");
	fakePlayerName.push_back("Lassosiaya");
	fakePlayerName.push_back("Chanephi");
	fakePlayerName.push_back("Chanirraya");
	fakePlayerName.push_back("Frichilsanya");
	fakePlayerName.push_back("Chamilsan");
	fakePlayerName.push_back("Iskista");
	fakePlayerName.push_back("Assast");
	fakePlayerName.push_back("Chamilsa");
	fakePlayerName.push_back("Assilsa");
	fakePlayerName.push_back("Chadossasta");
	fakePlayerName.push_back("Frichosia");
	fakePlayerName.push_back("Iskastnya");
	fakePlayerName.push_back("Chadim");
	fakePlayerName.push_back("Lirtassa");
	fakePlayerName.push_back("Iladar");
	fakePlayerName.push_back("Doistiman");
	fakePlayerName.push_back("Sondazoan");
	fakePlayerName.push_back("Lisofe");
	fakePlayerName.push_back("Sondirra");
	fakePlayerName.push_back("Chadosia");
	fakePlayerName.push_back("Sundassa");
	fakePlayerName.push_back("Lisossanya");
	fakePlayerName.push_back("Lassosiasda");
	fakePlayerName.push_back("Frichassa");
	fakePlayerName.push_back("Marossa");
	fakePlayerName.push_back("Sundastya");
	fakePlayerName.push_back("Mindossa");
	fakePlayerName.push_back("Chanistanya");
	fakePlayerName.push_back("Jiskjask");
	fakePlayerName.push_back("Sundim");
	fakePlayerName.push_back("Assjasksya");
	fakePlayerName.push_back("Lisushi");
	fakePlayerName.push_back("Lirtjask");
	fakePlayerName.push_back("Chanisi");
	fakePlayerName.push_back("Jiskadarsda");
	fakePlayerName.push_back("Undim");
	fakePlayerName.push_back("Jiskosia");
	fakePlayerName.push_back("Lirtilsasta");
	fakePlayerName.push_back("Loidinna");
	fakePlayerName.push_back("Sondossa");
	fakePlayerName.push_back("Assassa");
	fakePlayerName.push_back("Assosia");
	fakePlayerName.push_back("Chamirrasda");
	fakePlayerName.push_back("Sundista");
	fakePlayerName.push_back("Lisilsan");
	fakePlayerName.push_back("Lisassasda");
	fakePlayerName.push_back("Chadassa");
	fakePlayerName.push_back("Serun");
	fakePlayerName.push_back("Ormptor");
	fakePlayerName.push_back("Blon");
	fakePlayerName.push_back("Vorban");
	fakePlayerName.push_back("Ashed");
	fakePlayerName.push_back("Toit");
	fakePlayerName.push_back("Enthbur");
	fakePlayerName.push_back("Inasul");
	fakePlayerName.push_back("Zhiwor");
	fakePlayerName.push_back("Ardis");
	fakePlayerName.push_back("Teer");
	fakePlayerName.push_back("Nyath");
	fakePlayerName.push_back("Lyetorine");
	fakePlayerName.push_back("Etther");
	fakePlayerName.push_back("Umvor");
	fakePlayerName.push_back("Morth");
	fakePlayerName.push_back("Polmser");
	fakePlayerName.push_back("Morlkim");
	fakePlayerName.push_back("Verurnlor");
	fakePlayerName.push_back("Therrod");
	fakePlayerName.push_back("Isstora");
	fakePlayerName.push_back("Honr");
	fakePlayerName.push_back("Aoso");
	fakePlayerName.push_back("Worz");
	fakePlayerName.push_back("Aechu");
	fakePlayerName.push_back("Tozefa");
	fakePlayerName.push_back("Rilawim");
	fakePlayerName.push_back("Charilhin");
	fakePlayerName.push_back("Aldcem");
	fakePlayerName.push_back("Ormale");
	fakePlayerName.push_back("Beust");
	fakePlayerName.push_back("Gilypa");
	fakePlayerName.push_back("Hindcha");
	fakePlayerName.push_back("Brasam");
	fakePlayerName.push_back("Belnys");
	fakePlayerName.push_back("Degar");
	fakePlayerName.push_back("Elmir");
	fakePlayerName.push_back("Hycerale");
	fakePlayerName.push_back("Poly");
	fakePlayerName.push_back("Yeir");
	fakePlayerName.push_back("Yuigshy");
	fakePlayerName.push_back("Cherodi");
	fakePlayerName.push_back("Ghancer");
	fakePlayerName.push_back("Smaphusk");
	fakePlayerName.push_back("Athoem");
	fakePlayerName.push_back("Rynul");
	fakePlayerName.push_back("Enyer");
	fakePlayerName.push_back("Ormos");
	fakePlayerName.push_back("Tociru");
	fakePlayerName.push_back("Smyeld");
	fakePlayerName.push_back("Rynas");
	fakePlayerName.push_back("Getisa");
	fakePlayerName.push_back("Aise");
	fakePlayerName.push_back("Inghon");
	fakePlayerName.push_back("Chack");
	fakePlayerName.push_back("Quemorest");
	fakePlayerName.push_back("Echit");
	fakePlayerName.push_back("Oschebel");
	fakePlayerName.push_back("Tiad");
	fakePlayerName.push_back("Estver");
	fakePlayerName.push_back("Skel-moryg");
	fakePlayerName.push_back("Itqua");
	fakePlayerName.push_back("Heegden");
	fakePlayerName.push_back("Untos");
	fakePlayerName.push_back("Ryage");
	fakePlayerName.push_back("Nobyz");
	fakePlayerName.push_back("Rakan");
	fakePlayerName.push_back("Usken");
	fakePlayerName.push_back("Garorm");
	fakePlayerName.push_back("Slony");
	fakePlayerName.push_back("Atiao");
	fakePlayerName.push_back("Inala");
	fakePlayerName.push_back("Bekah");
	fakePlayerName.push_back("Deathmor");
	fakePlayerName.push_back("Angvgha");
	fakePlayerName.push_back("Nystas");
	fakePlayerName.push_back("Voros");
	fakePlayerName.push_back("Hatrayold");
	fakePlayerName.push_back("Rilald");
	fakePlayerName.push_back("Driesar");
	fakePlayerName.push_back("Tanhini");
	fakePlayerName.push_back("Ackturu");
	fakePlayerName.push_back("Athrisi");
	fakePlayerName.push_back("Yosu");
	fakePlayerName.push_back("Auskochae");
	fakePlayerName.push_back("Koardris");
	fakePlayerName.push_back("Eelma");
	fakePlayerName.push_back("Endkel");
	fakePlayerName.push_back("Angdan");
	fakePlayerName.push_back("Tenag");
	fakePlayerName.push_back("Issdtia");
	fakePlayerName.push_back("Awtintia");
	fakePlayerName.push_back("Athuntther");
	fakePlayerName.push_back("Swurrlt");
	fakePlayerName.push_back("Stokin");
	fakePlayerName.push_back("Alen");
	fakePlayerName.push_back("Ghaend");
	fakePlayerName.push_back("Imagea");
	fakePlayerName.push_back("Ighte");
	fakePlayerName.push_back("Kelgtai");
	fakePlayerName.push_back("Vorril");
	fakePlayerName.push_back("Ranag");
	fakePlayerName.push_back("Hinart");
	fakePlayerName.push_back("Morer");
	fakePlayerName.push_back("Snaightser");
	fakePlayerName.push_back("Adchabel");
	fakePlayerName.push_back("Erbanis");
	fakePlayerName.push_back("Untris");
	fakePlayerName.push_back("Rothem");
	fakePlayerName.push_back("Aswage");

	return fakePlayerName;
}
