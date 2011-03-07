--TEST--
controller class
--SKIPIF--
<?php
if (!class_exists('MHBenchmark')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
echo (int)(MHController::get_instance() === NULL);
$controller = new MHController();
$controller->a = 1;
$controller = MHController::get_instance();
echo "\n";
echo (int)($controller->a == 1);
?>

--EXPECTF--
1
1
