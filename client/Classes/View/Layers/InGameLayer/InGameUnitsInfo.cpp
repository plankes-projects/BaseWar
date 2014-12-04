#include "InGameUnitsInfo.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Model/Model.h"
#include "../../../Constants.h"

using namespace cocos2d;

bool InGameUnitsInfo::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 100, 255, 255)))
		return false;
	this->setTouchEnabled(true);
	//setOpacity(0); // 0- total transparent, 255 not
	_buttonYPosition = 0;
	_infoRowOffsetX = 0;
	_infoRowOffsetY = 10;

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_menuItemWidth = 66 * scaleMult;
	_menuItemHeigth = 66 * scaleMult;
	_padding = 10 * scaleMult;
	_speed = 600 * scaleMult;

	_unitFactoryPos = 0;

	CCMenu* menu = CCMenu::create();

	//iterate all players

	std::list<Player*> players = Model::getInstance()->getMyArmy(LEFT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		addUnitInfosForPlayer(menu, *it, LEFT);
	}

	players = Model::getInstance()->getMyArmy(RIGHT)->getPlayers();
	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
		addUnitInfosForPlayer(menu, *it, RIGHT);
	}

	addButtonInfo(menu);

	float boarderPadding = UNITS_INFO_OVERLAY_WIDTH * scaleMult;
	float width = 3 * (_menuItemWidth + _padding);
	float height = (_buttonYPosition + 1 - _buttonYPositionCorrectionSpaceForPlayerName) * (_menuItemHeigth + _padding);
	menu->setContentSize(CCSize(width + boarderPadding, height + 2 * boarderPadding));
	menu->setTouchPriority(-20000);
	addChild(menu);

	float spaceForFingerScroll = 0 * scaleMult;
	if (menu->getContentSize().height < winSize.height) {
		menu->setPosition(ccp(boarderPadding, winSize.height - 1 * _menuItemHeigth));
		setContentSize(CCSize(menu->getContentSize().width + spaceForFingerScroll, winSize.height));
	} else {
		menu->setPosition(ccp(boarderPadding, menu->getContentSize().height - 1 * _menuItemHeigth));
		setContentSize(CCSize(menu->getContentSize().width + spaceForFingerScroll, menu->getContentSize().height));
	}

	_shownLocation = ccp(winSize.width - getContentSize().width, -(getContentSize().height - winSize.height));
	_hiddenLocation = ccp(winSize.width - boarderPadding, -(getContentSize().height - winSize.height));

	setPosition(_hiddenLocation);
	_menuIsHidden = true;
	setOpacity(200);
	return true;
}

void InGameUnitsInfo::addButtonInfo(CCMenu* menu) {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	CCLabelTTF *info = CCLabelTTF::create("Button info:", FONT_NAME, 25 * scaleMult);
	info->setColor(ccc3(0, 0, 0));
	CCMenuItemLabel *menuItemWrapper = CCMenuItemLabel::create(info);
	menuItemWrapper->setDisabledColor(ccc3(0, 0, 0));
	menuItemWrapper->setEnabled(false);

	float x_name_offset = _infoRowOffsetX * scaleMult;
	float x_name = info->getContentSize().width / 2;
	float y_name_offset = _infoRowOffsetY * scaleMult;
	float y_name = _menuItemHeigth / 2 - (_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName) * (_menuItemHeigth + _padding);
	menuItemWrapper->setPosition(CCPoint(x_name + x_name_offset, y_name + y_name_offset));
	menu->addChild(menuItemWrapper);

	_buttonYPosition++; //this one is for player name
	_buttonYPositionCorrectionSpaceForPlayerName += 0.5f;

	int x = 0;

	cocos2d::CCMenuItemSprite* unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("AttackUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("AttackUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::attackInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ArmourUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ArmourUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::armorInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("HitpointsUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("HitpointsUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::healthInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	_buttonYPosition++;
	x = 0;

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("RunSpeedUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("RunSpeedUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::runInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("SkillUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("SkillUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::skillInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ManaUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ManaUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::manaInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);


	_buttonYPosition++;
	x = 0;


	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ManaRegUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("ManaRegUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::manaRegInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);

	unitButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("RangeUpgrade1.png"),
			CCSprite::createWithSpriteFrameName("RangeUpgrade2.png"), (CCObject*) this, menu_selector(InGameUnitsInfo::rangeInfoTouched));
	ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
			-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);
}

void InGameUnitsInfo::hideAnimation() {
	float y = getPosition().y;
	float distance = BW_Point(this->getPosition().x, y).distanceTo(BW_Point(_hiddenLocation.x, y));

	this->stopAllActions();
	this->runAction(CCSequence::create(CCMoveTo::create(distance / _speed, ccp(_hiddenLocation.x, y)), NULL));

	_menuIsHidden = true;
}

void InGameUnitsInfo::showAnimation() {
	float y = getPosition().y;
	float distance = BW_Point(this->getPosition().x, y).distanceTo(BW_Point(_shownLocation.x, y));

	this->stopAllActions();
	this->runAction(CCSequence::create(CCMoveTo::create(distance / _speed, ccp(_shownLocation.x, y)), NULL));

	_menuIsHidden = false;
}

void InGameUnitsInfo::addUnitInfosForPlayer(CCMenu* menu, Player* player, ArmyType side) {
	if(player->getRace()->isSkipUnitInfo())
		return;

	CCLOG("Creating unit info for player %s", player->getName().c_str());

	int x = 0;
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();

	//add the players name
	std::string pn = player->getName();
	if (Model::getInstance()->getGameMode() == NETWORK) {
		std::string prefix = side == LEFT ? "Left: " : "Right: ";
		pn = prefix + pn;
	}

	CCLabelTTF *playername = CCLabelTTF::create(pn.c_str(), FONT_NAME, 25 * scaleMult);
	playername->setColor(ccc3(0, 0, 0));
	CCMenuItemLabel *menuItemWrapper = CCMenuItemLabel::create(playername);
	menuItemWrapper->setDisabledColor(ccc3(0, 0, 0));
	menuItemWrapper->setEnabled(false);

	float x_name_offset = _infoRowOffsetX * scaleMult;
	float x_name = playername->getContentSize().width / 2;
	float y_name_offset = _infoRowOffsetY * scaleMult;
	float y_name = _menuItemHeigth / 2 - (_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName) * (_menuItemHeigth + _padding);
	menuItemWrapper->setPosition(CCPoint(x_name + x_name_offset, y_name + y_name_offset));
	menu->addChild(menuItemWrapper);

	_buttonYPosition++; //this one is for player name
	_buttonYPositionCorrectionSpaceForPlayerName += 0.5f;

	Race* race = player->getRace();

	//iterate all tiers
	for (int tier = Tier1; tier <= Tier3; tier++) {
		std::vector<UnitFactory*> factories = race->getUnitFactories((Tier) tier);
		//iterate all factories
		for (std::vector<UnitFactory*>::iterator it = factories.begin(); it != factories.end(); ++it) {

			UnitFactory* unitFactory = *it;
			cocos2d::CCMenuItemSprite* unitButton = CCMenuItemSprite::create(
					CCSprite::createWithSpriteFrameName(unitFactory->getNormalButtonImage()),
					CCSprite::createWithSpriteFrameName(unitFactory->getSelectedButtonImage()), (CCObject*) this,
					menu_selector(InGameUnitsInfo::unitInfoTouched));

			ViewTools::addToMenu(menu, unitButton, _menuItemWidth, _menuItemHeigth, x++,
					-(_buttonYPosition - _buttonYPositionCorrectionSpaceForPlayerName), _padding, -20000);
			unitButton->setTag(_unitFactoryPos++); //dont move this, we access it with index at touch event
			allFactories.push_back(unitFactory);
		}
		_buttonYPosition++;
		x = 0;
	}
}

void InGameUnitsInfo::unitInfoTouched(cocos2d::CCObject* pSender) {

	int i = 0;
	int tag = ((CCMenuItemSprite*) pSender)->getTag();
	for (std::vector<UnitFactory*>::iterator it = allFactories.begin(); it != allFactories.end(); ++it) {
		if (i++ == tag) {
			ViewTools::showUnitInfo(*it, getParent());
			return;
		}
	}
}

void InGameUnitsInfo::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

bool InGameUnitsInfo::ccTouchBegan(CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

	initialTouch = pTouch->getLocation();
	initialPos = getPosition();

	BW_Rect layer = BW_Rect(initialPos.x, initialPos.y, getContentSize().width, getContentSize().height);
	return layer.contains(BW_Point(initialTouch.x, initialTouch.y)); //return true if we touch this layer --> hadnle the touch event
}

void InGameUnitsInfo::ccTouchMoved(CCTouch * pTouch, CCEvent* event) {
	//we move the full layer here
	_menuIsHidden = false;

	float y = initialPos.y + (pTouch->getLocation().y - initialTouch.y);
	float x = initialPos.x + (pTouch->getLocation().x - initialTouch.x);

	float max_y = 0;
	float min_y = CCDirector::sharedDirector()->getWinSize().height - getContentSize().height;

	if (y > max_y)
		y = max_y;
	else if (y < min_y)
		y = min_y;

	float min_x = _shownLocation.x;
	if (x < min_x)
		x = min_x;

	setPosition(x, y);
}

void InGameUnitsInfo::ccTouchEnded(CCTouch * pTouch, CCEvent* event) {
	float max = _hiddenLocation.x - _shownLocation.x;
	float current = getPosition().x - _shownLocation.x;

	if (current <= max / 2) {
		showAnimation();
	} else {
		hideAnimation();
	}
}
