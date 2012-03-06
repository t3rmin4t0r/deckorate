#ifndef PHP_DECKORATE_H
#define PHP_DECKORATE_H

extern zend_module_entry deckorate_module_entry;
#define phpext_deckorate_ptr &deckorate_module_entry

#define PHP_DECKORATE_VERSION "0.0.1"

#ifdef PHP_WIN32
#define PHP_DECKORATE_API __declspec(dllexport)
#else
#define PHP_DECKORATE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* b/c for the new macros */
#ifndef Z_REFCOUNT_P
#define Z_REFCOUNT_P(pz)              (pz)->refcount
#define Z_REFCOUNT_PP(ppz)            Z_REFCOUNT_P(*(ppz))
#endif

#ifndef Z_SET_REFCOUNT_P
#define Z_SET_REFCOUNT_P(pz, rc)      (pz)->refcount = rc
#define Z_SET_REFCOUNT_PP(ppz, rc)    Z_SET_REFCOUNT_P(*(ppz), rc)
#endif

#ifndef Z_ADDREF_P
#define Z_ADDREF_P(pz)                (pz)->refcount++
#define Z_ADDREF_PP(ppz)              Z_ADDREF_P(*(ppz))
#endif

#ifndef Z_DELREF_P
#define Z_DELREF_P(pz)                (pz)->refcount--
#define Z_DELREF_PP(ppz)              Z_DELREF_P(*(ppz))
#endif

#ifndef Z_ISREF_P
#define Z_ISREF_P(pz)                 (pz)->is_ref
#define Z_ISREF_PP(ppz)               Z_ISREF_P(*(ppz))
#endif

#ifndef Z_SET_ISREF_P
#define Z_SET_ISREF_P(pz)             (pz)->is_ref = 1
#define Z_SET_ISREF_PP(ppz)           Z_SET_ISREF_P(*(ppz))
#endif

#ifndef Z_UNSET_ISREF_P
#define Z_UNSET_ISREF_P(pz)           (pz)->is_ref = 0
#define Z_UNSET_ISREF_PP(ppz)         Z_UNSET_ISREF_P(*(ppz))
#endif

#ifndef Z_SET_ISREF_TO_P
#define Z_SET_ISREF_TO_P(pz, isref)   (pz)->is_ref = isref
#define Z_SET_ISREF_TO_PP(ppz, isref) Z_SET_ISREF_TO_P(*(ppz), isref)
#endif

PHP_MINIT_FUNCTION(deckorate);
PHP_MSHUTDOWN_FUNCTION(deckorate);
PHP_RSHUTDOWN_FUNCTION(deckorate);
PHP_MINFO_FUNCTION(deckorate);


/* {{{ zend_deckorate_globals */
ZEND_BEGIN_MODULE_GLOBALS(deckorate)
ZEND_END_MODULE_GLOBALS(deckorate)
/* }}} */

/* {{{ extern deckorate_globals */
ZEND_EXTERN_MODULE_GLOBALS(deckorate)
/* }}} */

#ifdef ZTS
#define DECKORATE_G(v) TSRMG(deckorate_globals_id, zend_deckorate_globals *, v)
#else
#define DECKORATE_G(v) (deckorate_globals.v)
#endif

#endif	/* PHP_DECKORATE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim>600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
