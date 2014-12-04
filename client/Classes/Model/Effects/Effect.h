/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef Effect_H_
#define Effect_H_

class Unit;

class Effect {
public:
	static const float FOREVER;
	static const float NOTICK;
	virtual ~Effect();
	Effect(float timeInMilliseconds, float tickEveryMilliSeconds);
	/**
	 * returns false if we called last perform
	 * --> remove it from unit at this point
	 */
	bool tryPerform(Unit* owner);
	/**
	 *returnes false if force remove is already set
	 */
	bool forceRemove();

	bool isHarmful() {
		return _isHarmful;
	}

	bool isStackAble() {
		return _isStackAble;
	}

	//returns true if successfully extended
	void reApply(Effect* effect);

	bool equals(Effect* other) {
		return _GUID == other->getGUID();
	}

	int getGUID() {
		return _GUID;
	}

	float getDuration() {
		return _duration;
	}

protected:
	virtual void perform(Unit* owner) = 0;
	virtual void onApply(Unit* owner) = 0;
	virtual void onRemoving(Unit* owner) = 0;
	bool _isHarmful; //has to be set at subclasses
	int _GUID;
	bool _isStackAble;
	bool _isForceRemoveAble;
private:
	double _lastperform;
	bool _firstPerform;
	double _removeTime;
	float _tickDelay;
	bool _forceRemove;

	float _duration;
};

#endif /* ATTACK_H_ */
