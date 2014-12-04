#include "CCConfirmView.h"
#include "CCAlertView.h"
#include "../Model/Model.h"
#include "../Constants.h"

CCConfirmView *CCConfirmView::create(const char *_title, const char *_message, const char *_first, const char *_second, CCObject *_object,
		SEL_CallFuncO _selector1, SEL_CallFuncO _selector2) {
	CCConfirmView *pRet = new CCConfirmView();
	if (pRet && pRet->init(_title, _message, _first, _second, _object, _selector1, _selector2)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);

	return NULL;
}

bool CCConfirmView::init(const char *_title, const char *_message, const char *_first, const char *_second, CCObject *_object,
		SEL_CallFuncO _selector1, SEL_CallFuncO _selector2) {

	CCAlertView::setAlertView(this);
	this->setScale(Model::getInstance()->getGUIElementScaleMultiplicator());

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	this->setTouchEnabled(true);
	this->setPosition(ccp(size.width / 2.0f, size.height / 2.0f));

	CCSprite *alertViewSprite = CCSprite::createWithSpriteFrameName("alert_body.png");
	this->addChild(alertViewSprite, 1001);

	CCMenu *alertMenu = NULL;
	CCMenuItemSprite *button1 = NULL;
	if (_second == NULL || _object == NULL || _selector1 == NULL || _selector2 == NULL) {

		button1 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("alert_button1.png"),
				CCSprite::createWithSpriteFrameName("alert_button1_down.png"), (CCObject*) this,
				menu_selector(CCConfirmView::cleanUpCallback));
		button1->setOpacity(180);
		alertMenu = CCMenu::create(button1, NULL);

		CCLabelTTF *cancelLabel = CCLabelTTF::create(_first, FONT_NAME_BOLD, 18);
		cancelLabel->setPosition(ccp(button1->getContentSize().width / 2.0f, button1->getContentSize().height / 2.0f));
		button1->addChild(cancelLabel);
	} else {

		object = _object;
		selector1 = _selector1;
		selector2 = _selector2;

		button1 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("alert_button1.png"),
				CCSprite::createWithSpriteFrameName("alert_button1_down.png"), (CCObject*) this,
				menu_selector(CCConfirmView::button1Callback));

		CCMenuItemSprite *button2 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("alert_button2.png"),
				CCSprite::createWithSpriteFrameName("alert_button2_down.png"), (CCObject*) this,
				menu_selector(CCConfirmView::button2Callback));

		CCLabelTTF *cancelLabel = CCLabelTTF::create(_first, FONT_NAME_BOLD, 18);
		cancelLabel->setPosition(ccp(button2->getContentSize().width / 2.0f, button2->getContentSize().height / 2.0f));
		button1->addChild(cancelLabel);

		CCLabelTTF *OKlabel = CCLabelTTF::create(_second, FONT_NAME_BOLD, 18);
		OKlabel->setPosition(ccp(button1->getContentSize().width / 2.0f, button1->getContentSize().height / 2.0f));
		button2->addChild(OKlabel);

		button1->setOpacity(180);
		button2->setOpacity(180);
		alertMenu = CCMenu::create(button1, button2, NULL);
	}

	alertMenu->alignItemsHorizontallyWithPadding(7);
	alertMenu->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, 16 + (button1->getContentSize().height / 2.0f)));
	alertViewSprite->addChild(alertMenu);

	CCLabelTTF *titleLabel = CCLabelTTF::create(_title, FONT_NAME_BOLD, 18);
	titleLabel->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, alertViewSprite->getContentSize().height - 25.0f));
	alertViewSprite->addChild(titleLabel);

	CCLabelTTF *messageLabel = CCLabelTTF::create(_message, FONT_NAME, 15,
			CCSizeMake(alertViewSprite->getContentSize().width - 20.0f, 0), kCCTextAlignmentCenter);
	messageLabel->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, alertViewSprite->getContentSize().height - 55.0f));
	alertViewSprite->addChild(messageLabel);

	CCFiniteTimeAction *scale1 = CCScaleTo::create(0.15f, 1.1f);
	CCFiniteTimeAction *scale2 = CCScaleTo::create(0.1f, 0.9f);
	CCFiniteTimeAction *scale3 = CCScaleTo::create(0.05f, 1.0f);
	alertViewSprite->runAction(CCSequence::create(scale1, scale2, scale3, NULL));

	return true;
}

void CCConfirmView::cleanUpCallback() {
	CCAlertView::removeAlertViewIfPresent();
}

void CCConfirmView::button1Callback(CCObject *pSender) {
	CCCallFuncO *first = CCCallFuncO::create(object, selector1, object);
	CCCallFunc *second = CCCallFunc::create(this, callfunc_selector(CCConfirmView::cleanUpCallback));
	this->runAction(CCSequence::create(first, second, NULL));
}

void CCConfirmView::button2Callback(CCObject *pSender) {
	CCCallFuncO *first = CCCallFuncO::create(object, selector2, object);
	CCCallFunc *second = CCCallFunc::create(this, callfunc_selector(CCConfirmView::cleanUpCallback));
	this->runAction(CCSequence::create(first, second, NULL));
}
