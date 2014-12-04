<?php
//checking parameters =========================================================
if(!isset($_POST['deviceID']) || !isset($_POST['data'])){
	echo "POST parameter missing.";
	return;
}

if(!is_string($_POST['deviceID']) || !is_string($_POST['data'])){
	echo "POST parameter malformed.";
	return;
}

//parsing gamestat string into datastructure ==================================
$delemiter = "<end/>\n";
$exp = explode($delemiter, $_POST['data']);

$arraySize = count($exp);
if($arraySize !== 11){
	//invalid feedback data form
	echo "invalid game progress data form 1";
	return;
}

//last entry should be ""
if($exp[$arraySize-1] !== ""){
	//invalid feedback data form
	echo "invalid game progress data form 2";
	return;
}
unset($exp[$arraySize-1]);
$arraySize--;

//insert into mysql databse ===================================================
require 'config.php';

$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
if (mysqli_connect_errno())
{
	echo "Failed to connect to MySQL. (prog1)";
	return;
}

//get user_id
require_once 'scripts/LogHelperFunctions.php';
$userId = getUserId($con, $_POST['deviceID']);
if(!$userId)
	return;

$now = time();

$statsString 				= getStringId($con, $exp[0]);
$localGamesStarted			= (int)$exp[1];
$multiplayerStated 			= (int)$exp[2];
$multiplayerWin 			= (int)$exp[3];
$multiplayerLose 			= (int)$exp[4];
$localMultiplayerStarted 	= (int)$exp[5];
$localMultiplayerFinished	= (int)$exp[6];
$secondsPlayed 				= (int)$exp[7];
$achievementPoints			= (int)$exp[8];
$achievementString			= getStringId($con, $exp[9]);



mysqli_query($con, "INSERT INTO game_progress (`user`, `datetime`, `multiplayer_started`, `multiplayer_win`, `multiplayer_lose`, `local_multiplayer_started`, `local_multiplayer_finished`, `seconds_played`, `achievement_points`, `achievement_string`, `computer_games_stats`, `computer_games_started`)
		VALUES ($userId, $now, $multiplayerStated, $multiplayerWin, $multiplayerLose, $localMultiplayerStarted, $localMultiplayerFinished, $secondsPlayed, $achievementPoints, $achievementString, $statsString, $localGamesStarted)");
