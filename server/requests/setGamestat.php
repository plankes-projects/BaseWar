<?php

//_urlSetGamestat 	= _host + "?API_KEY=" + _apiKey + "&REQ=setGamestat";
if(!isset($_POST['data']))
{
	echo "[Error] Post data is not set.";
	mysqli_close($con);
	return;
}
$now = time();
$gameStat = mysqli_real_escape_string($con, $_POST['data']);
mysqli_query($con, "UPDATE game SET gameStat = '$gameStat', last_update = $now
		WHERE deviceID = '$deviceID'");

$gameId = mysqli_query($con, "SELECT gameID FROM game WHERE deviceID = '$deviceID'");
$gameId = $gameId->fetch_assoc();
$gameId = $gameId['gameID'];

require_once 'scripts/LogHelperFunctions.php';
logTurnState($deviceID, $gameId, $_POST['data']);

echo 'ok';