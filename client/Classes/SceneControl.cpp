/*
 * SceneControl.cpp
 *
 *  Created on: May 17, 2013
 *      Author: planke
 */

#include "SceneControl.h"
#include "View/Layers/StartMenuLayer/StartMenuLayer.h"
#include "View/Layers/RaceLayer/RaceLayer.h"
#include "View/Layers/InGameLayer/InGameTopMenuLayer.h"
#include "View/Layers/InGameLayer/InGameGameLayer.h"
#include "View/Layers/InGameLayer/InGameBottomMenuLayer.h"
#include "View/Layers/InGameLayer/InGameSpawnInfo.h"
#include "View/Layers/InGameLayer/InGameUnitsInfo.h"
#include "View/Layers/AchievementLayer/AchievementLayer.h"
#include "View/Layers/AchievementLayer/AchievementUpdateType.h"
#include "Model/Model.h"
#include "View/Layers/LoadingSpriteLayer/LoadingSpriteLayer.h"
#include "View/Layers/NetworkQueueLayer/NetworkQueueLayer.h"
#include "View/Layers/GameOverLayer/GameOverLayer.h"
#include "View/Layers/StatsLayer/StatsLayer.h"
#include "View/Layers/FeedbackLayer/FeedbackLayer.h"
#include "View/CCAlertView.h"
#include "View/CCConfirmView.h"
#include "View/Layers/InGameLayer/Tutorial.h"
#include "Debug/BalanceSingleton.h"
#include "Constants.h"

using namespace cocos2d;

SceneControl::SceneControl() {

}

SceneControl::~SceneControl() {
}

void SceneControl::replaceScene(CCScene* newScene, bool forward) {
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.35f, newScene));
}

CCScene* SceneControl::loadingSpritesScene() {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	scene->addChild(LoadingSpriteLayer::create());

	return scene;
}

CCScene* SceneControl::startMenuScene() {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	scene->addChild(StartMenuLayer::create());
	return scene;
}

CCScene* SceneControl::raceScene(GameMode mode, std::string name, ArmyType player) {

	CCAlertView::removeAlertViewIfPresent();
	if (player == LEFT)
		Model::reset();
	CCScene *scene = CCScene::create();
	RaceLayer* layer = RaceLayer::create(mode, name, player);
	scene->addChild(layer);

	return scene;
}

CCScene* SceneControl::achievementScene() {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	AchievementLayer * layer = AchievementLayer::create();
	scene->addChild(layer);
	layer->updateAchievement(AUT_ALL);
	return scene;
}

CCScene* SceneControl::feedbackScene() {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	scene->addChild(FeedbackLayer::create());
	return scene;
}

CCScene* SceneControl::statsScene() {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	scene->addChild(StatsLayer::create());
	return scene;
}

cocos2d::CCScene* SceneControl::gameOverScene(bool won) {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();
	GameOverLayer* layer = GameOverLayer::create();
	layer->setWon(won);
	scene->addChild(layer);
	return scene;
}

CCScene* SceneControl::queueScene(int raceId, std::string playerName) {
	CCAlertView::removeAlertViewIfPresent();
	CCScene *scene = CCScene::create();

	NetworkQueueLayer *layer = NetworkQueueLayer::create();
	layer->setPlayerName(playerName);
	layer->setRaceId(raceId);
	scene->addChild(layer);
	return scene;
}

CCScene* SceneControl::gameScene(float gameSpeed, GameMode mode) {
	CCAlertView::removeAlertViewIfPresent();

	Model::getInstance()->setGameSpeed(gameSpeed);
	Model::getInstance()->setGameMode(mode);

	CCScene *scene = CCScene::create();

	InGameGameLayer *gameLayer = InGameGameLayer::create();
	InGameTopMenuLayer *layer = InGameTopMenuLayer::create();
	layer->setInGamegameLayer(gameLayer);

	// add layer as a child to scene
	scene->addChild(gameLayer);
	scene->addChild(layer);

	//create this layer at last, because we need to have valid players and
	//unit factories for initialize the menu
	if (mode == PVP) {
		Model::getInstance()->setThisPlayer(Model::getInstance()->getMyArmy(RIGHT)->getPlayers().front());
		scene->addChild(InGameBottomMenuLayer::create());

		Model::getInstance()->setThisPlayer(Model::getInstance()->getMyArmy(LEFT)->getPlayers().front());
		scene->addChild(InGameBottomMenuLayer::create());

	} else if (mode == PVA || mode == NETWORK) {
		scene->addChild(InGameBottomMenuLayer::create());
		if (mode == PVA && Model::getInstance()->getDifficulty() == DIF_HELL)
			scene->addChild(CCConfirmView::create("Survival Mode", "Survive as long as possible!", "Ok"));

	} else if (mode == TUTORIAL) {
		scene->addChild(InGameBottomMenuLayer::create());
		(new Tutorial(scene))->gameFieldTut();
	}

	scene->addChild(InGameSpawnInfo::create());
	scene->addChild(InGameUnitsInfo::create());

	return scene;
}
