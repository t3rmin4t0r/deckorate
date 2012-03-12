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

/* {{{ ZEND_DECLARE_MODULE_GLOBALS(deckorate) */
ZEND_DECLARE_MODULE_GLOBALS(deckorate)
/* }}} */

/* {{{ php_deckorate_init_globals */
static void php_deckorate_init_globals(zend_deckorate_globals* deckorate_globals TSRMLS_DC)
{
    memset(&deckorate_globals->originals, 0, sizeof(HashTable));
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
DECKORATE_ARGINFO_STATIC
ZEND_BEGIN_ARG_INFO(php_deckorate_arginfo, 0)
    ZEND_ARG_INFO(0, old)
    ZEND_ARG_INFO(0, new)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ PHP_FUNCTION declarations */
PHP_FUNCTION(deckorate);
/* }}} */

/* {{{ deckorate_functions[]
 *
 * Every user visible function must have an entry in deckorate_functions[].
 */
zend_function_entry deckorate_functions[] = {
	PHP_FE(deckorate,                 php_deckorate_arginfo)
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
	PHP_RINIT(deckorate), 
	PHP_RSHUTDOWN(deckorate), 
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

/* {{{ PHP_RINIT_FUNCTION(deckorate) */
PHP_RINIT_FUNCTION(deckorate)
{
	zend_hash_init(&DECKORATE_G(originals), 32, NULL, NULL, 0);
	return SUCCESS;
}
/* }}} */


static int restore_function(char *fname, int fname_len TSRMLS_DC);

int reverse_decorators(zend_function *fe TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	restore_function(hash_key->arKey, hash_key->nKeyLength-1 TSRMLS_CC);
}

/* {{{ PHP_RSHUTDOWN_FUNCTION(deckorate) */
PHP_RSHUTDOWN_FUNCTION(deckorate)
{
	zend_hash_apply_with_arguments(&DECKORATE_G(originals) TSRMLS_CC, (apply_func_args_t)reverse_decorators, 0);
	zend_hash_clean(&DECKORATE_G(originals));
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


static int replace_function(char *fname, int fname_len, char *newname, int newname_len TSRMLS_DC) /* {{{ */
{
	zend_function fe, *pfe;
	if (zend_hash_find(EG(function_table), fname, fname_len + 1, (void**)&pfe) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s() not found", fname);
		return FAILURE;
	}

	fe = pfe[0];

	if (zend_hash_add(EG(function_table), newname, newname_len + 1, &fe, sizeof(zend_function), NULL) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s() could not be added", newname);
		return FAILURE;
	}

	if (zend_hash_del(EG(function_table), fname, fname_len + 1) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s() could not be replaced", fname);
		return FAILURE;
	}

	zend_hash_add(&DECKORATE_G(originals), newname, newname_len + 1, &fe, sizeof(zend_function), NULL);

	return SUCCESS;
} /* }}} */

static int restore_function(char *newname, int newname_len TSRMLS_DC) /* {{{ */
{
	zend_function *fe;
	char *fname;
	int fname_len;
	if (zend_hash_find(&DECKORATE_G(originals), newname, newname_len + 1, (void**)&fe) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s() could not be found", newname);
		return FAILURE;
	}

	fname = fe->common.function_name;
	fname_len = strlen(fname);

	if (zend_hash_del(EG(function_table), fname, fname_len + 1) == FAILURE) {
		/* go ahead anyway */
	}
	if (zend_hash_add(EG(function_table), fname, fname_len + 1, fe, sizeof(zend_function), NULL) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s() could not be added", fname);
		return FAILURE;
	}
	if (zend_hash_del(EG(function_table), newname, newname_len + 1) == FAILURE) {
		/* go ahead anyway */
	}
	return SUCCESS;
} /* }}} */

PHP_FUNCTION(deckorate)
{
	char *_old;
	int old_len;
	char *_new;
	int new_len;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &_old, &old_len, &_new, &new_len) == FAILURE)
	{
		return;
	}

	if(zend_hash_exists(&DECKORATE_G(originals), _old, old_len+1)) 
	{
		RETURN_FALSE;
	}

	if(replace_function(_old, old_len, _new, new_len TSRMLS_CC) == SUCCESS)
	{
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
