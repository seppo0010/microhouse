--TEST--
mh set_config/get_config function
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$config = mh_get_config();
echo (int)($config === NULL);
mh_set_config(array('key' => 'val'));
$config = mh_get_config();
echo "\n";
echo (int)($config['key'] === 'val');
?>

--EXPECTF--
1
1
