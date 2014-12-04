<?php

if(!isset($_POST['deviceID']) || !isset($_POST['data'])){
	echo "POST parameter missing.";
	return;
}

if(!is_string($_POST['deviceID']) || !is_string($_POST['data'])){
	echo "POST parameter malformed.";
	return;
}


//parsing offline log string into datastructure ==================================
$delemiter = "<n/>";
$turnDelemiter = "<g/>";
$timeDelemiter = "<t/>";

$exp = explode($delemiter, $_POST['data']);

$arraySize = count($exp);
if($arraySize < 3){
	//invalid feedback data form
	echo "invalid feedback data form 1";
	return;
}

//last entry should be ""
if($exp[$arraySize-1] !== ""){
	//invalid feedback data form
	echo "invalid feedback data form 2";
	return;
}
unset($exp[$arraySize-1]);
$arraySize--;

//insert into mysql databse ===================================================
require 'config.php';

$con=mysqli_connect($config_var['mySQLHost'], $config_var['mySQLLogUsername'],$config_var['mySQLLogPassword'],$config_var['mySQLLogDatabase']);
if (mysqli_connect_errno())
{
	echo "Failed to connect to MySQL. (off1)";
	return;
}


//get user_id
require_once 'scripts/LogHelperFunctions.php';
$userId = getUserId($con, $_POST['deviceID']);
if(!$userId)
	return;

$now = time();

//add game
//

//add feedbacks
$offline_game_id = 0;
for($i = 0; $i < $arraySize; $i++){
	
	$timeParts = explode($timeDelemiter, $exp[$i]);
	if(count($timeParts) !== 2){
		echo "malformed!";
		return;
	}
	
	$time = (int)$timeParts[0];
	$lineParts = explode($turnDelemiter, $timeParts[1]);	
	
	if($i === 0){
		if(count($lineParts) !== 1){
			echo "INFALID FORM 1";
			return;
		}
		$lineParts = $lineParts[0] === 'PVA' ? 'PVA' : 'PVP';
		//first line is game creation
		mysqli_query($con, "INSERT INTO offline_game (`user`, `mode`, `inserted`, `datetime`)
			VALUES ($userId, '$lineParts', $now, $time)");
		
		$offline_game_id = mysqli_insert_id($con);
	}else{
		
		if(count($lineParts) == 2){
			//turn mode
			$left = getOfflineStringId($con, $lineParts[0]);
			$right = getOfflineStringId($con, $lineParts[1]);
			
			mysqli_query($con, "INSERT INTO offline_turn_state (`datetime`, `offline_game`, `left`, `right`)
				VALUES ($time, $offline_game_id, $left, $right)");
		
		}else if(count($lineParts) == 1){
			//information mode
			$info = getOfflineStringId($con, $lineParts[0]);
			mysqli_query($con, "INSERT INTO offline_game_info (`datetime`, `offline_game`, `information`)
				VALUES ($time, $offline_game_id, $info)");
		}
	}
}

