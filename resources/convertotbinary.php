<?php

$audioHex = fopen("audiodata","r");
$audioDataBin = fopen("audiodatabin","w");

while(!feof($audioHex)) {

	$hexLine = trim(fgets($audioHex));

	if(!$hexLine) {
		continue;
	}

	$hexBytes = explode(" ",$hexLine);

	foreach($hexBytes as $hexByte) {

		echo "Byting: ".hexdec($hexByte)."\n";

		fwrite($audioDataBin,chr(hexdec($hexByte)));
	}
}