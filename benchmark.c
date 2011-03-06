#include "php.h"
#include "php_microhouse.h"
#include "benchmark.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhbenchmark___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhbenchmark_mark, 0, 0, 0)
	ZEND_ARG_INFO(0, name) /* string */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhbenchmark_elapsed_time, 0, 0, 0)
	ZEND_ARG_INFO(0, point1) /* string */
	ZEND_ARG_INFO(0, point2) /* string */
	ZEND_ARG_INFO(0, decimals) /* integer */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhbenchmark_memory_usage, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_class_entry *mhbenchmark_ce;
static zend_object_handlers mhbenchmark_object_handlers;

#define MICRO_IN_SEC 1000000.00

static PHP_METHOD(MHBenchmark, __construct)
{
	zval *object = getThis();
	zval *marker;

	MAKE_STD_ZVAL(marker);
	array_init(marker);
	add_property_zval_ex(object, "marker", sizeof("marker"), marker TSRMLS_CC);
	Z_DELREF_P(marker);
}

static void mhbenchmark_mark(zval *markers, char *marker, int marker_len)
{
	struct timeval tp = {0};
	if (gettimeofday(&tp, NULL)) {
		return;
	}
	char ret[100];
	snprintf(ret, 100, "%.8F %ld", tp.tv_usec / MICRO_IN_SEC, tp.tv_sec);
	add_assoc_string_ex(markers, marker, 1+marker_len, ret, 1);
}

static PHP_METHOD(MHBenchmark, mark)
{
	zval *object = getThis();
	zval **markers;
	char *marker;
	int marker_len;

	if (zend_hash_find(Z_OBJPROP_P(object), "marker", sizeof("marker"), (void **)&markers) == FAILURE) {
		return;
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &marker, &marker_len) == FAILURE) {
		return;
	}

	mhbenchmark_mark(*markers, marker, marker_len);
}

static PHP_METHOD(MHBenchmark, elapsed_time)
{
	zval *object = getThis();
	zval **markers;
	char *point1, *point2;
	size_t point1_len = 0, point2_len = 0;
	long decimals = 4;
	HashTable *hash;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssl", &point1, &point1_len, &point2, &point2_len, &decimals) == FAILURE) {
		RETURN_STRING("", 1);
	}

	if (point1_len == 0) {
		RETURN_STRING("{elapsed_time}", 1);
	}

	if (zend_hash_find(Z_OBJPROP_P(object), "marker", sizeof("marker"), (void **)&markers) == FAILURE) {
		RETURN_STRING("", 1);
	}

	hash = Z_ARRVAL_PP(markers);
	if (!zend_hash_exists(hash, point1, point1_len + 1)) {
		RETURN_STRING("", 1);
	}

	if (!zend_hash_exists(hash, point2, point2_len + 1)) {
		mhbenchmark_mark(*markers, point2, point2_len);
	}

	zval **p1_value, **p2_value;
	zend_hash_find(hash, point1, point1_len + 1, (void **) &p1_value);
	zend_hash_find(hash, point2, point2_len + 1, (void **) &p2_value);

	double sm, em;
	long ss, es;
	sscanf(Z_STRVAL_PP(p1_value), "%lf %ld", &sm, &ss);
	sscanf(Z_STRVAL_PP(p2_value), "%lf %ld", &em, &es);

	zval *func;
	MAKE_STD_ZVAL(func);
	ZVAL_STRINGL(func, "number_format", sizeof("number_format")-1, 1);
	zval *arg[2];
	MAKE_STD_ZVAL(arg[0]);
	ZVAL_DOUBLE(arg[0], (em + (double)es - sm - (double)ss))
	MAKE_STD_ZVAL(arg[1]);
	ZVAL_LONG(arg[1], decimals);
	if (call_user_function(EG(function_table), NULL, func, return_value, 2, arg TSRMLS_CC) == FAILURE) {
		ZVAL_NULL(return_value);
	}
	zval_ptr_dtor(&func);
	zval_ptr_dtor(&arg[0]);
	zval_ptr_dtor(&arg[1]);
}

static PHP_METHOD(MHBenchmark, memory_usage)
{
	RETURN_STRING("{memory_usage}", 1);
}

static zend_function_entry php_microhouse_mhbenchmark_class_functions[] = {
	PHP_ME(MHBenchmark,               __construct,                      arginfo_mhbenchmark___construct,         ZEND_ACC_PUBLIC)
	PHP_ME(MHBenchmark,               mark,                             arginfo_mhbenchmark_mark,                ZEND_ACC_PUBLIC)
	PHP_ME(MHBenchmark,               elapsed_time,                     arginfo_mhbenchmark_elapsed_time,        ZEND_ACC_PUBLIC)
	PHP_ME(MHBenchmark,               memory_usage,                     arginfo_mhbenchmark_memory_usage,        ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void mhbenchmark_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "MHBenchmark", php_microhouse_mhbenchmark_class_functions);
	memcpy(&mhbenchmark_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	mhbenchmark_object_handlers.clone_obj = NULL;
	mhbenchmark_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(mhbenchmark_ce, "marker", sizeof("marker")-1, ZEND_ACC_PUBLIC TSRMLS_CC);
}
