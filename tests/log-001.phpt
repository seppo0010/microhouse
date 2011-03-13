--TEST--
log class
--SKIPIF--
<?php
if (!class_exists('MHLog')) die("skip microhouse extension not available\n");
?>
--FILE--
<?php
$path = dirname(__FILE__) . '/' . date('Y-m-d');
@unlink($path);
$log = new MHLog();
ini_set('microhouse.log_threshold', 1);
ini_set('microhouse.log_path', dirname(__FILE__) . '/');
ini_set('microhouse.log_enabled', 1);
ini_set('microhouse.log_date_format', 'Y-m-d H:i:s');
$log->write_log('error', 'This is an error!');
$log->write_log('debug', 'This should not get logged');
ini_set('microhouse.log_threshold', 4);
$log->write_log('debug', 'This should get logged');
$log->write_log('test', 'This should not get logged');

$contents = file_get_contents($path);
$date = '(?:[0-9]{4}-[0-1][0-9]-[0-3][0-9] [0-2][0-9]:[0-5][0-9]:[0-5][0-9])';
echo (int)(0 < preg_match("/^ERROR- $date --> This is an error!\nDEBUG- $date --> This should get logged\n$/", $contents));
@unlink($path);
?>

--EXPECTF--
1
