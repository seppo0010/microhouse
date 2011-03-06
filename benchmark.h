#ifndef PHP_MHBENCHMARK_H
#define PHP_MHBENCHMARK_H

typedef struct php_mhbenchmark {
	zval *markers;
	zend_object zo;
} php_mhbenchmark;

void mhbenchmark_init(TSRMLS_D);
#endif PHP_MHBENCHMARK_H
