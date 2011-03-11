--TEST--
load_class function
--SKIPIF--
<?php
if (!function_exists('mh_load_class')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
for ($i=0;$i<20;++$i) {
	$obj = new stdClass();
	$obj->a = $i;
	mh_register_class('obj' . $i, $obj);
}
for ($i=0;$i<20;++$i) {
	$obj = mh_load_class('obj' . $i);
	echo (int)($obj->a == $i);
	if ($i < 19)
		echo "\n";
}
?>

--EXPECTF--
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
1
