#include "ScrollLayer.h"
#include "../Model/Model.h"
#include "../Constants.h"
#include "../Tools/BW_Time.h"

ScrollLayer* ScrollLayer::create(const char* title, std::vector<CCNode*> nodes, float layerWidth, float padding) {
	ScrollLayer *pRet = new ScrollLayer();
	if (pRet && pRet->init(title, nodes, layerWidth, padding)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool ScrollLayer::init(const char* title, std::vector<CCNode*> nodes, float layerWidth, float padding) {
	if (!CCLayerColor::initWithColor(ccc4(100, 255, 255, 0)))
		return false;
	this->setTouchEnabled(true);

	//config
	ccColor3B titleColor = ccc3(0, 0, 0); //	{r, g, b}
	float titleFontSize = 60;
	//config end -------------------------

	float scaleMult = Model::getInstance()->getGUIElementScaleMultiplicator();
	titleFontSize *= scaleMult;

	_manualScroll = false;
	_autoScrollSpeed = 0;

	float layerHeight = padding;
	for (std::vector<CCNode*>::reverse_iterator objectsI = nodes.rbegin(); objectsI != nodes.rend(); ++objectsI) {
		CCNode* object = *objectsI;
		addChild(object);
		object->setPosition(CCPoint(0, layerHeight));
		layerHeight += object->getContentSize().height + padding;
	}

	cocos2d::CCLabelTTF* titleObject = CCLabelTTF::create(title, FONT_NAME, titleFontSize);
	titleObject->setColor(titleColor);
	titleObject->setPosition(CCPoint(layerWidth / 2, layerHeight + titleFontSize / 2));
	addChild(titleObject);

	//set layer size and position
	setContentSize(CCSize(layerWidth, layerHeight));
	_layer_max_y = CCDirector::sharedDirector()->getWinSize().height * (1.0f / 4.0f);
	_layer_min_y = CCDirector::sharedDirector()->getWinSize().height * (3.0f / 4.0f) - getContentSize().height;
	setPosition(CCDirector::sharedDirector()->getWinSize().width / 2 - getContentSize().width / 2, _layer_min_y);

	this->schedule(schedule_selector(ScrollLayer::update));

	return true;
}

void ScrollLayer::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

bool ScrollLayer::ccTouchBegan(CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

	initialTouch = pTouch->getLocation();
	initialPos = getPosition();

	BW_Rect layer = BW_Rect(initialPos.x, initialPos.y, getContentSize().width, getContentSize().height);
	bool touchThisLayer = layer.contains(BW_Point(initialTouch.x, initialTouch.y));
	if (touchThisLayer)
		_manualScroll = true;

	return touchThisLayer; //return true if we touch this layer --> handle the touch event
}

void ScrollLayer::ccTouchMoved(CCTouch * pTouch, CCEvent* event) {
	//we move the full layer here
	double now = BW_Time::getMilliSeconds();
	if (now - _lastTouchTime > 100) {
		_lastTouch = pTouch->getLocation();
		_lastTouchTime = now;
	}
	float y = initialPos.y + (pTouch->getLocation().y - initialTouch.y);
	setThisPosition(y);
}

void ScrollLayer::ccTouchEnded(CCTouch * pTouch, CCEvent* event) {
	_manualScroll = false;
	_autoScrollSpeed = _lastTouch.y - pTouch->getLocation().y;

	if (_autoScrollSpeed == 0)
		return;

	//normalize speed to 1s
	double timeSpan = BW_Time::getMilliSeconds() - _lastTouchTime;
	_autoScrollSpeed /= (timeSpan / 1000);
}

void ScrollLayer::setThisPosition(float y) {
	setPosition(getPosition().x, y);
}

void ScrollLayer::update(float dt) {
	if (_manualScroll)
		return;

	float y = getPosition().y;
	float scrollInSpeed = 2000;
	//scroll back into screen mode
	if (y > _layer_max_y) {
		float newY = y - dt * scrollInSpeed;
		if (newY <= _layer_max_y)
			newY = _layer_max_y;

		setThisPosition(newY);
		_autoScrollSpeed = 0;
	} else if (y < _layer_min_y) {
		float newY = y + dt * scrollInSpeed;
		if (newY >= _layer_min_y)
			newY = _layer_min_y;

		setThisPosition(newY);
		_autoScrollSpeed = 0;
	}

	if (_autoScrollSpeed == 0)
		return;

	//auto scroll in scroll direction mode
	//calculate length the layer has to move
	float length = dt * _autoScrollSpeed * -1;
	setThisPosition(y + length);

	//calculate new speed
	float reduceSpeedFactor = 5000;
	float newSpeed = _autoScrollSpeed;
	if (_autoScrollSpeed < 0) {
		newSpeed += reduceSpeedFactor * dt;
		if (newSpeed >= 0)
			newSpeed = 0;
	} else if (_autoScrollSpeed > 0) {
		newSpeed -= reduceSpeedFactor * dt;
		if (newSpeed <= 0)
			newSpeed = 0;
	} else {
		newSpeed = 0;
	}
	_autoScrollSpeed = newSpeed;
}
