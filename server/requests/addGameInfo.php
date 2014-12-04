<?php

if(!isset($_POST['data']))
{
	echo "[Error] Post data is not set.";
	mysqli_close($con);
	return;
}

$resultTmp = mysqli_query($con, "	SELECT * FROM game
		WHERE deviceID = '$deviceID'");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


if(count($result) === 0)
{
	if(!$allowInfoWithoutGame)
	{
		echo "[Error] No gameID found.";
		mysqli_close($con);
		return;
	}
	$gameID = 0;
}
else
{
	$gameID = $result[0]['gameID'];
}

$information = mysqli_real_escape_string($con, $_POST['data']);
$datetime = time();
mysqli_query($con, "INSERT INTO game_info (gameID, deviceID, datetime, information)
		VALUES($gameID, '$deviceID', $datetime, '$information')");


$gameId = mysqli_query($con, "SELECT gameID FROM game WHERE deviceID = '$deviceID'");
$gameId = $gameId->fetch_assoc();
$gameId = $gameId['gameID'];

require_once 'scripts/LogHelperFunctions.php';
logInsertGameInfo($deviceID, $gameId, $_POST['data']);

echo 'ok';