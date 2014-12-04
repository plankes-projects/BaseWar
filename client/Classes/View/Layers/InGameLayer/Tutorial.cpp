#include "Tutorial.h"
#include "../../CCAlertView.h"
#include "../../../Constants.h"
#include "../../../Tools/Tools.h"
#include "../../../Model/Model.h"
#include "../../../Model/Player.h"
#include "../../../Model/Units/Race.h"
#include "../../../Model/Units/UnitFactory.h"
using namespace cocos2d;

Tutorial::Tutorial(CCNode* scene) {
	_scene = scene;
}

void Tutorial::gameFieldTut() {
	std::string content = ""
		"This is the gamefield.\n \n"
		"Your base is always to the left. You have to protect it. If the enemy destroys your base, you lose the game.\n"
		"Therefore, to win, your goal is to destroy the base of your enemy.";
	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::TowerKillInfo), true);

	_scene->addChild(alert, 100);
}

void Tutorial::TowerKillInfo(cocos2d::CCObject* pSender) {
	std::string content = ""
		"The tower.\n \n"
		"You most likely recognized the two towers. You will get a reward of ";
	content += Tools::toString(GOLD_ON_TOWER_KILL) + " gold if you destroy your enemies tower.\n \n"
		"So... maybe you should not lose yours...";
	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::MiddleControlInfo), true);

	_scene->addChild(alert, 100);
}

void Tutorial::MiddleControlInfo(cocos2d::CCObject* pSender) {

	std::string content = ""
		"Controlling the middle.\n \n"
		"There is also a reward for controlling the middle.\n"
		"The player with the units closer to the opponents base will get " + Tools::toString((int) ((GOLD_FOR_MIDDLE_CONTROL-0.99) * 100)) + "% additional gold.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::goldInfo), true);

	_scene->addChild(alert, 100);
}

void Tutorial::goldInfo(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "Wondering about the gold?\n \n";
	content += "Your units will not fight for nothing...\n";
	content += "They only join your army if you hire them with gold. Once hired, they will come every round again!";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::SpawnInfo), true);

	_scene->addChild(alert, 100);
}

void Tutorial::SpawnInfo(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "Units\n \n";
	content += "At the lower left corner there is a menu button.\n \n";
	content += "Press it.\n";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");

	_scene->addChild(alert, 100);
	delete this;
}

void Tutorial::unitMenuInfo() {

	std::string content = ""
		"Unit menu\n \n"
		"In this menu you see all your units and some other purchasable items.\n"
		"There is the gold upgrade button: It will give you " + Tools::toString((int) ((GOLD_FOR_GOLD_UPGRADE-0.99) * 100));
	content += "% additional gold per upgrade.\n"
		"And there is the tier unlock button: It unlocks the next row of units.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::unitMenuInfo2), true);

	_scene->addChild(alert, 100);
}

void Tutorial::unitMenuInfo2(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "Unit menu\n \n";
	content += "But for now we just want to buy a unit.\n \n";
	content += "Press at a unit button.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");

	_scene->addChild(alert, 100);
	delete this;
}

void Tutorial::unitInfo() {

	//buy one unit of each tier1 for enemy
	Army* army = Model::getInstance()->getEnemyArmy(Model::getInstance()->getThisPlayer()->getArmyTyp());
	Player* player = army->getPlayers().front();
	std::vector<UnitFactory*> unitFacts = player->getRace()->getUnitFactories(Tier1);
	for (std::vector<UnitFactory*>::iterator it = unitFacts.begin(); it != unitFacts.end(); ++it)
		(*it)->addOneUnit();

	//buy one unit of each tier1 for you
	army = Model::getInstance()->getMyArmy(Model::getInstance()->getThisPlayer()->getArmyTyp());
	player = army->getPlayers().front();
	unitFacts = player->getRace()->getUnitFactories(Tier1);
	for (std::vector<UnitFactory*>::iterator it = unitFacts.begin(); it != unitFacts.end(); ++it)
		(*it)->addOneUnit();

	std::string content = "";
	content += "Unit\n \n";
	content += "This is the menu where you are able to buy, sell or even upgrade your units.\n";
	content += "The second and third row are upgrades. They will apply to all units of this kind which will enter the battlefield.\n \n";
	content += "To speed things up, I already bought some units for you.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::unitInfo2), true);

	_scene->addChild(alert, 100);
}

void Tutorial::unitInfo2(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "Unit\n \n";
	content += "It would be nice to see how each unit performs, eg. how much life, damage, ... they have.\n";
	content += "For this, you may press the button with the unit image.\n \n";
	content += "Press it.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");

	_scene->addChild(alert, 100);
	delete this;
}

void Tutorial::afterUnitInfoWatched(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "Unit\n \n";
	content += "The same information is available at the information menu.\n";
	content += "Additionally there is a list of buttons at the bottom of this menu to get information about the upgrade buttons.\n\n";
	content += "Just wish in the blue bar to the right.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::afterUnitInfoWatched2), true);

	_scene->addChild(alert, 100);
}

void Tutorial::afterUnitInfoWatched2(cocos2d::CCObject* pSender) {

	std::string content = "";
	content += "The battle\n \n";
	content += "At this point you maybe wonder yourself what the heck is going on on the battlefield.\n";
	content += "Every time when the decreasing timer at the top left corner hits the 0, all your bought units will spawn again.\n";
	content += "There is already some action ongoing, but we are way too far away to watch it.";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::TopMenuInfo), true);

	_scene->addChild(alert, 100);
}

void Tutorial::TopMenuInfo(cocos2d::CCObject* pSender) {
	std::string content = "";
	content += "Cam control\n \n";
	content += "At the top right corner you are able to see some buttons. ";
	content += "They are for camera control.\n";
	content += "The first two are for zooming, the next two are for turning on/off the automatic camera ";
	content += "and for switching the automatic camera target.\n";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok", this, callfuncO_selector(Tutorial::tutorialEndMessage), true);

	_scene->addChild(alert, 100);
}

void Tutorial::tutorialEndMessage(cocos2d::CCObject* pSender) {
	std::string content = "";
	content += "Defeat your enemy\n \n";
	content += "Now try to defeat your enemy. This should not be very hard at the tutorial ";
	content += "because the enemy will not get more units.\n";
	content += "For more fun play versus the computer or enter an internet game.\n";

	CCAlertView *alert = CCAlertView::create("Tutorial", content.c_str(), "Ok");

	_scene->addChild(alert, 100);
	delete this;
}

