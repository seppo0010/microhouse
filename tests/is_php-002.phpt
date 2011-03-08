--TEST--
is_php version check
--SKIPIF--
<?php
if (!function_exists('is_php')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$version = '4.3.1';
$test = (version_compare(PHP_VERSION, $version) < 0) ? FALSE : TRUE;
echo (int)($test === is_php($version));
?>

--EXPECTF--
1
