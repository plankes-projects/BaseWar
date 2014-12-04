<?php
//config ------------------------------------------------------------
require 'config.php';

$queueRequestFromClient = 2;	//seconds 1
$queueRequestFromClientTimeout = 2 * $queueRequestFromClient;
$teamSize = 1;
$gameStatsDelemiter = '+';
$gameStartTime = 10; //s
$gameTimeOut = $gameStartTime + 20;

$spawnStringTimeout = 60; //needed one round, we delete after 3 rounds to be sure
$spawnStringDelemiter = ';';
$spawnStringMin = 10;
$spawnStringMax = 2990;
$spawnStringNumMax = 300;  //to avoid dos attacks
$spawnStringNumAdditionalBuffer = 10;  //min 2 for first round
$spawnStringNumMaxUnitsPerRound = 1;

$allowInfoWithoutGame = true;

$randomRaceStringConnector = "-";

//config end --------------------------------------------------------

if(!isset($_GET['REQ']))
	return;

//handle teamsize request
if($_GET['REQ'] === 'teamSize'){
	echo $teamSize;
	return;
}

//kill timeouts
$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLUsername'],$config_var['mySQLPassword'],$config_var['mySQLDatabase']);
if (mysqli_connect_errno())
{
	echo "Failed to connect to MySQL (game)";
	return;
}

//handle get num user request
//for this we do not need a deviceID
if($_GET['REQ'] === 'playerNum'){
	include 'requests/playerNum.php';
	mysqli_close($con);
	return;
}

if(!isset($_GET['API_KEY']))
	return;

if(!isset($_POST['deviceID']))
	return;

if(!isset($_SERVER['REMOTE_ADDR']))
	$_SERVER['REMOTE_ADDR'] = 'null';

$apiKey = mysqli_real_escape_string($con, $_GET['API_KEY']);
$deviceID = mysqli_real_escape_string($con, $_POST['deviceID']);

$deviceIDWithoutSessionKey = substr($deviceID, 0, strpos($deviceID, ':', strpos($deviceID, ':')+1));
$sessionKey = substr($deviceID, strpos($deviceID, ':', strpos($deviceID, ':')+1)+1);

//delete old game entries
require 'scripts/deleteOldGameEntries.php';


//we do not require $requestToLoad to avoid attacks on path traversal
$requestToLoad = "requests/".$_GET['REQ'].".php";
foreach (glob("requests/*.php") as $filename) {
	if($filename === $requestToLoad){
		require $filename;
		break;
	}
}

//var_dump($con);
//mysqli_close($con);
?>

