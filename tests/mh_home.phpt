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
ob_start();
require 'index.php';
$content = ob_get_contents();
ob_end_clean();
echo (int)preg_match('/Page rendered in [0-9]+\.[0-9]+/', $content);
?>

--EXPECTF--
1
