<?php
$del = time()-$gameTimeOut;
$resultTmp = mysqli_query($con, "SELECT id, gameID, deviceID FROM game WHERE last_update < $del");

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
			VALUES($gameIDdel, '$deviceIDdel', $datetime, 'Timeout - Kick from game table.')");
	
	require_once 'scripts/LogHelperFunctions.php';
	logInsertGameInfo($deviceIDdel, $gameIDdel, "REMOVED FROM GAME TABLE ($gameTimeOut sec inactive)");
}
