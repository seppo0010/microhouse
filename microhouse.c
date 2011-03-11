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
	PHP_FE(mh_load_class, NULL)
	PHP_FE(mh_register_class, NULL)
	PHP_FE(is_loaded, NULL)
	PHP_FE(mh_get_config, NULL)
	PHP_FE(mh_set_config, NULL)
	PHP_FE(config_item, NULL)
/*
	PHP_FE(show_error, NULL)
	PHP_FE(show_404, NULL)
	PHP_FE(log_message, NULL)
	PHP_FE(set_status_header, NULL)
	PHP_FE(_exception_handler, NULL)
	PHP_FE(remove_invisible_characters, NULL)
	PHP_FE(is_really_writable, NULL)
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
static zval *config;
PHP_MINIT_FUNCTION(microhouse)
{
	mhbenchmark_init(TSRMLS_C);
	mhcontroller_init(TSRMLS_C);

	zend_hash_init(&_is_php, 4, NULL, NULL, 1);
	config = NULL;

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
	ALLOC_HASHTABLE(MH(classes));
	zend_hash_init(MH(classes), 10, NULL, NULL, 0);
	ALLOC_HASHTABLE(MH(is_loaded));
	zend_hash_init(MH(is_loaded), 10, NULL, NULL, 0);
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(microhouse)
{
	if (MH(controller))
	{
		Z_DELREF_P((zval*)MH(controller));
	}
	zend_hash_destroy(MH(classes));
	FREE_HASHTABLE(MH(classes));
	zend_hash_destroy(MH(is_loaded));
	FREE_HASHTABLE(MH(is_loaded));
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

static void add_loaded(char *class, int class_len TSRMLS_DC)
{
	char *lowername = emalloc(sizeof(char) * (1+class_len));
	int i;
	for (i = 0; i < class_len; ++i) {
		if (class[i] >= 'A' && class[i] <= 'Z') {
			lowername[i] = class[i] + 'a' - 'A';	
		} else {
			lowername[i] = class[i];
		}
 	}
	lowername[class_len] = '\0';

	zval *val;
	MAKE_STD_ZVAL(val);
	ZVAL_STRINGL(val, class, class_len, 1);
	zend_hash_add(MH(is_loaded), lowername, class_len + 1, &val, sizeof(zval*), NULL);
	efree(lowername);
}

static HashTable *is_loaded(TSRMLS_D)
{
	return MH(is_loaded);
}

PHP_FUNCTION(mh_load_class)
{
	char *file = NULL;
	int file_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
		RETURN_NULL();
	}

	if (strcasecmp("controller", file) == 0) {
		zval* z = (zval*)microhouse_get_controller(TSRMLS_C);
		RETURN_ZVAL(z, 0, 0);
	}

	if (zend_hash_exists(MH(classes), file, file_len + 1)) {
		zval **ret;
		zend_hash_find(MH(classes), file, file_len + 1, (void **) &ret);
		RETURN_ZVAL(*ret, 1, 0);
	}
	RETURN_NULL();
}

PHP_FUNCTION(mh_register_class)
{
	zval *obj;
	char *file = NULL;
	int file_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &file, &file_len, &obj) == FAILURE) {
		RETURN_NULL();
	}

	add_loaded(file, file_len);
	if (strcasecmp("controller", file) == 0) {
		microhouse_set_controller(obj TSRMLS_CC);
		RETURN_NULL();
	}

	Z_ADDREF_P(obj);
	zend_hash_add(MH(classes), file, file_len + 1, &obj, sizeof(zval*), NULL);
	RETURN_NULL();
}

PHP_FUNCTION(is_loaded)
{
	char *class = NULL;
	int class_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &class, &class_len) == FAILURE) {
		RETURN_NULL();
	}

	if (class != NULL) {
		add_loaded(class, class_len TSRMLS_CC);
	}

	HashTable *hash = is_loaded(TSRMLS_C);
	zval *ret;
	MAKE_STD_ZVAL(ret);
	array_init(ret);
	*Z_ARRVAL_P(ret) = *hash;
	RETURN_ZVAL(ret, 1, 0);
}

PHP_FUNCTION(mh_get_config)
{
	if (config == NULL) RETURN_NULL();
	RETURN_ZVAL(config, 1, 0);
}

PHP_FUNCTION(mh_set_config)
{
	zval *_config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_config) == FAILURE) {
		RETURN_NULL();
	}

	if (config != NULL) {
		Z_DELREF_P(config);
	}
	MAKE_STD_ZVAL(config);
	MAKE_COPY_ZVAL(&_config, config);
}

PHP_FUNCTION(config_item)
{
	char *item = NULL;
	int item_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &item, &item_len) == FAILURE) {
		RETURN_NULL();
	}

	if (strcmp(item, "subclass_prefix") == 0) {
		RETURN_STRINGL("MH", sizeof("MH")-1, 1); // expect not need this in the future
	}

	if (config == NULL) RETURN_NULL();

	HashTable *aht = HASH_OF(config);

	if (zend_hash_exists(aht, item, item_len + 1)) {
		zval **ret;
		zend_hash_find(aht, item, item_len + 1, (void **) &ret);
		RETURN_ZVAL(*ret, 1, 0);
	}
}

