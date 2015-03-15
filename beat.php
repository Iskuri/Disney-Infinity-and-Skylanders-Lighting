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
$peaksTroughs = array();

$startTime = microtime(true);

echo "Processing the music!\n";

$tempPlayout = tempnam("/tmp/","playOut");

// proc_open(sprintf("play %s 2>&1 >/dev/null | tr  '\\r' '\\n' > %s &",$audioFile,$tempPlayout),array(0 => array("pipe", "r"),1 => array("pipe", "w")),$pipesAudio);
$fifoFile = "resources/playfifo";

// posix_mkfifo($fifoFile, 777);
$pFile = popen(sprintf("/usr/bin/play %s 2>&1 >/dev/null | tr  '\\r' '\\n' &",$audioFile),"r");

// $fifo = fopen("playfifo","r");

// echo "Getting ready!\n";

// while(true) {
	
// 	if(strpos(fgets($fifo),"In:") !== false) {
// 		break;
// 	}
// }

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
		// if($data[$i] > 0) {
		if($data[$i] > (($peaksTroughs[$i-1]["peak"]+$peaksTroughs[$i-1]["trough"])/2)) {
			fwrite($lightPipes[0],$i."\n");
		}

		$channelLast[$i-1] = $data[$i];
		if(isset($peaksTroughs[$i-1]["peak"])) {
			$peaksTroughs[$i-1]["peak"] = max($peaksTroughs[$i-1]["peak"],$data[$i]);
			$peaksTroughs[$i-1]["trough"] = min($peaksTroughs[$i-1]["trough"],$data[$i]);
		} else {
			$peaksTroughs[$i-1]["peak"] = $data[$i];
			$peaksTroughs[$i-1]["trough"] = $data[$i];
		}

	}

	// var_dump($peaksTroughs);
}

unlink($tempListOut);