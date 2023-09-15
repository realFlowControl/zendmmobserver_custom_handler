#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_observe_zendmm.h"
#include "Zend/zend_alloc.h"
#include <stddef.h>
#include <stdio.h>

// the previous heap that was found in ZendMM
zend_mm_heap* prev_heap = NULL;

// the custom handlers that might have been found in the previous heap in case
// there are any
void* (*custom_malloc)(size_t) = NULL;
void (*custom_free)(void*) =  NULL;
void* (*custom_realloc)(void *, size_t) = NULL;
size_t (*custom_gc)(void) = NULL;
void (*custom_shutdown)(bool, bool) = NULL;

// this is our heap that we install our custom handlers on and inject into
// ZendMM
zend_mm_heap* heap = NULL;

void* observe_malloc(size_t size)
{
	void *ptr = NULL;
	if (custom_malloc) {
		ptr = custom_malloc(size);
	} else {
		ptr = _zend_mm_alloc(prev_heap, size);
	}
	printf("Allocated %zu bytes at %p\n", size, ptr);
	return ptr;
}

void observe_free(void* ptr)
{
	if (custom_free)
	{
		custom_free(ptr);
	} else {
		_zend_mm_free(prev_heap, ptr);
	}
	printf("Freed memory at %p\n", ptr);
}

void* observe_realloc(void* ptr, size_t size)
{
	void * new_ptr;
	if (custom_realloc)
	{
		new_ptr = custom_realloc(ptr, size);
	} else {
		new_ptr = _zend_mm_realloc(prev_heap, ptr, size);
	}
	printf("Realloc of %zu bytes from %p to %p\n", size, ptr, new_ptr);
	return new_ptr;
}

size_t observe_gc()
{
	size_t size = 0;
	if (custom_gc) {
		size = custom_gc();
	} else {
		size = zend_mm_gc(prev_heap);
	}
	printf("ZendMM GC freed %zu bytes", size);
	return size;
}

void observe_shutdown(bool full, bool silent)
{
	if (custom_shutdown) {
		custom_shutdown(full, silent);
	} else {
		zend_mm_shutdown(prev_heap, full, silent);
	}
	printf("Shutdown happened: full -> %d, silent -> %d\n", full, silent);
}

PHP_RINIT_FUNCTION(observe_zendmm)
{
	prev_heap = zend_mm_get_heap();
	if (zend_mm_is_custom_heap(prev_heap)) {
		zend_mm_get_custom_handlers_ex(
			prev_heap,
			&custom_malloc,
			&custom_free,
			&custom_realloc,
			&custom_gc,
			&custom_shutdown
		);
	}
	printf("Prev handlers at %p, %p, %p, %p, %p\n", custom_malloc, custom_free, custom_realloc, custom_gc, custom_shutdown);
	heap = zend_mm_heap_create();
	zend_mm_set_custom_handlers_ex(
		heap,
		observe_malloc,
		observe_free,
		observe_realloc,
		observe_gc,
		observe_shutdown
	);
	zend_mm_set_heap(heap);
	printf("Heap at %p installed in ZendMM (orig at %p)\n", heap, prev_heap);
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(observe_zendmm)
{
	zend_mm_set_heap(prev_heap);
	zend_mm_heap_free(heap);
	heap = NULL;
	printf("Prev heap at %p restored in ZendMM\n", prev_heap);
	return SUCCESS;
}

zend_module_entry observe_zendmm_module_entry = {
	STANDARD_MODULE_HEADER,
	"observe_zendmm",					/* Extension name */
	NULL,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(observe_zendmm),			/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(observe_zendmm),							/* PHP_RSHUTDOWN - Request shutdown */
	NULL,			/* PHP_MINFO - Module info */
	PHP_OBSERVE_ZENDMM_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OBSERVE_ZENDMM
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(observe_zendmm)
#endif
