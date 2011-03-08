--TEST--
is_php version check
--SKIPIF--
<?php
if (!function_exists('is_php')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$version = '5.0.0'; // default
$test = (version_compare(PHP_VERSION, $version) < 0) ? FALSE : TRUE;
is_php($version);
echo (int)($test === is_php());
?>

--EXPECTF--
1
