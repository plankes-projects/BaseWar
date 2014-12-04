#include "AppDelegate.h"

#include "cocos2d.h"
#include "SceneControl.h"
#include "Tools/BW_Time.h"
#include "Model/Model.h"
#include "Sound/SoundHandler.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
	CCLOG("APP delegegate called");
}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
#ifdef DEBUG
	// turn on display FPS
	pDirector->setDisplayStats(true);
#endif
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	CCScene *pScene = SceneControl::loadingSpritesScene();

	// run
	pDirector->runWithScene(pScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	CCUserDefault::sharedUserDefault()->flush();
	if (Model::getInstance()->getStartedAs() != NETWORK) {
		if (!Model::getInstance()->isGamePaused()) {
			BW_Time::stopCachedMilliseconds();
			CCDirector::sharedDirector()->pause();
		}

		CCDirector::sharedDirector()->stopAnimation();
	}

	SoundHandler::getInstance()->pauseSound();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	if (Model::getInstance()->getStartedAs() != NETWORK) {
		CCDirector::sharedDirector()->stopAnimation(); //fixes a bug in cocos2dx
		CCDirector::sharedDirector()->startAnimation();

		if (!Model::getInstance()->isGamePaused()) {
			CCDirector::sharedDirector()->resume();
			BW_Time::startCachedMilliseconds();
		}
	}

	SoundHandler::getInstance()->resumeSound();
}
