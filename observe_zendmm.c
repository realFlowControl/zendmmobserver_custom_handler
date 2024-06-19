#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_observe_zendmm.h"
#include "Zend/zend_alloc.h"
#include <stddef.h>
#include <stdio.h>

ZEND_DECLARE_MODULE_GLOBALS(observe_zendmm)

void* observe_malloc(size_t size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
{
	void *ptr = NULL;
	if (OZ_G(custom_malloc)) {
		zend_mm_set_heap(OZ_G(original_heap));
		ptr = OZ_G(custom_malloc)(size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
		zend_mm_set_heap(OZ_G(observed_heap));
	} else {
		ptr = _zend_mm_alloc(OZ_G(original_heap), size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
	}
	printf("Allocated %zu bytes at %p\n", size, ptr);
	return ptr;
}

void observe_free(void* ptr ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
{
	if (OZ_G(custom_free))
	{
		zend_mm_set_heap(OZ_G(original_heap));
		OZ_G(custom_free)(ptr ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
		zend_mm_set_heap(OZ_G(observed_heap));
	} else {
		_zend_mm_free(OZ_G(original_heap), ptr ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
	}
	printf("Freed memory at %p\n", ptr);
}

void* observe_realloc(void* ptr, size_t size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
{
	void * new_ptr;
	if (OZ_G(custom_realloc))
	{
		zend_mm_set_heap(OZ_G(original_heap));
		new_ptr = OZ_G(custom_realloc)(ptr, size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
		zend_mm_set_heap(OZ_G(observed_heap));
	} else {
		new_ptr = _zend_mm_realloc(OZ_G(original_heap), ptr, size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
	}
	printf("Realloc of %zu bytes from %p to %p\n", size, ptr, new_ptr);
	return new_ptr;
}

size_t observe_gc(void)
{
	size_t size = 0;
	if (OZ_G(custom_gc)) {
		zend_mm_set_heap(OZ_G(original_heap));
		size = OZ_G(custom_gc)();
		zend_mm_set_heap(OZ_G(observed_heap));
	} else {
		size = zend_mm_gc(OZ_G(original_heap));
	}
	printf("ZendMM GC freed %zu bytes", size);
	return size;
}

void observe_shutdown(bool full, bool silent)
{
	if (OZ_G(custom_shutdown)) {
		zend_mm_set_heap(OZ_G(original_heap));
		OZ_G(custom_shutdown)(full, silent);
		zend_mm_set_heap(OZ_G(observed_heap));
	} else {
		zend_mm_shutdown(OZ_G(original_heap), full, silent);
	}
	printf("Shutdown happened: full -> %d, silent -> %d\n", full, silent);
}

void observe_zendmm_custom_handlers_init(void)
{
	if (OZ_G(observed_heap) != NULL) {
		return;
	}
	OZ_G(original_heap) = zend_mm_get_heap();
	if (zend_mm_is_custom_heap(OZ_G(original_heap))) {
		zend_mm_get_custom_handlers_ex(
			OZ_G(original_heap),
			&OZ_G(custom_malloc),
			&OZ_G(custom_free),
			&OZ_G(custom_realloc),
			&OZ_G(custom_gc),
			&OZ_G(custom_shutdown)
		);
	}
	printf("Prev handlers at %p, %p, %p, %p, %p\n", OZ_G(custom_malloc), OZ_G(custom_free), OZ_G(custom_realloc), OZ_G(custom_gc), OZ_G(custom_shutdown));
	OZ_G(observed_heap) = zend_mm_startup();
	zend_mm_set_custom_handlers_ex(
		OZ_G(observed_heap),
		observe_malloc,
		observe_free,
		observe_realloc,
		observe_gc,
		observe_shutdown
	);
	zend_mm_set_heap(OZ_G(observed_heap));
	printf("Heap at %p installed in ZendMM (orig at %p)\n", OZ_G(observed_heap), OZ_G(original_heap));
}

void observe_zendmm_custom_handlers_shutdown(void)
{
	if (OZ_G(observed_heap) == NULL) {
		return;
	}
	zend_mm_set_heap(OZ_G(original_heap));
	zend_mm_set_custom_handlers_ex(
		OZ_G(observed_heap),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	zend_mm_shutdown(OZ_G(observed_heap), true, true);
	OZ_G(observed_heap) = NULL;
	printf("Prev heap at %p restored in ZendMM\n", OZ_G(original_heap));
}

PHP_RINIT_FUNCTION(observe_zendmm)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(observe_zendmm)
{
	return SUCCESS;
}

PHP_MINIT_FUNCTION(observe_zendmm)
{
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(observe_zendmm)
{
	return SUCCESS;
}

PHP_GINIT_FUNCTION(observe_zendmm)
{
	observe_zendmm_custom_handlers_init();
}

PHP_GSHUTDOWN_FUNCTION(observe_zendmm)
{
	observe_zendmm_custom_handlers_shutdown();
}

zend_module_entry observe_zendmm_module_entry = {
	STANDARD_MODULE_HEADER,
	"observe_zendmm",					/* Extension name */
	NULL,					/* zend_function_entry */
	PHP_MINIT(observe_zendmm),							/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(observe_zendmm),							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(observe_zendmm),			/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(observe_zendmm),							/* PHP_RSHUTDOWN - Request shutdown */
	NULL,			/* PHP_MINFO - Module info */
	PHP_OBSERVE_ZENDMM_VERSION,		/* Version */
	PHP_MODULE_GLOBALS(observe_zendmm),
	PHP_GINIT(observe_zendmm),
	PHP_GSHUTDOWN(observe_zendmm),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_OBSERVE_ZENDMM
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(observe_zendmm)
#endif
