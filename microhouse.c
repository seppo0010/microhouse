#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_microhouse.h"
#include "benchmark.h"
#include "controller.h"

#include "ext/standard/php_versioning.h"

ZEND_DECLARE_MODULE_GLOBALS(microhouse);

static function_entry microhouse_functions[] = {
	PHP_FE(microhouse_version, NULL)
	PHP_FE(is_php, NULL)
/*
	PHP_FE(is_really_writable, NULL)
	PHP_FE(load_class, NULL)
	PHP_FE(is_loaded, NULL)
	PHP_FE(get_config, NULL)
	PHP_FE(config_item, NULL)
	PHP_FE(show_error, NULL)
	PHP_FE(show_404, NULL)
	PHP_FE(log_message, NULL)
	PHP_FE(set_status_header, NULL)
	PHP_FE(_exception_handler, NULL)
	PHP_FE(remove_invisible_characters, NULL)
*/
	{NULL, NULL, NULL}
};

zend_module_entry microhouse_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_MICROHOUSE_EXTNAME,
	microhouse_functions,
	PHP_MINIT(microhouse),
	PHP_MSHUTDOWN(microhouse),
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

static HashTable _is_php;
PHP_MINIT_FUNCTION(microhouse)
{
	mhbenchmark_init(TSRMLS_C);
	mhcontroller_init(TSRMLS_C);

	zend_hash_init(&_is_php, 4, NULL, NULL, 1);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(microhouse)
{
	HashTable *ht = &_is_php;
	zend_hash_destroy(ht);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(microhouse)
{
	MH(controller) = NULL;
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(microhouse)
{
	if (MH(controller))
	{
		Z_DELREF_P((zval*)MH(controller));
	}
	return SUCCESS;
}

void *microhouse_get_controller(TSRMLS_D)
{
	return MH(controller);
}

void microhouse_set_controller(void *controller TSRMLS_DC)
{
	if (controller) {
		Z_ADDREF_P(controller);
	}
	if (MH(controller)) {
		Z_DELREF_P(MH(controller));
	}
	MH(controller) = controller;
}

PHP_FUNCTION(microhouse_version)
{
	RETURN_STRING(PHP_MICROHOUSE_VERSION, 1);
}

PHP_FUNCTION(is_php)
{
	char *version = NULL;
	int version_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &version, &version_len) == FAILURE) {
		return;
	}

	if (version == NULL) {
		version = "5.0.0";
		version_len = sizeof(version);
	}

        if (zend_hash_exists(&_is_php, version, version_len + 1)) {
		int *ret;
		zend_hash_find(&_is_php, version, version_len + 1, (void **) &ret);
		RETURN_BOOL(*ret);
        }

	int result = php_version_compare(PHP_VERSION, version) >= 0;
	zend_hash_add(&_is_php, version, version_len + 1, &result, sizeof(int*), NULL);
	RETURN_BOOL(result);
}
