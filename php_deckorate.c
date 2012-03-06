#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_scandir.h"
#include "zend_globals.h"
#include "zend_ini_scanner.h"
#include "zend_hash.h"
#include "ext/standard/info.h"
#include "SAPI.h"
#include "malloc.h"

#include "php_deckorate.h"

/* {{{ php_deckorate_init_globals */
static void php_deckorate_init_globals(zend_deckorate_globals* deckorate_globals TSRMLS_DC)
{
}
/* }}} */



/* {{{ php_deckorate_shutdown_globals */
static void php_deckorate_shutdown_globals(zend_deckorate_globals* deckorate_globals TSRMLS_DC)
{
	/* nothing ? */
}
/* }}} */

/* {{{ ini entries */
PHP_INI_BEGIN()
PHP_INI_END()
/* }}} */

/* {{{ arginfo static macro */
#if PHP_MAJOR_VERSION > 5 || PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 3
#define DECKORATE_ARGINFO_STATIC
#else
#define DECKORATE_ARGINFO_STATIC static
#endif
/* }}} */

/* {{{ arginfo */
/* }}} */


/* {{{ deckorate_functions[]
 *
 * Every user visible function must have an entry in deckorate_functions[].
 */
zend_function_entry deckorate_functions[] = {
	{NULL, NULL, NULL}	/* Must be the last line in deckorate_functions[] */
};
/* }}} */

/* {{{ deckorate_module_entry
 */
zend_module_entry deckorate_module_entry = {
	STANDARD_MODULE_HEADER,
	"deckorate",
	deckorate_functions,
	PHP_MINIT(deckorate),
	PHP_MSHUTDOWN(deckorate),
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	PHP_MINFO(deckorate),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_DECKORATE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DECKORATE
ZEND_GET_MODULE(deckorate)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(deckorate)
{
	ZEND_INIT_MODULE_GLOBALS(deckorate, php_deckorate_init_globals, php_deckorate_shutdown_globals);

	REGISTER_INI_ENTRIES();
	
	deckorate_init(TSRMLS_C);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 *  */
PHP_MSHUTDOWN_FUNCTION(deckorate)
{
#ifdef ZTS
	ts_free_id(deckorate_globals_id);
#else
	php_deckorate_shutdown_globals(&deckorate_globals);
#endif
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION(deckorate) */
PHP_RSHUTDOWN_FUNCTION(deckorate)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(deckorate)
{
	php_info_print_table_start();

	php_info_print_table_header(2, "deckorate support", "enabled");
	php_info_print_table_row(2, "version", PHP_DECKORATE_VERSION);

	php_info_print_table_end();
}
/* }}} */

