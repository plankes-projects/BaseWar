/*
 * Tools.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "ViewTools.h"
#include "../Model/Attacks/Attack.h"
#include "../Model/Armours/Armour.h"
#include "../Model/Model.h"
#include "../View/Layers/InGameLayer/Tutorial.h"
#include "../View/CCConfirmView.h"
#include "Tools.h"
#include "BW_Time.h"
#include "../View/CCAlertView.h"
#include "../Constants.h"
#include "../Model/Units/Pinkie.h"
#include "../Model/Attacks/InstantAttack.h"
#include "../Model/DamageType.h"
#include "../AchievementSystem/AchievementTutorialFinished.h"

void ViewTools::resizeMenu(CCMenu* menu, float menuItemWidth, float menuItemHeigth, int anzx, int anzy, float padding, float boarderPadding) {

	float width = anzx * (menuItemWidth + padding);
	float height = anzy * (menuItemHeigth + padding);

	menu->setContentSize(CCSize(width + 2 * boarderPadding, height + 2 * boarderPadding));
}
float ViewTools::addToMenu(CCMenu* menu, CCNode* menuItem, float menuItemWidth, float menuItemHeigth, float posx, float posy, float padding, int prio) {

	menuItem->setScaleX(menuItemWidth / menuItem->getContentSize().width);
	menuItem->setScaleY(menuItemHeigth / menuItem->getContentSize().height);

	float x = menuItemWidth / 2 + posx * (menuItemWidth + padding);
	float y = menuItemHeigth / 2 + posy * (menuItemHeigth + padding);

	menuItem->setPosition(CCPoint(x, y));

	menu->addChild(menuItem, prio);
	return menuItemHeigth / menuItem->getContentSize().height;
}

bool ViewTools::isGameFieldSpriteOnScreen(CCSprite* sprite) {
	InGameGameLayer* layer = (InGameGameLayer*) sprite->getParent();

	BW_Rect shownRec = layer->getShownRect();
	shownRec.leftDown.x -= sprite->getContentSize().width * sprite->getScale() / 2 * layer->getScale();
	shownRec.leftDown.y -= sprite->getContentSize().height * sprite->getScale() / 2 * layer->getScale();
	shownRec.rightUp.x += sprite->getContentSize().width * sprite->getScale() / 2 * layer->getScale();
	shownRec.rightUp.y += sprite->getContentSize().height * sprite->getScale() / 2 * layer->getScale();

	return shownRec.contains(BW_Point(sprite->getPosition().x, sprite->getPosition().y));

}

ViewTools::ViewTools() {

}

ViewTools::~ViewTools() {
}


CCNode* ViewTools::createNotUnlockedMessage(int neededAchievementPoints){
	std::string body = Tools::toString(neededAchievementPoints) + " achievement points needed";
	return CCConfirmView::create("Not unlocked yet!", body.c_str(), "Ok");
}

void ViewTools::showUnitInfo(UnitFactory* unitFactory, CCNode* parentNode) {

	std::string title = unitFactory->getUnitName();

	std::string attackString = unitFactory->getAttack()->isHeal() ? "Heal" : "Attack";
	std::string attacktypeString = unitFactory->getAttack()->isHeal() ? "Heal" : "Damage";
	std::string dhpsString = unitFactory->getAttack()->isHeal() ? "HPS" : "DPS";

	Attack* a = unitFactory->getAttack();
	Armour* arm = unitFactory->getArmour();
	std::string manaInfo = "";
	if (unitFactory->getSkills().size() != 0) {
		manaInfo = "\nMana: " + Tools::toString((int) (unitFactory->getMana())) + " (Reg: " + Tools::toString(unitFactory->getManaPerSeconds(), 2) + "/s)";
	}

	std::string content = "Hitpoints: " + Tools::toString((int) (unitFactory->getHitpoints()))

	+ manaInfo

	+ "\n" + attackString + ": " + a->getAttackInfo()

	+ "\n" + dhpsString + ": " + Tools::toString((int) (a->getDamagePerSecond() * a->getDamageMultiplier()))

	+ " - Range: " + Tools::toString((int) a->getAttackRange())

	+ " - Type: " + Tools::toString(a->getDamageType())

	+ "\nAttack speed: " + Tools::toString(a->getAttackSpeed(), 1) + "/sec"

	+ "\nRun speed: " + Tools::toString((int) unitFactory->getRunspeed())

	+ "\nArmor: " + arm->getArmourInfo();

	int skillNum = 1;
	std::list<Skill*> skills = unitFactory->getSkills();
	for (std::list<Skill*>::iterator it = skills.begin(); it != skills.end(); ++it) {
		content += "\nSkill " + Tools::toString(skillNum) + ": " + (*it)->getInfo();
	}

	Tutorial* tut = NULL;
	bool skipRemover = false;
	if (Model::getInstance()->getGameMode() == TUTORIAL && !Model::getInstance()->isTutorialFinished()) {
		tut = new Tutorial(parentNode);
		Model::getInstance()->setTutorialFinished(true);
		AchievementTutorialFinished().setAchievedNow();
		skipRemover = true;
	}
	CCAlertView *alert = CCAlertView::create(title.c_str(), content.c_str(), "Ok", tut, callfuncO_selector(Tutorial::afterUnitInfoWatched), skipRemover);
	parentNode->addChild(alert, 100);
}

float ViewTools::addVersionAndCreditInfo(CCNode* parentNode, float scaleMult) {

	ccColor3B fontColor = ccc3(200, 200, 200);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//add version
	std::string version = std::string("v") + VERSION;
	cocos2d::CCLabelTTF* verion = CCLabelTTF::create(version.c_str(), FONT_NAME, 25 * scaleMult);
	verion->setColor(fontColor);
	verion->setPosition(ccp(winSize.width - verion->getContentSize().width / 2, verion->getContentSize().height / 2));
	parentNode->addChild(verion);

	//add credit
	std::string creditString = std::string("Credit & more: ") + WEBSITE_LOCATION;
	cocos2d::CCLabelTTF* credit = CCLabelTTF::create(creditString.c_str(), FONT_NAME, 25 * scaleMult);
	credit->setColor(fontColor);
	credit->setPosition(ccp(credit->getContentSize().width / 2, credit->getContentSize().height / 2));
	parentNode->addChild(credit);

	return verion->getContentSize().height * scaleMult;
}

void ViewTools::setStandardBackgroundImage(CCNode* parentNode) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCSprite* backgroundArt = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("menuBackground.png"));

	float bgScale = winSize.height / backgroundArt->getContentSize().height;
	backgroundArt->setScale(bgScale);

	float diff = winSize.width - backgroundArt->getContentSize().width * bgScale;
	float x = backgroundArt->getContentSize().width / 2 * bgScale + diff / 2;
	float y = backgroundArt->getContentSize().height / 2 * bgScale;
	backgroundArt->setPosition(CCPoint(x, y));
	parentNode->addChild(backgroundArt);
}

void ViewTools::addIdleUnitTo(CCNode* parentNode, float offSetDown) {
	Unit* u = new Pinkie(0, 0, 0, new InstantAttack(0, MAGIC, 0, 0), NULL, std::list<Skill*>(), 0, 0);
	u->doNotKillSpriteAndSlowIdle();
	u->doIdle();
	u->showHealthBar(false);
	CCSprite* sprite = (CCSprite*) u->getNode();

	float unitheight = CCDirector::sharedDirector()->getWinSize().height / 3;
	parentNode->addChild(sprite);
	sprite->setScale(unitheight / sprite->getContentSize().height);
	sprite->setPosition(CCPoint(u->getContentSize().getWidth() / 2, offSetDown + u->getContentSize().getHeight() / 2));

	sprite->setOpacity(0);
	CCActionInterval* action = CCFadeIn::create(1);
	sprite->runAction(action);

	delete u;
}

CCLayerColor * ViewTools::createLayerFromAchievement(Achievement * ach) {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float height = 100 * scaleMult;
	float width = CCDirector::sharedDirector()->getWinSize().width - 80 * scaleMult * 2; // need space at the sides for the return button
	float max_width = GUI_BOX_WIDTH_MAX * scaleMult;
	if (width > max_width)
		width = max_width;
	cocos2d::ccColor3B fontColor = ach->isAchieved() ? ccc3(0, 0, 0) : ccc3(100, 100, 100);
	cocos2d::ccColor3B imageColor = ach->isAchieved() ? ccc3(255, 255, 255) : ccc3(100, 100, 100);
	float imageOpacy = ach->isAchieved() ? 255 : 150;

	CCLayerColor* layer = CCLayerColor::create(ccc4(100, 100, 100, 100), width, height);

	//add the image
	cocos2d::CCSprite* image = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ach->getImage().c_str()));

	image->setColor(imageColor);
	float image_height = height * (3.0f / 4.0f);
	float image_width = height * (3.0f / 4.0f); //quadratic image, thats why we use height here too
	image->setScaleX(image_height / image->getContentSize().height);
	image->setScaleY(image_width / image->getContentSize().width);
	layer->addChild(image);
	image->setPosition(CCPoint(height / 2, height / 2));
	image->setOpacity(imageOpacy);

	//add points
	cocos2d::CCLabelTTF* points = CCLabelTTF::create(Tools::toString(ach->getPoints()).c_str(), FONT_NAME, height * (2.0f / 4.0f));
	points->setColor(fontColor);
	points->setPosition(CCPoint(width - height / 2, height / 2));
	layer->addChild(points);

	//add title
	cocos2d::CCLabelTTF * title = CCLabelTTF::create(ach->getTitle().c_str(), FONT_NAME_BOLD, height * (1.0f / 5.0f), CCSizeMake(width - 2.0f * height, 0),
			kCCTextAlignmentCenter);
	title->setColor(fontColor);
	title->setPosition(CCPoint(width / 2, height * (5.0f / 6.0f)));
	layer->addChild(title);

	//add description
	cocos2d::CCLabelTTF * description = CCLabelTTF::create(ach->getDescription().c_str(), FONT_NAME, height * (1.0f / 5.0f),
			CCSizeMake(width - 2.0f * height, 0), kCCTextAlignmentCenter);
	description->setColor(fontColor);
	description->setPosition(CCPoint(width / 2, height * (3.0f / 6.0f)));
	layer->addChild(description);

	//add achieved time
	if (ach->isAchieved()) {
		//add achieved time
		cocos2d::CCLabelTTF * achievedTime = CCLabelTTF::create(BW_Time::integerToDayTimeString(ach->getTimeAchieved()).c_str(), FONT_NAME,
				height * (1.0f / 5.0f), CCSizeMake(width - 2.0f * height, 0), kCCTextAlignmentCenter);
		achievedTime->setColor(fontColor);
		achievedTime->setPosition(CCPoint(width / 2, height * (1.0f / 6.0f)));
		layer->addChild(achievedTime);
	}

	return layer;
}
