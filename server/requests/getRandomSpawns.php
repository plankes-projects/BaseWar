<?php

if(!isset($_POST['data'])){
	mysqli_close($con);
	echo "[ERROR] Data not set";
	return;
}
if(strlen($_POST['data']) < 2){
	mysqli_close($con);
	echo "[ERROR] data is too small";
	return;
}

$hisTeam = substr($_POST['data'], 0, 1);
$hisTeam = $hisTeam === "0"; // "0" or "1"
$wave = substr($_POST['data'], 1);

//get playerid to calculate with which team we have to response
$resultTmp = mysqli_query($con, "	SELECT playerID, gameID FROM game
		WHERE deviceID = '$deviceID' ");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


if(count($result) !== 1){
	mysqli_close($con);
	echo "[ERROR] Did not found the game entry";
	return;
}

//player ids 0...x
$playerID = $result[0]['playerID'];
$gameID = intval($result[0]['gameID']);
$team = false;
if($teamSize > $playerID)
	$team = true;

//switch team to request
if(!$hisTeam)
	$team = !$team;

//transform team to int for mysql
$team = $team ? 1 : 0;
$wave = intval($wave);


//try to get randoms for this wave
$resultTmp = mysqli_query($con, "	SELECT spawnString FROM spawn_sync
		WHERE team = $team AND gameID = $gameID AND wave = $wave");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


if(count($result) > 1){
	mysqli_close($con);
	echo "[ERROR] Found too much spawn syncs";
	return;
} else if(count($result) === 1){
	echo $result[0]['spawnString'];
	mysqli_close($con);
	return;
}

//not existing, generate the string and add it

$datetime = time();
$spawnStringTimeout = $datetime - $spawnStringTimeout;
mysqli_query($con,"DELETE FROM spawn_sync WHERE created < $spawnStringTimeout");

$randomString = "";
$connect = '';
$anz = ($spawnStringNumMaxUnitsPerRound * $wave) + $spawnStringNumAdditionalBuffer;
if($anz > $spawnStringNumMax)
	$anz = $spawnStringNumMax;
for($i = 0; $i < $anz; $i++){
	$randomString .= $connect . rand($spawnStringMin, $spawnStringMax);
	$connect = $spawnStringDelemiter;
}
mysqli_query($con, "INSERT INTO spawn_sync (team, gameID, wave, created, spawnString)
		VALUES($team, $gameID, $wave, $datetime, '$randomString')");


echo $randomString;