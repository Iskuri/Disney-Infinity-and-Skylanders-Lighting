<?php

$lines = file_get_contents("maskerminddata");

$lines = explode("\n",$lines);

echo "unsigned char packets[][32] = {\n";

foreach($lines as $line) {

	$bytes = explode(" ",trim($line));

	if(count($bytes) == 1) {
		continue;
	}

	echo "\t{";

	foreach($bytes as $key => $byte) {
		echo "0x".$byte;

		if($key != (count($bytes)-1)) {
			echo ",";
		}
	}

	echo "},\n";
}

echo "};\n";

echo "int packetCount = ".count($lines).";\n";

?>