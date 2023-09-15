dnl config.m4 for extension observe_zendmm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([observe_zendmm],
dnl   [for observe_zendmm support],
dnl   [AS_HELP_STRING([--with-observe_zendmm],
dnl     [Include observe_zendmm support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([observe_zendmm],
  [whether to enable observe_zendmm support],
  [AS_HELP_STRING([--enable-observe_zendmm],
    [Enable observe_zendmm support])],
  [no])

if test "$PHP_OBSERVE_ZENDMM" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, OBSERVE_ZENDMM_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-observe_zendmm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/observe_zendmm.h"  # you most likely want to change this
  dnl if test -r $PHP_OBSERVE_ZENDMM/$SEARCH_FOR; then # path given as parameter
  dnl   OBSERVE_ZENDMM_DIR=$PHP_OBSERVE_ZENDMM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for observe_zendmm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OBSERVE_ZENDMM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OBSERVE_ZENDMM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the observe_zendmm distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-observe_zendmm -> add include path
  dnl PHP_ADD_INCLUDE($OBSERVE_ZENDMM_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-observe_zendmm -> check for lib and symbol presence
  dnl LIBNAME=OBSERVE_ZENDMM # you may want to change this
  dnl LIBSYMBOL=OBSERVE_ZENDMM # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_OBSERVE_ZENDMM_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your observe_zendmm library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OBSERVE_ZENDMM_DIR/$PHP_LIBDIR, OBSERVE_ZENDMM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OBSERVE_ZENDMM_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your observe_zendmm library.])
  dnl ],[
  dnl   -L$OBSERVE_ZENDMM_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OBSERVE_ZENDMM_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_OBSERVE_ZENDMM, 1, [ Have observe_zendmm support ])

  PHP_NEW_EXTENSION(observe_zendmm, observe_zendmm.c, $ext_shared)
fi
