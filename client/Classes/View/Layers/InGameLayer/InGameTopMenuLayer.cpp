#include "InGameTopMenuLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
#include "../../../Model/ArmyType.h"
#include "../../../Model/Model.h"
#include "../../../SceneControl.h"
#include "../../../Constants.h"
#include "../../../Network/ServerCommunication.h"
#include "../../../Tools/BW_Time.h"
#include "../../../Tools/ViewTools.h"
#include "../../CCConfirmView.h"
#include "../../SpriteTags.h"
#include "InGamePauseLayer.h"
#include "../../../Sound/SoundHandler.h"
#include "../../../Tools/OfflineLogger.h"

// on "init" you need to initialize your instance
bool InGameTopMenuLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
		return false;
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float button_length = 66 * scaleMult;
	float button_heigth = 66 * scaleMult;
	float padding = 10 * scaleMult;
	_soundOnString = "on";
	_soundOffString = "off";

	CCMenuItemSprite *shutdown = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("shutdown0.png"),
			CCSprite::createWithSpriteFrameName("shutdown1.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::shutdownTouched));
	shutdown->setScaleX(button_length / shutdown->getContentSize().width);
	shutdown->setScaleY(button_heigth / shutdown->getContentSize().height);

	CCMenuItemSprite *autoCam = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("autoCam0.png"),
			CCSprite::createWithSpriteFrameName("autoCam1.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::autoCamTouched));
	autoCam->setScaleX(button_length / autoCam->getContentSize().width);
	autoCam->setScaleY(button_heigth / autoCam->getContentSize().height);

	float scale = button_heigth / autoCam->getContentSize().height;
	_autCamOnOffLabel = CCLabelTTF::create("auto", FONT_NAME, PRICE_LABEL_FONT_SIZE / scale * scaleMult,
			CCSize(autoCam->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult), kCCTextAlignmentCenter);
	autoCam->addChild(_autCamOnOffLabel);
	_autCamOnOffLabel->setColor(Model::getInstance()->getInGameGameLayer()->getAutoCam() ? ccc3(0, 255, 0) : ccc3(255, 0, 0));
	_autCamOnOffLabel->setPosition(CCPoint(autoCam->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_autCamOnOffLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	CCMenuItemSprite *autoCamSwitch = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("autoCamSwitch0.png"),
			CCSprite::createWithSpriteFrameName("autoCamSwitch1.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::flipThisPlayerForCamTouched));
	autoCamSwitch->setScaleX(button_length / autoCamSwitch->getContentSize().width);
	autoCamSwitch->setScaleY(button_heigth / autoCamSwitch->getContentSize().height);

	CCMenuItemSprite *pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pause1.png"), CCSprite::createWithSpriteFrameName("pause2.png"),
			(CCObject*) this, menu_selector(InGameTopMenuLayer::pauseTouched));
	pause->setScaleX(button_length / pause->getContentSize().width);
	pause->setScaleY(button_heigth / pause->getContentSize().height);

	CCMenuItemSprite *zoomIn = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ZoomIn1.png"), CCSprite::createWithSpriteFrameName("ZoomIn2.png"),
			(CCObject*) this, menu_selector(InGameTopMenuLayer::zoomInTouched));
	zoomIn->setScaleX(button_length / zoomIn->getContentSize().width);
	zoomIn->setScaleY(button_heigth / zoomIn->getContentSize().height);

	CCMenuItemSprite *zoomOut = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ZoomOut1.png"),
			CCSprite::createWithSpriteFrameName("ZoomOut2.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::zoomOutTouched));
	zoomOut->setScaleX(button_length / zoomOut->getContentSize().width);
	zoomOut->setScaleY(button_heigth / zoomOut->getContentSize().height);

	CCMenuItemSprite *toggleGUI = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("toggleGUI1.png"),
			CCSprite::createWithSpriteFrameName("toggleGUI2.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::toggleGUITouched));
	toggleGUI->setScaleX(button_length / toggleGUI->getContentSize().width);
	toggleGUI->setScaleY(button_heigth / toggleGUI->getContentSize().height);

	CCMenuItemSprite *enableSound = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("soundEnable0.png"),
			CCSprite::createWithSpriteFrameName("soundEnable1.png"), (CCObject*) this, menu_selector(InGameTopMenuLayer::enableSoundTouched));
	enableSound->setScaleX(button_length / enableSound->getContentSize().width);
	enableSound->setScaleY(button_heigth / enableSound->getContentSize().height);

	scale = button_heigth / enableSound->getContentSize().height;
	_enableSoundLabel = CCLabelTTF::create(SoundHandler::getInstance()->isSoundEnabled() ? _soundOnString.c_str() : _soundOffString.c_str(), FONT_NAME,
			PRICE_LABEL_FONT_SIZE / scale * scaleMult, CCSize(enableSound->getContentSize().width, PRICE_LABEL_HEIGTH / scale * scaleMult),
			kCCTextAlignmentCenter);
	enableSound->addChild(_enableSoundLabel);
	_enableSoundLabel->setColor(SoundHandler::getInstance()->isSoundEnabled() ? ccc3(0, 255, 0) : ccc3(255, 0, 0));
	_enableSoundLabel->setPosition(CCPoint(enableSound->getContentSize().width / 2, PRICE_LABEL_BOTTOM_SPACE / scale * scaleMult));
	_enableSoundLabel->enableStroke(PRICE_LABEL_COLOR_STROKE, PRICE_LABEL_STROKE_SIZE / scale * scaleMult, true);

	CCMenu* menu = CCMenu::create();
	int x = 0;
	int y = 1;
	ViewTools::addToMenu(menu, zoomIn, button_length, button_heigth, x++, y, padding);
	ViewTools::addToMenu(menu, zoomOut, button_length, button_heigth, x++, y, padding);
	ViewTools::addToMenu(menu, autoCamSwitch, button_length, button_heigth, x++, y, padding);
	ViewTools::addToMenu(menu, autoCam, button_length, button_heigth, x++, y, padding);
	ViewTools::addToMenu(menu, toggleGUI, button_length, button_heigth, x++, y, padding);

	if (Model::getInstance()->getGameMode() != NETWORK)
		ViewTools::addToMenu(menu, pause, button_length, button_heigth, x++, y, padding);

	ViewTools::addToMenu(menu, shutdown, button_length, button_heigth, x, y, padding);
	ViewTools::addToMenu(menu, enableSound, button_length, button_heigth, x, y - 1, padding);

	// resizing this layer to the menu and place the menu correct
	float width = (x + 1) * button_length + x * padding;

	float height = button_heigth * 2 + padding;
	setContentSize(CCSize(width, height));
	menu->setPosition(0, 0);
	addChild(menu);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	setPosition(winSize.width - width - (UNITS_INFO_OVERLAY_WIDTH * scaleMult), winSize.height - height);
	setOpacity(0); // 0- total transparent, 255 not
	return true;
}

void InGameTopMenuLayer::enableSoundTouched(cocos2d::CCObject* pSender) {
	bool sound = !SoundHandler::getInstance()->isSoundEnabled();
	SoundHandler::getInstance()->enableSound(sound);

	_enableSoundLabel->setColor(sound ? ccc3(0, 255, 0) : ccc3(255, 0, 0));
	_enableSoundLabel->setString(sound ? _soundOnString.c_str() : _soundOffString.c_str());
}

void InGameTopMenuLayer::setInGamegameLayer(InGameGameLayer* ingameGameLayer) {
	this->ingameGameLayer = ingameGameLayer;
}

void InGameTopMenuLayer::toggleGUITouched(CCObject* pSender) {
	Model::getInstance()->setShowHealthBar(!Model::getInstance()->isShowHealthbar());
}

void InGameTopMenuLayer::autoCamTouched(CCObject* pSender) {
	if (ingameGameLayer->getAutoCam()) {
		//_autCamOnOffLabel->setString("Off");
		_autCamOnOffLabel->setColor(ccc3(255, 0, 0));
	} else {
		//_autCamOnOffLabel->setString("On");
		_autCamOnOffLabel->setColor(ccc3(0, 255, 0));
	}
	ingameGameLayer->setAutoCam(!ingameGameLayer->getAutoCam());
}

void InGameTopMenuLayer::flipThisPlayerForCamTouched(CCObject* pSender) {
	Model* model = Model::getInstance();

	ArmyType type = model->getAutoCamTarget();
	if (type != LEFT)
		model->setAutoCamTarget(LEFT);
	else
		model->setAutoCamTarget(RIGHT);
}

void InGameTopMenuLayer::centerTouched(CCObject* pSender) {
	if (ingameGameLayer->getAutoCam())
		autoCamTouched (NULL);
	ingameGameLayer->setInitialPosition();
}

void InGameTopMenuLayer::shutdownFunction() {
	CCLOG("QUIT!=================================================================================================");
	std::string quitInfo = "QUITTING";
	if (Model::getInstance()->getGameMode() == NETWORK) {
		ServerCommunication().sendGameInfo(quitInfo);
	} else if (Model::getInstance()->getStartedAs() == PVP || Model::getInstance()->getStartedAs() == PVA) {
		OfflineLogger::getInstance()->logInformationForCurrentGame(quitInfo);
	}

	GameStatistics().increaseSecondsPlayedBy(BW_Time::getMilliSecondsOverCached() / 1000 + 1);

	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}

void InGameTopMenuLayer::shutdownAbortFunction() {
	//do nothing...
}

void InGameTopMenuLayer::shutdownTouched(CCObject* pSender) {
	CCConfirmView *alert = CCConfirmView::create("", "Do you really want to leave?", "Yes", "No", this,
			callfuncO_selector(InGameTopMenuLayer::shutdownFunction), callfuncO_selector(InGameTopMenuLayer::shutdownAbortFunction));
	alert->setTag(NOUNIT);
	getParent()->addChild(alert, 100);
}

void InGameTopMenuLayer::pauseTouched(CCObject* pSender) {
	getParent()->addChild(InGamePauseLayer::create());
}

void InGameTopMenuLayer::zoomInTouched(CCObject* pSender) {
	ingameGameLayer->zoom(ingameGameLayer->getScale() * ZOOM_SCALE);
}
void InGameTopMenuLayer::zoomOutTouched(CCObject* pSender) {
	ingameGameLayer->zoom(ingameGameLayer->getScale() / ZOOM_SCALE);
}
