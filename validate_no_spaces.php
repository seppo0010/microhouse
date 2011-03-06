<?php
$files = array('php_microhouse.h','microhouse.c', 'benchmark.c');
foreach ($files as $file)
{
	$f = file_get_contents($file);
	$start = 0;
	while (($pos = strpos($f, "\n ", $start)) !== FALSE) {
		echo 'Found at pos ' . $pos , "\n";
		$start = $pos+1;
	}
}
