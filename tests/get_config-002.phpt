--TEST--
mh set_config/get_config function
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
mh_set_config(array('key' => 'val1'));
mh_set_config(array('key' => 'val2'));
$config = mh_get_config();
echo (int)($config['key'] === 'val2');
?>

--EXPECTF--
1
