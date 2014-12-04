/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementWin.h"
#include "../Tools/GameStatistics.h"

#include "../Tools/Tools.h"

AchievementWin::AchievementWin(GameMode mode, Difficulty difficulty, AchievementWinsEnum wins) :
		Achievement("", "", "", "", 1) {
	_mode = mode;
	_dif = difficulty;
	switch (wins) {
	case WIN_1:
		_title = "Rookie";
		_wins = 1;
		break;
	case WIN_5:
		_title = "Persistent";
		_wins = 5;
		break;
	case WIN_10:
		_title = "Advanced";
		_wins = 10;
		break;
	case WIN_23:
		_title = "Veteran";
		_wins = 23;
		break;
	case WIN_53:
		_title = "Pro Gamer";
		_wins = 53;
		break;
	}
	_image = "achievementWin" + Tools::toString(_wins) + ".png";

	_points = 5;
	switch (_mode) {
	case PVP:
		_description = (_wins == 1) ? "Finish a PVP game." : "Finish " + Tools::toString(_wins) + " PVP games.";
		break;
	case NETWORK:
		_description = (_wins == 1) ? "Win a network game." : "Win " + Tools::toString(_wins) + " network games.";
		break;
	case PVA:
		_description = (_wins == 1) ? "Win a game against the computer." : "Win " + Tools::toString(_wins) + " games against the computer.";
		_description += " (" + Tools::toString(_dif) + ").";
		_points *= (1 + _dif);
		break;
	case BALANCE_TEST:
	case TUTORIAL:
		//this will not happen
		break;
	}

	if (mode != PVA)
		difficulty = DIF_EASY;

	_persistentLoadSaveId = "achievementwin_" + Tools::toString((int) _mode) + "_" + Tools::toString((int) _dif) + "_"
			+ Tools::toString(_wins);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementWin::isAchievedInternCalculation() {

	switch (_mode) {
	case PVP:
		return GameStatistics().getLocalMultiplayerGamesFinished() >= _wins;
	case NETWORK:
		return GameStatistics().getNetworkGamesWon() >= _wins;
	case PVA:
		switch (_dif) {
		case DIF_EASY:
			return GameStatistics().getEasylocalGamesWon() >= _wins;
		case DIF_NORMAL:
			return GameStatistics().getNormallocalGamesWon() >= _wins;
		case DIF_HARD:
			return GameStatistics().getHardlocalGamesWon() >= _wins;
		case DIF_VERYHARD:
			return GameStatistics().getVeryhardlocalGamesWon() >= _wins;
		case DIF_HELL:
			return GameStatistics().getHelllocalGamesWon() >= _wins;
		}

	default:
		return false;
	}
}
