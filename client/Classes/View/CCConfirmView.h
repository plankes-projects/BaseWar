#include "cocos2d.h"

#ifndef __CC_CCConfirmView_CLASS__
#define __CC_CCConfirmView_CLASS__

using namespace cocos2d;

class CCConfirmView: public CCLayer {
public:
	static CCConfirmView *create(const char *_title, const char *_message, const char *_first, const char *_second = NULL,
			CCObject *_object = NULL, SEL_CallFuncO _selector1 = NULL, SEL_CallFuncO _selector2 = NULL);
	bool init(const char *_title, const char *_message, const char *_first, const char *_second, CCObject *_object,
			SEL_CallFuncO _selector1, SEL_CallFuncO _selector2);

private:
	CCObject *object;
	SEL_CallFuncO selector1;
	SEL_CallFuncO selector2;
	void cleanUpCallback();
	void button1Callback(CCObject *pSender);
	void button2Callback(CCObject *pSender);
};

#endif
