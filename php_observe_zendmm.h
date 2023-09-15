/* observe_zendmm extension for PHP (c) 2023 Florian Engelhardt */

#ifndef PHP_OBSERVE_ZENDMM_H
# define PHP_OBSERVE_ZENDMM_H

extern zend_module_entry observe_zendmm_module_entry;
# define phpext_observe_zendmm_ptr &observe_zendmm_module_entry

# define PHP_OBSERVE_ZENDMM_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_OBSERVE_ZENDMM)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_OBSERVE_ZENDMM_H */
