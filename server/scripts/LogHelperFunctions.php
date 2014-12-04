<?php

function logInsertGameInfo($deviceId, $gameId, $info){
	require 'config.php';
	
	$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL. (log1)";
		return;
	}
	
	$gameId = (int)$gameId;
	$userId = getUserId($con, $deviceId);
	$session = mysqli_real_escape_string($con, logGetSessionFromDeviceId($deviceId));
	$user_game_id = mysqli_query($con, "SELECT id FROM user_game WHERE user = $userId AND game = $gameId AND session_key = '$session'");
	$user_game_id = $user_game_id->fetch_assoc();
	$user_game_id = $user_game_id['id'];
	
	$now = time();
	$infoId = getGameStateStringId($con, $info);
	mysqli_query($con, "INSERT INTO game_information (`user_game`, `datetime`, `information`) VALUES ($user_game_id, $now, $infoId)");
	
}

function logTurnState($deviceId, $gameId, $state){
	require 'config.php';

	$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL. (log2)";
		return;
	}

	$gameId = (int)$gameId;
	$userId = getUserId($con, $deviceId);
	$session = mysqli_real_escape_string($con, logGetSessionFromDeviceId($deviceId));
	$user_game_id = mysqli_query($con, "SELECT id FROM user_game WHERE user = $userId AND game = $gameId AND session_key = '$session'");
	$user_game_id = $user_game_id->fetch_assoc();
	$user_game_id = $user_game_id['id'];

	//we only want to add it if it differs from the previous entry
	$newestStateId = mysqli_query($con, "SELECT game_state FROM game_state WHERE user_game = $user_game_id ORDER BY datetime DESC LIMIT 0, 1");
	$newestStateId = $newestStateId->fetch_assoc();
	$newestStateId = $newestStateId ? $newestStateId['game_state'] : $newestStateId;
	
	$now = time();
	$stateId = getGameStateStringId($con, $state);
	if(!$newestStateId || $newestStateId !== $stateId)
		mysqli_query($con, "INSERT INTO game_state (`user_game`, `datetime`, `game_state`) VALUES ($user_game_id, $now, $stateId)");

}

function logCreateGame($gameId, $playerNum, $gameStartTime){
	require 'config.php';

	$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL. (log3)";
		return;
	}

	$gameId = (int)$gameId;
	$playerNum = (int)$playerNum;
	$gameStartTime = (int)$gameStartTime;
	mysqli_query($con, "INSERT INTO game (`id`, `datetime`, `player_num`) VALUES ($gameId, $gameStartTime, $playerNum)");
}

function logAddUserToGame($gameId, $deviceId, $leftTeam){
	require 'config.php';

	$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL. (log4)";
		return;
	}

	$gameId = (int)$gameId;
	$team = $leftTeam ? "LEFT" : "RIGHT";
	$userId = getUserId($con, $deviceId);
	$session = mysqli_real_escape_string($con, logGetSessionFromDeviceId($deviceId));

	mysqli_query($con, "INSERT INTO user_game (`user`, `game`, `session_key`, `team`)
			VALUES ($userId, $gameId, '$session', '$team')");
}

function logGetSessionFromDeviceId($deviceId){
	return substr($deviceId, 1 + strpos($deviceId, ':', strpos($deviceId, ':')+1));
}

function logQueueEnter($deviceID, $name, $ip, $raceID, $randomRaceString, $apiKey){
	require 'config.php';
	$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to log MySQL.";
		return;
	}

	$user_id = getUserId($con, $deviceID);
	$now = time();
	$name_id = getStringId($con, $name);
	$ip_id = getStringId($con, $ip);
	$raceID = (int)$raceID;
	$randomRaceString_id = getStringId($con, $randomRaceString);
	$apiKey_id = getStringId($con, $apiKey);


	mysqli_query($con, "INSERT INTO `entered_queue` (`user`, `datetime`, `name`, `ip`, `race`, `random_race_string`, `api_key`)
			VALUES ($user_id, $now, $name_id, $ip_id, $raceID, $randomRaceString_id, $apiKey_id)");

}


function getUserId($con, $device_id){
	if(2  !== substr_count( $device_id , ':')){
		echo "ERROR: Invalid format of device id: $device_id";
		return null;
	}

	$device_id = substr($device_id, 0, strpos($device_id, ':', strpos($device_id, ':')+1));
	$deviceID = mysqli_real_escape_string($con, $device_id);
	$resultTmp = mysqli_query($con, "SELECT `user`.`id` FROM `user`, `string` WHERE `string`.`id` = `user`.`device_id` AND `string`.`value` = '$deviceID'");
	$row = $resultTmp->fetch_assoc();
	if(!$row){
		$devStringId = getStringId($con, $deviceID);
		if($devStringId === null)
			return null;

		mysqli_query($con, "INSERT INTO user (device_id) VALUES ($devStringId)");
		$resultTmp = mysqli_query($con, "SELECT `user`.`id` FROM `user`, `string` WHERE `string`.`id` = `user`.`device_id` AND `string`.`value` = '$deviceID'");
		$row = $resultTmp->fetch_assoc();
		if(!$row){
			echo "Failed to add new user with device id: $deviceID";
			return null;
		}
	}

	return $row['id'];
}


function getStringId($con, $string){
	$string = mysqli_real_escape_string($con, $string);
	$resultTmp = mysqli_query($con, "SELECT id FROM string WHERE value = '$string'");
	$row = $resultTmp->fetch_assoc();
	if(!$row){
		mysqli_query($con, "INSERT INTO string (value) VALUES ('$string')");
		$resultTmp = mysqli_query($con, "SELECT id FROM string WHERE value = '$string'");
		$row = $resultTmp->fetch_assoc();
		if(!$row){
			echo "Failed to add following string: $string";
			return null;
		}
	}

	return $row['id'];
}

function getGameStateStringId($con, $string){
	$string = mysqli_real_escape_string($con, $string);
	$resultTmp = mysqli_query($con, "SELECT id FROM game_state_string WHERE value = '$string'");
	$row = $resultTmp->fetch_assoc();
	if(!$row){
		mysqli_query($con, "INSERT INTO game_state_string (value) VALUES ('$string')");
		$resultTmp = mysqli_query($con, "SELECT id FROM game_state_string WHERE value = '$string'");
		$row = $resultTmp->fetch_assoc();
		if(!$row){
			echo "Failed to add following string: $string";
			return null;
		}
	}

	return $row['id'];
}

function getOfflineStringId($con, $string){
	$string = mysqli_real_escape_string($con, $string);
	$resultTmp = mysqli_query($con, "SELECT id FROM offline_string WHERE value = '$string'");
	$row = $resultTmp->fetch_assoc();
	if(!$row){
		mysqli_query($con, "INSERT INTO offline_string (value) VALUES ('$string')");
		$resultTmp = mysqli_query($con, "SELECT id FROM offline_string WHERE value = '$string'");
		$row = $resultTmp->fetch_assoc();
		if(!$row){
			echo "Failed to add following string: $string";
			return null;
		}
	}

	return $row['id'];
}