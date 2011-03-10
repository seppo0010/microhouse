--TEST--
mh config_item function
--SKIPIF--
<?php
if (!function_exists('config_item')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
echo (int)(config_item('key') === NULL);
mh_set_config(array('key' => 'val'));
echo "\n";
echo (int)(config_item('key') === 'val');
echo "\n";
echo (int)(config_item('unknown') === NULL);
?>

--EXPECTF--
1
1
1
