#include "FeedbackQuestionLayer.h"
#include "../../../Model/Model.h"
#include "../../../Constants.h"

FeedbackQuestionLayer* FeedbackQuestionLayer::create(std::string question, std::vector<std::string> answers, float layerWidth) {
	FeedbackQuestionLayer *pRet = new FeedbackQuestionLayer();
	if (pRet && pRet->init(question, answers, layerWidth)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

// on "init" you need to initialize your instance
bool FeedbackQuestionLayer::init(std::string question, std::vector<std::string> answers, float layerWidth) {
	if (!CCLayerColor::initWithColor(ccc4(100, 100, 100, 100)))
		return false;

	_activeColor = ccc3(0, 90, 0);
	_inactiveColor = ccc3(230, 230, 230);
	_question = question;
	_currentAnswer = -1;

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();

	//add question
	float questionFontSize = 20 * scaleMult;
	float answerFontSize = 20 * scaleMult;
	float padding = 5 * scaleMult;
	std::string fontName = FONT_NAME;
	ccColor3B fontColor = ccc3(0, 0, 0);

	CCLabelTTF * questionTFT = CCLabelTTF::create(question.c_str(), fontName.c_str(), questionFontSize, CCSize(layerWidth - padding * 2, 0.f),
			kCCTextAlignmentCenter);
	questionTFT->setColor(fontColor);
	addChild(questionTFT);

	float questionHeight = questionTFT->getContentSize().height;

	//add answers to the layer
	float answerHeight = 0;
	CCMenu* menu = CCMenu::create();
	for (unsigned int i = 0; i < answers.size(); i++) {
		float pos = (1.0 / ((float) answers.size() + 1.0)) * ((float) i + 1.0) * layerWidth;
		cocos2d::CCMenuItemFont* answer = CCMenuItemFont::create(answers[i].c_str(), this, menu_selector(FeedbackQuestionLayer::answerTouched));
		answer->setFontSizeObj(answerFontSize);
		answer->setFontName(fontName.c_str());
		answer->setColor(_inactiveColor);
		answer->setPosition(CCPoint(pos, padding + answer->getContentSize().height / 2));
		answer->setTag(i);

		_answers.push_back(answer);
		_answersInString.push_back(answers[i]);

		menu->addChild(answer, 1);

		if (answerHeight < answer->getContentSize().height)
			answerHeight = answer->getContentSize().height;
	}
	menu->setPosition(CCPoint(0, 0));
	addChild(menu, 1);

	questionTFT->setPosition(CCPoint(layerWidth / 2, questionHeight / 2 + padding * 2 + answerHeight));
	setContentSize(CCSize(layerWidth, questionHeight + 3 * padding + answerHeight));

	return true;
}

void FeedbackQuestionLayer::answerTouched(cocos2d::CCObject* pSender) {
	cocos2d::CCMenuItemFont* sender = (cocos2d::CCMenuItemFont*) pSender;
	for (unsigned int i = 0; i < _answers.size(); i++) {
		_answers[i]->setColor((_answers[i] == sender) ? _activeColor : _inactiveColor);
	}

	_currentAnswer = sender->getTag();
}

std::string FeedbackQuestionLayer::getQuestion() {
	return _question;
}

std::string FeedbackQuestionLayer::getAnswer() {
	if (_currentAnswer == -1)
		return "";

	return _answersInString[_currentAnswer];
}
