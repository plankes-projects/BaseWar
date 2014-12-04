<?php


//_urlGetGamestats 	= _host + "?API_KEY=" + _apiKey + "&REQ=getGamestat";
$resultTmp = mysqli_query($con, "	SELECT * FROM game
		WHERE deviceID = '$deviceID'");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;


if(count($result) === 0)
{
	echo "[Error] No gameID found.";
	mysqli_close($con);
	return;
}
$gameID = $result[0]['gameID'];
$result = mysqli_query($con, "SELECT * FROM game
		WHERE gameID = $gameID");
$connect = '';
while($row = mysqli_fetch_array($result))
{
	if($row['deviceID'] === $deviceID)
		continue;

	echo $connect . $row['playerID'] . $row['gameStat'];
	$connect = $gameStatsDelemiter;
}