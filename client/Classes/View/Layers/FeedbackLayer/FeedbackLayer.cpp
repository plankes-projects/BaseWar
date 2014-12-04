#include "FeedbackLayer.h"
#include "../../../SceneControl.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Model/Model.h"
#include "../../../Tools/Tools.h"
#include "../../../Constants.h"
#include "../../CCAlertView.h"
#include "../../../Network/ServerCommunication.h"
#include "../../../AchievementSystem/AchievementFeedback.h"

// on "init" you need to initialize your instance
bool FeedbackLayer::init() {
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 255)))
		return false;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	//adding the background
	ViewTools::setStandardBackgroundImage(this);
	float offsetDown = ViewTools::addVersionAndCreditInfo(this, scaleMult);
	ViewTools::addIdleUnitTo(this, offsetDown);

	int buttonHeight = 66 * scaleMult;
	int buttonLength = 66 * scaleMult;
	CCMenu* backButtonmenu = CCMenu::create();
	CCMenuItemSprite *backButton = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return0.png"),
			CCSprite::createWithSpriteFrameName("return1.png"), (CCObject*) this, menu_selector(FeedbackLayer::backTouched));
	backButton->setScaleX(buttonLength / backButton->getContentSize().width);
	backButton->setScaleY(buttonHeight / backButton->getContentSize().height);
	backButtonmenu->addChild(backButton);
	addChild(backButtonmenu);
	backButtonmenu->setPosition(CCPoint(winSize.width - buttonLength / 2, winSize.height - buttonHeight / 2));

	float layerWidth = winSize.width - 80 * scaleMult * 2; // need space at the sides for the return button
	float max_width = GUI_BOX_WIDTH_MAX * scaleMult;
	if (layerWidth > max_width)
		layerWidth = max_width;

	float submitFormToServerSize = 40 * scaleMult;
	//add toggle send game stats to server

	cocos2d::CCMenuItemFont *submitFormToServer = CCMenuItemFont::create("Submit", this, menu_selector(FeedbackLayer::sendFeedbackToServer));
	submitFormToServer->setFontSizeObj(submitFormToServerSize);
	submitFormToServer->setFontName(FONT_NAME_BOLD);
	submitFormToServer->setColor(ccc3(0, 90, 0));
	CCMenu* menu = CCMenu::create();
	menu->addChild(submitFormToServer, 1);
	menu->setPosition(CCPoint(layerWidth / 2, submitFormToServerSize / 2));

	std::vector<CCNode*> objects = createQuestionObjects(layerWidth);

	CCLayer *layer = CCLayer::create();
	layer->setContentSize(CCSize(layerWidth, submitFormToServerSize));
	layer->addChild(menu);
	objects.push_back(layer);

	_scrollLayer = ScrollLayer::create("Feedback", objects, layerWidth, 10 * scaleMult);
	addChild(_scrollLayer);

	return true;
}

void FeedbackLayer::sendFeedbackToServer(cocos2d::CCObject* pSender) {
	std::string delemiter = "<end/>\n";
	std::string sendString = "";

	for (unsigned int i = 0; i < _questions.size(); i++) {
		if (_questions[i]->getAnswer() == "")
			continue;

		sendString += _questions[i]->getQuestion() + delemiter;
		sendString += _questions[i]->getAnswer() + delemiter;
	}

	if (sendString == "")
		return; //nothing selected

	//add fixed questions and answers
	std::string noUserMark = "[nouser]";

	int width = cocos2d::CCDirector::sharedDirector()->getWinSize().width;
	int height = cocos2d::CCDirector::sharedDirector()->getWinSize().height;
	int dpi = cocos2d::CCDevice::getDPI();

	//to verify "What device are you playing at?"
	sendString += noUserMark + "resolutionString" + delemiter;
	sendString += Tools::toString(width) + "x" + Tools::toString(height) + ":" + Tools::toString(dpi) + delemiter;

	_scrollLayer->removeFromParentAndCleanup(true);

	float loadingInfoFontSize = 40 * Model::getInstance()->getGUIElementScaleMultiplicator();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_loadingInfo = CCLabelTTF::create("Transmitting...", FONT_NAME, loadingInfoFontSize);
	_loadingInfo->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
	addChild(_loadingInfo);

	ServerCommunication().sendFeedback(sendString, this, httpresponse_selector(FeedbackLayer::onTransmissionCompleted));
}

void FeedbackLayer::backTouched(CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}

void FeedbackLayer::onTransmissionCompleted(CCHttpClient *sender, CCHttpResponse *response) {
	std::string resp = "";
	if (!response) {
		CCLog("transmission response failed - response missing");
		resp = "Error: Response is missing.";
	} else if (!response->isSucceed()) {
		CCLog("transmission response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		resp = "Error: " + std::string(response->getErrorBuffer());
	} else {
		(new AchievementFeedback())->setAchievedNow();
		resp = "Thank you very much\nfor your feedback!";
		CCUserDefault::sharedUserDefault()->setStringForKey(LAST_FEEDBACK_FOR_VERSION, VERSION);
		CCUserDefault::sharedUserDefault()->flush();
	}

	CCLOG("feedback resp:\n %s", Tools::toString(response->getResponseData()).c_str());

	_loadingInfo->setString(resp.c_str());
}

std::vector<CCNode*> FeedbackLayer::createQuestionObjects(float layerWidth) {
	//create answers
	std::vector < std::string > answerYesNo;
	answerYesNo.push_back("Yes");
	answerYesNo.push_back("No");

	std::vector < std::string > answerRating;
	answerRating.push_back("Very good");
	answerRating.push_back("Good");
	answerRating.push_back("Hmm...");
	answerRating.push_back("Bad");
	answerRating.push_back("Very bad");

	std::vector < std::string > answerTime;
	answerTime.push_back("Very often");
	answerTime.push_back("Often");
	answerTime.push_back("Rarely");
	answerTime.push_back("Never");

	//create answers
	std::vector < std::string > answerDevice;
	answerDevice.push_back("Smartphone");
	answerDevice.push_back("Phablet");
	answerDevice.push_back("Tablet");

	std::vector < std::string > answerSex;
	answerSex.push_back("man");
	answerSex.push_back("woman");

	std::vector < std::string > answerForumusage;
	answerForumusage.push_back("use it");
	answerForumusage.push_back("read it");
	answerForumusage.push_back("know it");
	answerForumusage.push_back("do not know it");

	std::vector < std::string > answerAge;
	answerAge.push_back("<10");
	answerAge.push_back("10-15");
	answerAge.push_back("16-20");
	answerAge.push_back("21-30");
	answerAge.push_back(">30");

	//create questions
	std::vector<CCNode*> objects;
	objects.push_back(FeedbackQuestionLayer::create("What device are you playing at?", answerDevice, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("This version is ...", answerRating, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("Are the units well balanced?", answerRating, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("Would you recommend BaseWar to your friends?", answerYesNo, layerWidth));
	objects.push_back(FeedbackQuestionLayer::create("Do you play BaseWar with your friends?", answerTime, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("Do you play offline?", answerTime, layerWidth));
	objects.push_back(FeedbackQuestionLayer::create("Do you play online?", answerTime, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("There is a BaseWar forum at basewar.net/forum.\nI ...", answerForumusage, layerWidth));

	objects.push_back(FeedbackQuestionLayer::create("How old are you?", answerAge, layerWidth));
	objects.push_back(FeedbackQuestionLayer::create("I am a beautiful ...", answerSex, layerWidth));

	for (unsigned int i = 0; i < objects.size(); i++) {
		_questions.push_back((FeedbackQuestionLayer*) objects[i]);
	}
	return objects;
}
