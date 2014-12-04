#include "cocos2d.h"

#ifndef __CC_ALERT_VIEW_CLASS__
#define __CC_ALERT_VIEW_CLASS__

using namespace cocos2d;

class CCAlertView : public CCLayer {
public:
    static CCAlertView *create(const char *_title, const char *_message, const char *_buttonLabel, CCObject *_object = NULL, SEL_CallFuncO _selector1 = NULL, bool skipRemove = false);
    bool init(const char *_title, const char *_message, const char *_buttonLabel, CCObject *_object, SEL_CallFuncO _selector1, bool skipRemove);
    static void removeAlertViewIfPresent();
    static void setAlertView(CCNode* node);
private:
    static CCNode* _instanceRemover;
	CCObject *object;
	SEL_CallFuncO selector1;
	void buttonCallback(CCObject *pSender);
	void buttonCallbackAlt(CCObject *pSender);
	bool _skipRemove;
};

#endif
