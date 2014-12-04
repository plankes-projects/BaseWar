<?php

//	_urlQueue 			= _host + "?API_KEY=" + _apiKey + "&REQ=queue";
//$time = time();

//trying to enter a game ------------------------------------------------
//delete old queued clients

$queueRequestFromClientTimeout = time() - $queueRequestFromClientTimeout;
mysqli_query($con,"DELETE FROM queue WHERE last_update < $queueRequestFromClientTimeout");

$neededPlayers = $teamSize * 2;
//is there a game for me already?
//delete me from game if there was a previous entry for me
$resultTmp = mysqli_query($con, "SELECT id, gameID, deviceID FROM game WHERE deviceID = '$deviceID' AND joined = 1");


$gameTimeouts = array();
while($row = $resultTmp->fetch_assoc())
	$gameTimeouts[] = $row;


$datetime = time();
foreach($gameTimeouts as $gameTimeout)
{
	mysqli_query($con, "DELETE FROM game WHERE id = {$gameTimeout['id']}");
	$gameIDdel = mysqli_real_escape_string($con, $gameTimeout['gameID']);
	$deviceIDdel = mysqli_real_escape_string($con, $gameTimeout['deviceID']);
	mysqli_query($con, "INSERT INTO game_info (gameID, deviceID, datetime, information)
			VALUES($gameIDdel, '$deviceIDdel', $datetime, 'Requeue - Kick from game table.')");
}



$resultTmp = mysqli_query($con, "	SELECT * FROM game
		WHERE deviceID = '$deviceID'");


$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


if(count($result) !== 0)
{
	//got a game, print game info
	$resultTmp = mysqli_query($con, "	SELECT * FROM game
			WHERE gameID = {$result[0]['gameID']} ORDER by playerID");


	$result = array();
	while($row = $resultTmp->fetch_assoc())
		$result[] = $row;



	if(count($result) !== $neededPlayers)
	{
		var_dump($result);
		echo "[Error] Got a game, but number of players missmatch.";
		mysqli_close($con);
		return;
	}

	$resultTmp = mysqli_query($con, "SELECT * FROM game_start WHERE gameID = '{$result[0]['gameID']}'");

	$gameStart = array();
	while($row = $resultTmp->fetch_assoc())
		$gameStart[] = $row;

	if(count($gameStart) !== 1)
	{
		echo "[Error] No start time found for this game.";
		mysqli_close($con);
		return;
	}

	echo ($gameStart[0]['datetime'] - time()) . PHP_EOL;
	foreach($result as $row)
	{
		/*
		 3456780			//datetime, zeit bis start
		0:1+Joe
		1				//ich
		2:0+123-132-41-12-412-423-1-412-1+Metro
		3:1+Sate
		*/
		if($row['deviceID'] === $deviceID)
			echo $row['playerID'] . PHP_EOL;
		else{
			if($row['raceID'] == 0){
				//random unit race
				echo $row['playerID'] . ':' . $row['raceID'] . '+' . $row['randomRaceString'] . '+' . $row['name'] . PHP_EOL;
			}else{
				echo $row['playerID'] . ':' . $row['raceID'] . '+' . $row['name'] . PHP_EOL;
			}
		}
	}
	//update my entry to joined
	mysqli_query($con, "UPDATE game SET joined = 1
			WHERE deviceID = '$deviceID'");

	mysqli_close($con);
	return;	//finished game init message for client
}

//am i in the queue?
$resultTmp = mysqli_query($con, "	SELECT * FROM queue
		WHERE deviceID = '$deviceID'");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;



$now = time();
$wasInQueue = '0';

if(count($result) !== 0)
{
	//i am in the queue, update last_update
	mysqli_query($con, "UPDATE queue SET last_update = $now
			WHERE deviceID = '$deviceID'");
	$wasInQueue = true;
}
else
{
	if(!isset($_POST['data']))
	{
		echo "[Error] No post data set.";
		mysqli_close($con);
		return;
	}

	$data = explode(';', $_POST['data']);
	if(count($data) === 2)
	{
		$raceID = (int)$data[0];
		$name = mysqli_real_escape_string($con, $data[1]);
		$randomRaceString = "";
	}
	else if(count($data) === 3){
		//new format with random string
		$raceID = (int)$data[0];
		$randomRaceString = mysqli_real_escape_string($con, $data[1]);
		$name = mysqli_real_escape_string($con, $data[2]);
	}
	else{
		echo "[Error] Post data has the wrong format.";
		mysqli_close($con);
		return;
	}

	//i am not in the queue, add me to the queue
	mysqli_query($con, "INSERT INTO queue (last_update, entered_queue, name, raceID, deviceID, api_key, randomRaceString)
			VALUES($now, $now, '$name', $raceID, '$deviceID', '$apiKey', '$randomRaceString')");

	$info = "Name: $name\nApikey: " . $apiKey . "\nRace: " . $raceID . "\nRandomRaceString: " . $randomRaceString . "\nIP: " . mysqli_real_escape_string($con, $_SERVER['REMOTE_ADDR']);
	mysqli_query($con, "INSERT INTO entered_queue (datetime, deviceID, information)
			VALUES($now, '$deviceID', '$info')");
	
	require_once 'scripts/LogHelperFunctions.php';
	logQueueEnter($deviceID, $name, $_SERVER['REMOTE_ADDR'], $raceID, $randomRaceString, $apiKey);
}


// try to create a game ------------------------------------
$resultTmp = mysqli_query($con, "SELECT * FROM queue WHERE api_key = '$apiKey' ORDER BY entered_queue ASC LIMIT 0, $neededPlayers");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


$numberOfFoundPlayers = count($result);
if($numberOfFoundPlayers === $neededPlayers)
{
	//jiihaaa start the game
	//we will join at next request
	//get next gameID
	$resultTmp = mysqli_query($con, "SELECT * FROM game_start ORDER BY gameID DESC LIMIT 0, 1");

	$gameIDMySQL = array();
	while($row = $resultTmp->fetch_assoc())
		$gameIDMySQL[] = $row;


	$gameID = 0;	//init case
	if(count($gameIDMySQL) === 1)
		$gameID = $gameIDMySQL[0]['gameID'] + 1;
	$playerID = 0;
	$information = "";
	$gameStartTime += time();
	require_once 'scripts/LogHelperFunctions.php';
	logCreateGame($gameID, $neededPlayers, $gameStartTime);
	foreach($result as $row)
	{
		$information .= $row['name'] . " - " . $row['deviceID'] . PHP_EOL;
		if(($playerID+1) === $teamSize)
			$information .= "vs" . PHP_EOL;
		
		//remove from queue
		$device_id = mysqli_real_escape_string($con, $row['deviceID']);
		mysqli_query($con, "DELETE FROM queue WHERE deviceID = '$device_id'");

		//add to game
		$name = mysqli_real_escape_string($con, $row['name']);
		$randomRaceString = mysqli_real_escape_string($con, $row['randomRaceString']);
		mysqli_query($con, "INSERT INTO game (gameID, playerID, gameStat, last_update, name, raceID, deviceID, joined, randomRaceString)
				VALUES($gameID, $playerID, '', $now, '$name', '{$row['raceID']}', '$device_id', 0, '$randomRaceString')");

		logAddUserToGame($gameID, $device_id, $playerID >= $teamSize);
		
		$playerID++;
	}

	//create gamestart
	$information = mysqli_real_escape_string($con, $information);
	mysqli_query($con, "INSERT INTO game_start (gameID, datetime, information)
			VALUES($gameID, $gameStartTime, '$information')");

	

}
if($wasInQueue === true)
{
	echo $numberOfFoundPlayers . "/" . $neededPlayers;
}
else
{
	echo $numberOfFoundPlayers . "/" . $neededPlayers;	//joined here, maybe other handling?
}