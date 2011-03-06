#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_microhouse.h"
#include "benchmark.h"

static function_entry microhouse_functions[] = {
	PHP_FE(microhouse_version, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry microhouse_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_MICROHOUSE_EXTNAME,
	microhouse_functions,
	PHP_MINIT(microhouse),
	NULL,
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_MICROHOUSE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MICROHOUSE
ZEND_GET_MODULE(microhouse)
#endif

PHP_MINIT_FUNCTION(microhouse)
{
	mhbenchmark_init(TSRMLS_C);
	return SUCCESS;
}

PHP_FUNCTION(microhouse_version)
{
	RETURN_STRING(PHP_MICROHOUSE_VERSION, 1);
}
