<?php
$time = 0.0;
$count = 0;
$times = 1000;
if ($argc > 2) $times = $argv[2];
$port = 80;
if ($argc > 3) $port = $argv[3];

for ($i = 0; $i < $times; ++$i) {
	$contents = file_get_contents('http://localhost:' . $port . '/' . $argv[1] . '/index.php');
	$matches = NULL;
	if (0 < preg_match('/Page rendered in ([0-9]+\.[0-9]+) seconds/', $contents, $matches)) {
		$time += (float)$matches[1];
		++$count;
	}
}
echo number_format($time / $count, 6) , "\n";
