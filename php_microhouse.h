#ifndef PHP_MICROHOUSE_H
#define PHP_MICROHOUSE_H 1

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_MICROHOUSE_VERSION "unstable"
#define PHP_MICROHOUSE_EXTNAME "MicroHouse"

PHP_MINIT_FUNCTION(microhouse);
PHP_MSHUTDOWN_FUNCTION(microhouse);
PHP_RINIT_FUNCTION(microhouse);
PHP_RSHUTDOWN_FUNCTION(microhouse);

PHP_FUNCTION(microhouse_version);

/* Common */
PHP_FUNCTION(is_php);
PHP_FUNCTION(mh_load_class);
PHP_FUNCTION(mh_register_class);
PHP_FUNCTION(is_loaded);
PHP_FUNCTION(mh_get_config);
PHP_FUNCTION(mh_set_config);
/*
PHP_FUNCTION(config_item);
PHP_FUNCTION(show_error);
PHP_FUNCTION(show_404);
PHP_FUNCTION(log_message);
PHP_FUNCTION(set_status_header);
PHP_FUNCTION(_exception_handler);
PHP_FUNCTION(remove_invisible_characters);
PHP_FUNCTION(is_really_writable);
*/
/* End Common */

ZEND_BEGIN_MODULE_GLOBALS(microhouse)
	void *controller;
	HashTable *classes;
	HashTable *is_loaded;
ZEND_END_MODULE_GLOBALS(microhouse)

#ifdef ZTS
#define MH(v) TSRMG(microhouse_globals_id, zend_microhouse_globals *, v)
#else
#define MH(v) (microhouse_globals.v)
#endif

void *microhouse_get_controller(TSRMLS_D);
void microhouse_set_controller(void *controller TSRMLS_DC);

extern zend_module_entry microhouse_module_entry;
#define phpext_microhouse_ptr &microhouse_module_entry

#endif
