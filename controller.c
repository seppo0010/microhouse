#include "php.h"
#include "php_microhouse.h"
#include "controller.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhcontroller___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhcontroller_STATIC_get_instance, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_class_entry *mhcontroller_ce;
static zend_object_handlers mhcontroller_object_handlers;

static PHP_METHOD(MHController, __construct)
{
	zval *object = getThis();
	microhouse_set_controller(object);
}

static PHP_METHOD(MHController, get_instance)
{
	zval *controller = (zval*)microhouse_get_controller();
	if (controller == NULL) {
		RETURN_NULL();
	} else {
		RETURN_ZVAL(controller, 0, 0);
	}
}

static zend_function_entry php_microhouse_mhcontroller_class_functions[] = {
	PHP_ME(MHController,               __construct,                      arginfo_mhcontroller___construct,         ZEND_ACC_PUBLIC)
	PHP_ME(MHController,               get_instance,                     arginfo_mhcontroller_STATIC_get_instance, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};


void mhcontroller_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "MHController", php_microhouse_mhcontroller_class_functions);
	memcpy(&mhcontroller_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	mhcontroller_object_handlers.clone_obj = NULL;
	mhcontroller_ce = zend_register_internal_class(&ce TSRMLS_CC);
}
