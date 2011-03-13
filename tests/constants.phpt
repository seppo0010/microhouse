--TEST--
MicroHouse constants
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
if (ini_get('microhouse.self') == FALSE) die("skip SELF constant -- not in ini\n");
if (ini_get('microhouse.ext') == FALSE) die("skip EXT constant -- not in ini\n");
if (ini_get('microhouse.basepath') == FALSE) die("skip BASEPATH constant -- not in ini\n");
if (ini_get('microhouse.fcpath') == FALSE) die("skip FCPATH constant -- not in ini\n");
if (ini_get('microhouse.apppath') == FALSE) die("skip APPPATH constant -- not in ini\n");
?>
--FILE--
<?php
echo (int)(defined('SELF') && SELF == ini_get('microhouse.self'));
echo "\n";
echo (int)(defined('EXT') && EXT == ini_get('microhouse.ext'));
echo "\n";
echo (int)(defined('BASEPATH') && BASEPATH == ini_get('microhouse.basepath'));
echo "\n";
echo (int)(defined('FCPATH') && FCPATH == ini_get('microhouse.fcpath'));
echo "\n";
echo (int)(defined('APPPATH') && APPPATH == ini_get('microhouse.apppath'));
?>

--EXPECTF--
1
1
1
1
1
