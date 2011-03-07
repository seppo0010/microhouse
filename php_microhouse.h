#ifndef PHP_MICROHOUSE_H
#define PHP_MICROHOUSE_H 1

#define PHP_MICROHOUSE_VERSION "unstable"
#define PHP_MICROHOUSE_EXTNAME "MicroHouse"

PHP_MINIT_FUNCTION(microhouse);
PHP_MSHUTDOWN_FUNCTION(microhouse);
PHP_RINIT_FUNCTION(microhouse);
PHP_RSHUTDOWN_FUNCTION(microhouse);

PHP_FUNCTION(microhouse_version);

/* Common */
PHP_FUNCTION(is_php);
/*
PHP_FUNCTION(is_really_writable);
PHP_FUNCTION(load_class);
PHP_FUNCTION(is_loaded);
PHP_FUNCTION(get_config);
PHP_FUNCTION(config_item);
PHP_FUNCTION(show_error);
PHP_FUNCTION(show_404);
PHP_FUNCTION(log_message);
PHP_FUNCTION(set_status_header);
PHP_FUNCTION(_exception_handler);
PHP_FUNCTION(remove_invisible_characters);
*/
/* End Common */

typedef struct microhouse_singletons {
	void *controller;
} microhouse_singletons;

void *microhouse_get_controller(TSRMLS_D);
void microhouse_set_controller(void *controller TSRMLS_DC);

extern zend_module_entry microhouse_module_entry;
#define phpext_microhouse_ptr &microhouse_module_entry

#endif
