#include "CCAlertView.h"
#include "../Model/Model.h"
#include "../Constants.h"

CCAlertView *CCAlertView::create(const char *_title, const char *_message, const char *_buttonLabel, CCObject *_object,
		SEL_CallFuncO _selector1, bool skipRemove) {
	CCAlertView *pRet = new CCAlertView();
	if (pRet && pRet->init(_title, _message, _buttonLabel, _object, _selector1, skipRemove)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);

	return NULL;
}

void CCAlertView::setAlertView(CCNode* node) {
	CCAlertView::removeAlertViewIfPresent();
	CCAlertView::_instanceRemover = node;
}

CCNode* CCAlertView::_instanceRemover = NULL;
void CCAlertView::removeAlertViewIfPresent() {
	if (CCAlertView::_instanceRemover != NULL)
		CCAlertView::_instanceRemover->removeFromParentAndCleanup(true);
	CCAlertView::_instanceRemover = NULL;
}

bool CCAlertView::init(const char *_title, const char *_message, const char *_cancel, CCObject *_object, SEL_CallFuncO _selector1, bool skipRemove) {
	CCAlertView::setAlertView(this);
	_skipRemove = skipRemove;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	this->setTouchEnabled(true);
	this->setPosition(ccp(size.width / 2.0f, size.height / 2.0f));
	this->setScale(Model::getInstance()->getGUIElementScaleMultiplicator());

	CCSprite *alertViewSprite = CCSprite::createWithSpriteFrameName("alert_body_alert.png");
	this->addChild(alertViewSprite, 1001);
	//alertViewSprite->setOpacity(180);
	CCMenuItemSprite *button1 = NULL;
	if (_object == NULL || _selector1 == NULL) {
		button1 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("alert_button1.png"),
				CCSprite::createWithSpriteFrameName("alert_button1_down.png"), (CCObject*) this,
				menu_selector(CCAlertView::buttonCallback));
	} else {
		object = _object;
		selector1 = _selector1;
		button1 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("alert_button1.png"),
				CCSprite::createWithSpriteFrameName("alert_button1_down.png"), (CCObject*) this,
				menu_selector(CCAlertView::buttonCallbackAlt));
	}
	button1->setOpacity(180);

	CCMenu *alertMenu = CCMenu::create(button1, NULL);
	alertMenu->alignItemsHorizontallyWithPadding(7);
	alertMenu->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, 16 + (button1->getContentSize().height / 2.0f)));
	alertViewSprite->addChild(alertMenu);

	CCLabelTTF *titleLabel = CCLabelTTF::create(_title, FONT_NAME_BOLD, 18);
	titleLabel->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, alertViewSprite->getContentSize().height - 25.0f));
	alertViewSprite->addChild(titleLabel);

	CCLabelTTF *messageLabel = CCLabelTTF::create(_message, FONT_NAME, 15, CCSizeMake(alertViewSprite->getContentSize().width - 20.0f, 0),
			kCCTextAlignmentCenter);
	messageLabel->setPosition(ccp(alertViewSprite->getContentSize().width / 2.0f, alertViewSprite->getContentSize().height - 150.0f));
	alertViewSprite->addChild(messageLabel);

	CCLabelTTF *cancelLabel = CCLabelTTF::create(_cancel, FONT_NAME_BOLD, 18);
	cancelLabel->setPosition(ccp(button1->getContentSize().width / 2.0f, button1->getContentSize().height / 2.0f));
	button1->addChild(cancelLabel);

	CCFiniteTimeAction *scale1 = CCScaleTo::create(0.15f, 1.1f);
	CCFiniteTimeAction *scale2 = CCScaleTo::create(0.1f, 0.9f);
	CCFiniteTimeAction *scale3 = CCScaleTo::create(0.05f, 1.0f);
	alertViewSprite->runAction(CCSequence::create(scale1, scale2, scale3, NULL));

	return true;
}

void CCAlertView::buttonCallback(CCObject *pSender) {
	CCAlertView::removeAlertViewIfPresent();
}
void CCAlertView::buttonCallbackAlt(CCObject *pSender) {
	CCCallFuncO *first = CCCallFuncO::create(object, selector1, object);
	CCCallFunc *second = _skipRemove ? NULL : CCCallFunc::create(this, callfunc_selector(CCAlertView::buttonCallback));
	this->runAction(CCSequence::create(first, second, NULL));
}
