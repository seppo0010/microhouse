#include "php.h"
#include "ext/date/php_date.h"
#include "php_microhouse.h"
#include "log.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhlog___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhlog_write_log, 0, 0, 0)
	ZEND_ARG_INFO(0, level) /* string */
	ZEND_ARG_INFO(0, msg) /* string */
	ZEND_ARG_INFO(0, php_error) /* bool */
ZEND_END_ARG_INFO()

static zend_class_entry *mhlog_ce;
static zend_object_handlers mhlog_object_handlers;

static PHP_METHOD(MHLog, __construct)
{
	zval *object = getThis();

	zval *_levels;
	MAKE_STD_ZVAL(_levels);
	array_init(_levels);
	add_assoc_long_ex(_levels, "ERROR", sizeof("ERROR"), 1);
	add_assoc_long_ex(_levels, "DEBUG", sizeof("DEBUG"), 2);
	add_assoc_long_ex(_levels, "INFO", sizeof("INFO"), 3);
	add_assoc_long_ex(_levels, "ALL", sizeof("ALL"), 4);
	add_property_zval_ex(object, "_levels", sizeof("_levels"), _levels TSRMLS_CC);
	Z_DELREF_P(_levels);
}

static PHP_METHOD(MHLog, write_log)
{
	if (MH(log_enabled) == 0 || MH(log_threshold) == 0) {
		RETURN_FALSE;
	}

	if (MH(log_path) == NULL || MH(log_date_format) == NULL) {
		RETURN_FALSE;
	}

	zval *object = getThis();
	char *level = NULL, *msg = NULL;
	int level_len, msg_len, php_error;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b", &level, &level_len, &msg, &msg_len, &php_error) == FAILURE) {
		RETURN_FALSE;
	}

	char *upperlevel = emalloc(sizeof(char) * (1+level_len));
	int i;
	for (i = 0; i < level_len; ++i) {
		if (level[i] >= 'a' && level[i] <= 'z') {
			upperlevel[i] = level[i] + 'A' - 'a';	
		} else {
			upperlevel[i] = level[i];
		}
 	}
	upperlevel[level_len] = '\0';

	zval *levels = zend_read_property(mhlog_ce, object, "_levels", sizeof("_levels") - 1, 1 TSRMLS_CC);
	if (levels == NULL) {
		RETURN_FALSE;
	}

	zval **ret = NULL;
	if (zend_hash_find(HASH_OF(levels), upperlevel, level_len + 1, (void **) &ret) != SUCCESS || ret == NULL) {
		RETURN_FALSE;
	}

	if (Z_TYPE_PP(ret) != IS_LONG) {
		RETURN_FALSE;
	}

	if (Z_LVAL_PP(ret) > MH(log_threshold)) {
		RETURN_FALSE;
	}

	char *filename = php_format_date("Y-m-d", sizeof("Y-m-d") - 1, time(NULL), 1 TSRMLS_CC);
	int filename_len = strlen(filename);

	int path_len = strlen(MH(log_path)) + filename_len;
	char *path = emalloc(sizeof(char) * (path_len+1));
	strcpy(path, MH(log_path));
	memcpy(path + strlen(MH(log_path)), filename, filename_len);
	efree(filename);
	path[path_len] = '\0';

	FILE *fp = fopen(path, "a");
	efree(path);

	if (fp) {
		char *date = php_format_date(MH(log_date_format), strlen(MH(log_date_format)), time(NULL), 1 TSRMLS_CC);
		int date_len = strlen(date);

		//LEVEL- YYYY-mm-dd --> <msg>\n
		int message_len = date_len + 13 + msg_len;
		char *message = emalloc(sizeof(char) * (message_len+1));
		memcpy(message, upperlevel, level_len);
		if (level_len == 4) memcpy(message+level_len, " - ", 3);
		else memcpy(message+level_len, "- ", 2);
		memcpy(message+7, date, date_len);
		memcpy(message+7+date_len, " --> ", sizeof(" --> ")-1);
		memcpy(message+7+date_len+sizeof(" --> ")-1, msg, msg_len);
		message[7+date_len+sizeof(" --> ")-1+msg_len] = '\n';
		message[7+date_len+sizeof(" --> ")-1+msg_len+1] = '\0';
		fwrite(message, sizeof(char), message_len, fp);

		fclose(fp);
		efree(message);
		efree(date);
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

static zend_function_entry php_microhouse_mhlog_class_functions[] = {
	PHP_ME(MHLog,               __construct,                      arginfo_mhlog___construct,         ZEND_ACC_PUBLIC)
	PHP_ME(MHLog,               write_log,                        arginfo_mhlog_write_log,           ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void mhlog_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "MHLog", php_microhouse_mhlog_class_functions);
	memcpy(&mhlog_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	mhlog_object_handlers.clone_obj = NULL;
	mhlog_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(mhlog_ce, "_levels", sizeof("_levels")-1, ZEND_ACC_PUBLIC TSRMLS_CC); // not easy to make protected
}
