#ifndef BW_CONSTANTS_H
#define BW_CONSTANTS_H

#include "cocos2d.h"

#define APIKEY 						"bw3.11" //NO SPACE ALLOWED!!!
#define VERSION 					"3.11"   //NO SPACE ALLOWED!!!

#define 	PRICE_LABEL_STROKE_SIZE		(cocos2d::CCDevice::getDPI() / 240.f)
//if we use const char* for FONT_NAME we have linking problems
#define FONT_NAME  "Helvetica"
#define FONT_NAME_BOLD  "HelveticaNeue-Bold"
#define WEBSITE_LOCATION "basewar.net"

#ifdef DEBUG
const int NEEDED_POINTS_FOR_NETWORK_GAME = 0;
const int NEEDED_POINTS_FOR_RANDOM_UNIT_RACE = 0;
const int NEEDED_POINTS_FOR_SPECIAL_AI_RACES = 0;
#else
const int NEEDED_POINTS_FOR_NETWORK_GAME = 40;
const int NEEDED_POINTS_FOR_RANDOM_UNIT_RACE = 80;
const int NEEDED_POINTS_FOR_SPECIAL_AI_RACES = 200;
#endif

const int START_ID_FOR_AI_RACES = 100;

const float PRICE_LABEL_HEIGTH = 30.f;
const float PRICE_LABEL_BOTTOM_SPACE = 18.f;
const float PRICE_LABEL_FONT_SIZE = 25.f;

const cocos2d::ccColor3B PRICE_LABEL_COLOR_STROKE = { 0, 0, 0 }; //	{r, g, b}
const cocos2d::ccColor3B PRICE_LABEL_COLOR_BUY = { 255, 215, 0 }; //	gold
const cocos2d::ccColor3B PRICE_LABEL_COLOR_INFO = { 0, 255, 0 }; //	green

const float ZOOM_SCALE = 1.4142f; //	sqrt(2)

const float NETWORK_SYNC_DELAY = 2; //	in seconds

const int GOLD_ON_TOWER_KILL = 300;
const float GOLD_FOR_MIDDLE_CONTROL = 1.15;
const float GOLD_FOR_GOLD_UPGRADE = 1.15;

const float BALANCE_TEST_GAME_SPEED = 10.f;
const int BALANCE_TEST_GAME_ROUNDS = 100;

const float UNITS_INFO_OVERLAY_WIDTH = 20; //the layer which moves in from the right in ingame

const int ACHIEVEMENT_FAST_WIN_UNDER_MIN = 15;
const int ACHIEVEMENT_SLOW_WIN_AFTER_MIN = 60;

const int ACHIEVEMENT_MASTER_SAVER_GOLD = 4000;

const float GUI_BOX_WIDTH_MAX = 700; //achievement width + statistic width

const int BACKGROUND_SPRITES_DISTANCE = 500;
const int PROJECTILE_SPEED = 900;

//instead of UNITS_TO_ULTIMATE_UNIT units, there will spawn one unit 5 times stronger
const int UNITS_TO_ULTIMATE_UNIT = 5;
const int UNITS_TO_ULTIMATE_UNIT_MULT = 4;
const int UNITS_TO_MASTER_UNIT = 5;	//not used anymore
const int UNITS_TO_MASTER_UNIT_MULT = 4;	//not used anymore


const float TIER_PRICE_REDUCTION_PER_SECOND = 0.5; //reducing 200 gold in 6.6 minutes

const int MELEE_UNIT_RECOGNITION_RANGE = 199; //200 is troll range... it should be handled as dps

const int SHOW_GIVE_FEEDBACK_EVERY_SECONDS = 3 * 60 * 60; // every 3h
#define LAST_FEEDBACK_FOR_VERSION  "lastFeedbackForVersion"

const bool LOG_OFFLINE_GAMES = true;
const int MAX_OFFLINE_GAMES_TO_LOG = 30;
const int NUMBER_OF_OFFLINE_GAMES_SEND_AT_ONE_TIME = 5;

#endif
