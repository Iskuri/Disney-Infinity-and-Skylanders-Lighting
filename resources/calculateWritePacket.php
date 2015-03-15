<?php

$packetDump = file_get_contents("writePacketDump2");

$lines = explode("\n",$packetDump);

$started = false;

$setups = array();

$setupsCount = 0;

$setupNames = array("sheep creep", "broccoli guy", "chompy mage", "cuckoo clocker");

foreach($lines as $line) {

	if(!$line) {
		continue;
	}

	$split = explode(":",$line);
	$data = explode(" ",trim($split[1]));
	$dataBin = array();

	if($data[0] == '57') {
		$started = true;
	}

	if($started == true) {

		if($data[0] == '57' || $data[0] == '51') {
			$setups[$setupsCount][] = trim($split[1]);
		} else {
			$setupsCount++;
		}
	}
	// foreach(explode(" ",$data) as $dataHex) {
	// 	$dataBin[] = chr(hexdec($dataHex));
	// }

}

$keyedSetup = array();

$counter = 0;
foreach($setups as $setupData) {

	// echo $setupNames[$counter]."\n";
	// foreach($setupData as $setupLine) {
	// 	echo $setupLine."\n";
	// }

	$keyedSetup[$setupNames[$counter]][] = $setupData;

	$counter++;

	if($counter >= count($setupNames)) {
		$counter = 0;
	}
}

var_dump($keyedSetup);