--TEST--
loading microhouse
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
chdir(dirname(__FILE__) . '/../MicroHouse');
set_include_path(dirname(__FILE__) . '/../MicroHouse:' . get_include_path());
require 'index.php';
?>

--EXPECTF--
1
