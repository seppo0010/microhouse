<?php
$time = 0.0;
$count = 0;
for ($i = 0; $i < 1000; ++$i) {
	$contents = file_get_contents('http://localhost:8080/' . $argv[1] . '/index.php');
	$matches = NULL;
echo "$i\n";
	if (0 < preg_match('/Page rendered in ([0-9]+\.[0-9]+) seconds/', $contents, $matches)) {
		$time += (float)$matches[1];
		++$count;
	}
}
