/* observe_zendmm extension for PHP (c) 2023 Florian Engelhardt */

#ifndef PHP_OBSERVE_ZENDMM_H
# define PHP_OBSERVE_ZENDMM_H

extern zend_module_entry observe_zendmm_module_entry;
# define phpext_observe_zendmm_ptr &observe_zendmm_module_entry

# define PHP_OBSERVE_ZENDMM_VERSION "0.1.0"

#ifdef ZTS
#include "TSRM.h"
#endif

# if defined(ZTS) && defined(COMPILE_DL_OBSERVE_ZENDMM)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

ZEND_BEGIN_MODULE_GLOBALS(observe_zendmm)
	// the previous heap that was found in ZendMM
	zend_mm_heap* original_heap;
	// the custom handlers that might have been found in the previous heap
	void* (*custom_malloc)(size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
	void (*custom_free)(void* ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
	void* (*custom_realloc)(void *, size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
	size_t (*custom_gc)(void);
	void (*custom_shutdown)(bool, bool);
	// this is our heap that we install our custom handlers on and inject into
	// ZendMM
	zend_mm_heap* observed_heap;
ZEND_END_MODULE_GLOBALS(observe_zendmm)

extern ZEND_DECLARE_MODULE_GLOBALS(observe_zendmm)

#define OZ_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(observe_zendmm, v)

#endif	/* PHP_OBSERVE_ZENDMM_H */
