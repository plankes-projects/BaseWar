#ifndef __FeedbackQuestionLayer_H__
#define __FeedbackQuestionLayer_H__

#include "cocos2d.h"
using namespace cocos2d;

class FeedbackQuestionLayer: public cocos2d::CCLayerColor {
public:
	static FeedbackQuestionLayer* create(std::string question, std::vector<std::string> answers, float layerWidth);

	std::string getQuestion();
	std::string getAnswer();

private:

	virtual bool init(std::string question, std::vector<std::string> answers, float layerWidth);
	void answerTouched(cocos2d::CCObject* pSender);

	std::string _question;
	int _currentAnswer;
	std::vector<cocos2d::CCMenuItemFont*> _answers;
	std::vector<std::string> _answersInString;
	cocos2d::ccColor3B _activeColor;
	cocos2d::ccColor3B _inactiveColor;
};

#endif // __StatsLayer_H__
