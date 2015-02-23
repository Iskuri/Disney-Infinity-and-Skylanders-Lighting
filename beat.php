<?php

if(posix_getuid() != 0) {
	die("Please run this application as root/sudo\n");
}

if(!isset($argv[1])) {
	throw new Exception("No file selected");
}

$audioFile = $argv[1];
$fileType = mime_content_type($audioFile);

if($fileType != "audio/x-wav") {
	throw new Exception("Wrong file type selected");
}

$lightShowProc = proc_open(sprintf("./lightshow",$audioFile),array(0 => array("pipe", "r"),1 => array("pipe", "w")),$lightPipes);

$lightsLine = explode(" ",str_replace("\n","",fgets($lightPipes[1])));

if($lightsLine[0] != "lights") {
	throw new Exception("Output formatting error");
}

$lightCount = $lightsLine[1];

$lightCount = max($lightCount,3);

$soxProc = proc_open(sprintf("sox %s -t dat -r 32 -c %d - 2>&1",$audioFile,$lightCount),array(0 => array("pipe", "r"),1 => array("pipe", "w")),$pipes);

$soxOut = $pipes[1];

$channelLast = array();

$startTime = microtime(true);

proc_open(sprintf("play %s 2>&1 &",$audioFile),array(0 => array("pipe", "r"),1 => array("pipe", "w")),$pipesAudio);

echo "Starting the music!\n";

while(!feof($soxOut)) {

	$line = trim(str_replace(array("\n","\r"),"",fgets($soxOut)));

	if(strpos($line,";") === 0 || !$line) {
		continue;
	}

	// $data = explode(" ",$line);
	$data = preg_split("/ +/",$line);
	$time = $data[0];

	$currTime = microtime(true)-$startTime;

	while($currTime < $time) {
		$currTime = microtime(true)-$startTime;
	}

	// var_dump("It's $currTime");

	for($i = 1 ; $i <= $lightCount ; $i++) {
		// if(isset($channelLast[$i-1]) && $data[$i] < $channelLast[$i-1]) {
		if($data[$i] > 0) {
			fwrite($lightPipes[0],$i."\n");
		}

		$channelLast[$i-1] = $data[$i];
	}
}