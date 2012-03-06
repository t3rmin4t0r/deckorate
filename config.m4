dnl $Id: $
dnl config.m4 for extension deckorate

PHP_ARG_ENABLE(deckorate, whether to enable deckorate support,
[  --enable-deckorate           Enable deckorate support])

if test "$PHP_DECKORATE" != "no"; then

  deckorate_sources="php_deckorate.c"

  PHP_NEW_EXTENSION(deckorate, $deckorate_sources, $ext_shared)
fi
