--TEST--
load_class function
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$controller1 = new MHController();
$controller2 = mh_load_class('controller');
echo (int)($controller1 === $controller2);
?>

--EXPECTF--
1
