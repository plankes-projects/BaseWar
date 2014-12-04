/*
 * GameStat.h
 *
 *  Created on: 20.07.2013
 *      Author: Planke
 */

#ifndef GAMESTAT_H_
#define GAMESTAT_H_

#include <string>
#include <map>

class GameStat {
public:
	GameStat();

	// this string has additional to the client gamestatString
	// the player id at the beginnning
	GameStat(std::string serverGameStatString);
	virtual ~GameStat();

	//retuning the clientGamestateString
	std::string toString();

	// returns -1 if no race is set
	int getRace();
	void setRace(int id);

	int getGoldUp();
	void setGoldUp(int goldUp);

	int getUnitNum(int tier, int unit);
	void setUnitNum(int tier, int unit, int num);

	int getArmourUp(int tier, int unit);
	void setArmourUp(int tier, int unit, int up);

	int getAttackUp(int tier, int unit);
	void setAttackUp(int tier, int unit, int up);

	int getSkillUp(int tier, int unit);
	void setSkillUp(int tier, int unit, int up);

	int getManaUp(int tier, int unit);
	void setManaUp(int tier, int unit, int up);

	int getManaRegUp(int tier, int unit);
	void setManaRegUp(int tier, int unit, int up);

	int getRangeUp(int tier, int unit);
	void setRangeUp(int tier, int unit, int up);

	int getLifeUp(int tier, int unit);
	void setLifeUp(int tier, int unit, int lifeUp);

	int getRunspeedUp(int tier, int unit);
	void setRunspeedUp(int tier, int unit, int runspeedUp);

	void setPlayerId(int id);
	int getPlayerId();

	void setPlayerTier(int tier);
	int getPlayerTier();

	//removed the returned spawn
	//returns -1 if no spawn is left
	int getNextSpawnFor(int tier, int unit);
	void addSpawnPosition(int tier, int unit, int position);
	void clearSpawnPositions();

	char getGameStatDelemiter();

	std::string getRandomRaceString() {
		return _randomRaceString;
	}

	void setRandomRaceString(std::string randomRaceString) {
		_randomRaceString = randomRaceString;
	}

private:
	std::map<std::string, std::string> _map;

	// these 2 chars are not allowed to be in the map
	std::string _randomRaceString; //not added to the toString method
	char _delemiter;
	char _gameStatsDelemiter;
	void setValue(std::string key, int value);
	void setValue(std::string key, std::string value);
	bool hasKey(std::string key);
	void init();

	// this string has additional to the client gamestatString
	// the player id at the beginnning
	void fromString(std::string serverGameStatString);
};

#endif /* GAMESTAT_H_ */
