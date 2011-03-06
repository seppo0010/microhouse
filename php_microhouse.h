#ifndef PHP_MICROHOUSE_H
#define PHP_MICROHOUSE_H 1

#define PHP_MICROHOUSE_VERSION "unstable"
#define PHP_MICROHOUSE_EXTNAME "MicroHouse"

PHP_MINIT_FUNCTION(microhouse);

PHP_FUNCTION(microhouse_version);

extern zend_module_entry microhouse_module_entry;
#define phpext_microhouse_ptr &microhouse_module_entry

#endif
