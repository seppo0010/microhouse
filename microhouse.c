#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_microhouse.h"
#include "benchmark.h"
#include "controller.h"

static microhouse_singletons *singletons = NULL;;
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
	PHP_RINIT(microhouse),
	PHP_RSHUTDOWN(microhouse),
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
	mhcontroller_init(TSRMLS_C);
	return SUCCESS;
}

PHP_RINIT_FUNCTION(microhouse)
{
	singletons = emalloc(sizeof(microhouse_singletons));
	singletons->controller = NULL;
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(microhouse)
{
	if (singletons->controller)
	{
		Z_DELREF_P((zval*)singletons->controller);
	}
	efree(singletons);

	return SUCCESS;
}

void *microhouse_get_controller(TSRMLS_D)
{
	return singletons->controller;
}

void microhouse_set_controller(void *controller TSRMLS_DC)
{
	if (controller) {
		Z_ADDREF_P(controller);
	}
	if (singletons->controller) {
		Z_DELREF_P(singletons->controller);
	}
	singletons->controller = controller;
}

PHP_FUNCTION(microhouse_version)
{
	RETURN_STRING(PHP_MICROHOUSE_VERSION, 1);
}
