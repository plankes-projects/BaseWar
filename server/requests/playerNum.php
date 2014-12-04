<?php
$resultTmp = mysqli_query($con, "SELECT COUNT(*) FROM game;");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;

$num = (int)$result[0]['COUNT(*)'];

$resultTmp = mysqli_query($con, "SELECT COUNT(*) FROM queue;");

$result = array();
while($row = $resultTmp->fetch_assoc())
	$result[] = $row;

$num += (int)$result[0]['COUNT(*)'];

if(!isset($_GET['REAL']) && $num < 20) {
	$randNum = rand(10, 20);
	if($randNum > $num)
		$num = $randNum;
}

$estimatedTimeInQueue = rand(60, 90); //double this amount if we use a real time

echo $num;
echo '-';
echo $estimatedTimeInQueue;