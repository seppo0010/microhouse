--TEST--
load_class function
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$obj1 = new stdClass();
$obj1->a = 1;
mh_register_class('obj', $obj1);
echo (int)($obj1 === mh_load_class('obj'));
echo "\n";
echo (int)($obj1->a === 1);
?>

--EXPECTF--
1
1
