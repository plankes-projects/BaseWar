#include "StatsLayer.h"
#include "../../../SceneControl.h"
#include "../../../Tools/ViewTools.h"
#include "../../../Model/Model.h"
#include "../../ScrollLayer.h"
#include "../../../Tools/Tools.h"
#include "../../../Constants.h"

// on "init" you need to initialize your instance
bool StatsLayer::init() {
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
			CCSprite::createWithSpriteFrameName("return1.png"), (CCObject*) this, menu_selector(StatsLayer::backTouched));
	backButton->setScaleX(buttonLength / backButton->getContentSize().width);
	backButton->setScaleY(buttonHeight / backButton->getContentSize().height);
	backButtonmenu->addChild(backButton);
	addChild(backButtonmenu);
	backButtonmenu->setPosition(CCPoint(winSize.width - buttonLength / 2, winSize.height - buttonHeight / 2));

	//create stats layer

	std::vector < std::vector<std::string> > content;
	std::vector < std::string > section;

	GameStatistics gs = GameStatistics();

	section = std::vector<std::string>();
	section.push_back("Player VS Computer");
	section.push_back("Started: " + Tools::toString(gs.getLocalGamesStarted()));
	section.push_back("Finished: " + Tools::toString(gs.getLocalGamesWon() + gs.getLocalGamesLost()));
	section.push_back("");
	section.push_back("Won: " + Tools::toString(gs.getLocalGamesWon()));
	section.push_back("Easy: " + Tools::toString(gs.getEasylocalGamesWon()));
	section.push_back("Normal: " + Tools::toString(gs.getNormallocalGamesWon()));
	section.push_back("Hard: " + Tools::toString(gs.getHardlocalGamesWon()));
	section.push_back("Very hard: " + Tools::toString(gs.getVeryhardlocalGamesWon()));
	//section.push_back("Hell: " + Tools::toString(gs.getHelllocalGamesWon()));
	section.push_back("");
	section.push_back("Lost: " + Tools::toString(gs.getLocalGamesLost()));
	section.push_back("Easy: " + Tools::toString(gs.getEasylocalGamesLost()));
	section.push_back("Normal: " + Tools::toString(gs.getNormallocalGamesLost()));
	section.push_back("Hard: " + Tools::toString(gs.getHardlocalGamesLost()));
	section.push_back("Very hard: " + Tools::toString(gs.getVeryhardlocalGamesLost()));
	//section.push_back("Hell: " + Tools::toString(gs.getHelllocalGamesLost()));
	section.push_back("");
	section.push_back("Survival mode played: " + Tools::toString(gs.getHelllocalGamesLost() + gs.getHelllocalGamesWon()));
	int sec = gs.getBestSurviveInSec();
	int min = sec / 60;
	sec %= 60;
	std::string secString = sec < 10 ? ("0" + Tools::toString(sec)) : Tools::toString(sec);
	section.push_back("Best survival time: " + Tools::toString(min) + ":" + secString);
	content.push_back(section);

	section = std::vector<std::string>();
	section.push_back("Player VS Player");
	section.push_back("Started: " + Tools::toString(gs.getLocalMultiplayerGamesStarted()));
	section.push_back("Finished: " + Tools::toString(gs.getLocalMultiplayerGamesFinished()));
	content.push_back(section);

	section = std::vector<std::string>();
	section.push_back("Network");
	section.push_back("Started: " + Tools::toString(gs.getNetworkGamesStarted()));
	section.push_back("Finished: " + Tools::toString(gs.getNetworkGamesWon() + gs.getNetworkGamesLost()));
	section.push_back("Won: " + Tools::toString(gs.getNetworkGamesWon()));
	section.push_back("Lost: " + Tools::toString(gs.getNetworkGamesLost()));
	content.push_back(section);

	section = std::vector<std::string>();
	section.push_back("Additional Information");
	section.push_back("Total hours played: " + Tools::toString((int) (gs.getSecondsPlayed() / 3600)));
	content.push_back(section);

	float layerWidth = winSize.width - 80 * scaleMult * 2; // need space at the sides for the return button
	float max_width = GUI_BOX_WIDTH_MAX * scaleMult;
	if (layerWidth > max_width)
		layerWidth = max_width;

	std::vector<CCNode*> objects;
	for (std::vector<std::vector<std::string> >::iterator contentI = content.begin(); contentI != content.end(); ++contentI) {
		cocos2d::CCLayerColor* sectionLayer = createSectionLayer(*contentI, layerWidth);
		objects.push_back(sectionLayer);
	}

	addChild(ScrollLayer::create("Game Statistic", objects, layerWidth, 10 * scaleMult));

	return true;
}

CCLayerColor* StatsLayer::createSectionLayer(std::vector<std::string> section, float width) {
	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	float padding = 5 * scaleMult;

	CCLayerColor* layer = CCLayerColor::create(ccc4(100, 100, 100, 100));

	float layerHeight = padding;
	for (int i = section.size() - 1; i >= 0; i--) {
		float fontSize = i == 0 ? 25 * scaleMult : 20 * scaleMult;
		std::string fontName = i == 0 ? FONT_NAME_BOLD : FONT_NAME;
		ccColor3B fontColor = i == 0 ? ccc3(0, 0, 0) : ccc3(0, 0, 0);

		CCLabelTTF * text = CCLabelTTF::create(section[i].c_str(), fontName.c_str(), fontSize, CCSize(width, 0.f), kCCTextAlignmentCenter);
		layer->addChild(text);
		text->setPosition(CCPoint(width / 2, layerHeight + fontSize / 2));
		text->setColor(fontColor);
		layerHeight += fontSize + padding;

	}
	layer->setContentSize(CCSize(width, layerHeight));
	return layer;
}

void StatsLayer::backTouched(CCObject* pSender) {
	SceneControl::replaceScene(SceneControl::startMenuScene(), false);
}
