/*
 * Armour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef BalanceSingleton_H_
#define BalanceSingleton_H_

#include <string>

class BalanceSingleton {
public:
	static BalanceSingleton * getInstance();
	virtual ~BalanceSingleton();
	static void reset();
	bool isAnotherRound();
	void increaseRound();
	void increaseRace1Win();
	void increaseRace2Win();
	void addDuration(int seconds);
	std::string getInformation();

	int getRaceId1() {
		return _raceId1;
	}
	int getRaceId2() {
		return _raceId2;
	}

	void setRace1(int race1) {
		_raceId1 = race1;
	}
	void setRace2(int race2) {
		_raceId2 = race2;
	}

	void increasegamesStuck() {
		_gamesStuck++;
	}

protected:
	BalanceSingleton();
	static BalanceSingleton* _instance;
	int _raceId1;
	int _raceId2;
	int _raceId1Wins;
	int _raceId2Wins;
	int _roundsDone;
	int _seconds;
	int _gamesStuck;
};

#endif
