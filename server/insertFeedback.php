<?php

//checking parameters =========================================================
if(!isset($_POST['deviceID']) || !isset($_POST['data'])){
	echo "POST parameter missing.";
	return;
}

if(!isset($_GET['VERSION'])){
	echo "GET parameter missing.";
	return;
}

if(!is_string($_POST['deviceID']) || !is_string($_POST['data'])){
	echo "POST parameter malformed.";
	return;
}

if(!is_string($_GET['VERSION'])){
	echo "GET parameter malformed.";
	return;
}

//parsing feedback string into datastructure ==================================
$delemiter = "<end/>\n";
$exp = explode($delemiter, $_POST['data']);

$arraySize = count($exp);
if(($arraySize+1) % 2 !== 0){
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
	echo "Failed to connect to MySQL. (feed1)";
	return;
}


//get user_id
require_once 'scripts/LogHelperFunctions.php';
$userId = getUserId($con, $_POST['deviceID']);
if(!$userId)
	return;

$now = time();
$versionId = getStringId($con, $_GET['VERSION']);

//add feedbacks
for($i = 0; $i < $arraySize; $i += 2){
	$questionId = getStringId($con, $exp[$i]);
	$answerId = getStringId($con, $exp[$i+1]);
	mysqli_query($con, "INSERT INTO feedback (user, datetime, version, question, answer)
			VALUES ($userId, $now, $versionId, $questionId, $answerId)");
}



