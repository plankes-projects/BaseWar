/*
 * Trigonometric.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef GameStatistics_H_
#define GameStatistics_H_

#include <string>

class GameStatistics {
public:
	GameStatistics();
	virtual ~GameStatistics();

	int getLocalGamesLost();
	int getLocalGamesWon();
	int getLocalGamesStarted();

	int getLocalMultiplayerGamesStarted();
	int getLocalMultiplayerGamesFinished();

	int getNetworkGamesLost();
	int getNetworkGamesWon();
	int getNetworkGamesStarted();

	int getSecondsPlayed();

	void increaseLocalGamesLost();
	void increaseLocalGamesWon();
	void increaseLocalGamesStarted();

	void increaseLocalMultiplayerGamesStarted();
	void increaseLocalMultiplayerGamesFinished();

	void increaseNetworkGamesLost();
	void increaseNetworkGamesWon();
	void increaseNetworkGamesStarted();

	void increaseSecondsPlayedBy(int seconds);

	int getBestSurviveInSec() const {
		return bestSurviveInSec;
	}

	void updateBestSurviveInSec(int bestSurviveInSec);

	int getHelllocalGamesLost() const {
		return helllocalGamesLost;
	}

	int getHelllocalGamesWon() const {
		return helllocalGamesWon;
	}

	int getEasylocalGamesLost() const {
		return easylocalGamesLost;
	}

	int getEasylocalGamesWon() const {
		return easylocalGamesWon;
	}

	int getHardlocalGamesLost() const {
		return hardlocalGamesLost;
	}

	int getHardlocalGamesWon() const {
		return hardlocalGamesWon;
	}

	int getNormallocalGamesLost() const {
		return normallocalGamesLost;
	}

	int getNormallocalGamesWon() const {
		return normallocalGamesWon;
	}

	int getVeryhardlocalGamesLost() const {
		return veryhardlocalGamesLost;
	}

	int getVeryhardlocalGamesWon() const {
		return veryhardlocalGamesWon;
	}

	std::string getLocalGameStatisticString();
private:
	int localGamesLost;
	int localGamesWon;
	int localGamesStarted;

	int easylocalGamesLost;
	int easylocalGamesWon;
	int normallocalGamesLost;
	int normallocalGamesWon;
	int hardlocalGamesLost;
	int hardlocalGamesWon;
	int veryhardlocalGamesLost;
	int veryhardlocalGamesWon;
	int helllocalGamesLost;
	int helllocalGamesWon;

	int localMultiplayerGamesStarted;
	int localMultiplayerGamesFinished;

	int networkGamesLost;
	int networkGamesWon;
	int networkGamesStarted;

	int secondsPlayed;

	int bestSurviveInSec;

};

#endif /* GameStatistics_H_ */
