PHP_ARG_WITH(tokyo-tyrant-dir, whether to enable tokyo tyrant support,
[  --with-tokyo-tyrant-dir[=DIR]       Enable tokyo tyrant support. DIR is the prefix to Tokyo Tyrant installation directory.], yes)

PHP_ARG_WITH(tokyo-cabinet-dir, directory of the Tokyo Cabinet installation,
[  --with-tokyo-cabinet-dir[=DIR]       DIR is the prefix to Tokyo Cabinet installation directory.], yes)

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test -z "$PKG_CONFIG"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall the pkg-config distribution])
  fi
  ORIG_PKG_CONFIG_PATH=$PKG_CONFIG_PATH
  
  if test "$PHP_TOKYO_TYRANT_DIR" = "yes"; then
    export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/local/lib/pkgconfig:/opt/lib/pkgconfig:/opt/local/lib/pkgconfig
  else
    export PKG_CONFIG_PATH=$PHP_TOKYO_TYRANT_DIR:$PHP_TOKYO_TYRANT_DIR/lib/pkgconfig
  fi

  AC_MSG_CHECKING([for Tokyo Tyrant])
  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists tokyotyrant; then
    PHP_TYRANT_INCS=`$PKG_CONFIG tokyotyrant --cflags`
    PHP_TYRANT_LIBS=`$PKG_CONFIG tokyotyrant --libs`
    PHP_TYRANT_VERSION_STRING=`$PKG_CONFIG tokyotyrant --modversion`

    PHP_EVAL_LIBLINE($PHP_TYRANT_LIBS, ICEJ_TT_SHARED_LIBADD)
    PHP_EVAL_INCLINE($PHP_TYRANT_INCS)
    AC_MSG_RESULT([yes, ${PHP_TYRANT_VERSION_STRING}])
  else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the Tokyo Tyrant distribution])
  fi

	AC_MSG_CHECKING([that Tokyo Tyrant is at least version 1.1.24])
  `$PKG_CONFIG --atleast-version=1.1.24 tokyotyrant`

  if test "$PHP_TOKYO_CABINET_DIR" = "yes"; then
    export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/local/lib/pkgconfig:/opt/lib/pkgconfig:/opt/local/lib/pkgconfig
  else
    export PKG_CONFIG_PATH=$PHP_TOKYO_CABINET_DIR:$PHP_TOKYO_CABINET_DIR/lib/pkgconfig
  fi
	AC_MSG_CHECKING([for Tokyo Cabinet])
  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists tokyocabinet; then
    PHP_CABINET_INCS=`$PKG_CONFIG tokyocabinet --cflags`
    PHP_CABINET_LIBS=`$PKG_CONFIG tokyocabinet --libs`
    PHP_CABINET_VERSION=`$PKG_CONFIG tokyocabinet --modversion`

    PHP_EVAL_LIBLINE($PHP_CABINET_LIBS, ICEJ_TT_SHARED_LIBADD)
    PHP_EVAL_INCLINE($PHP_CABINET_INCS)
    AC_MSG_RESULT([yes, ${PHP_CABINET_VERSION}])
  else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the Tokyo Cabinet distribution])
  fi

  PHP_NEW_EXTENSION(icej_tt, icej_tt.c, $ext_shared)
  PHP_SUBST(ICEJ_TT_SHARED_LIBADD)
  export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"

dnl $Id$
dnl config.m4 for extension icej_tt

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(icej_tt, for icej_tt support,
dnl Make sure that the comment is aligned:
dnl [  --with-icej_tt             Include icej_tt support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(icej_tt, whether to enable icej_tt support,
dnl Make sure that the comment is aligned:
dnl [  --enable-icej_tt           Enable icej_tt support])

dnl if test "$PHP_ICEJ_TT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-icej_tt -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/icej_tt.h"  # you most likely want to change this
  dnl if test -r $PHP_ICEJ_TT/$SEARCH_FOR; then # path given as parameter
  dnl   ICEJ_TT_DIR=$PHP_ICEJ_TT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for icej_tt files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ICEJ_TT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ICEJ_TT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the icej_tt distribution])
  dnl fi

  dnl # --with-icej_tt -> add include path
  dnl PHP_ADD_INCLUDE($ICEJ_TT_DIR/include)

  dnl # --with-icej_tt -> check for lib and symbol presence
  dnl LIBNAME=icej_tt # you may want to change this
  dnl LIBSYMBOL=icej_tt # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ICEJ_TT_DIR/lib, ICEJ_TT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ICEJ_TTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong icej_tt lib version or lib not found])
  dnl ],[
  dnl   -L$ICEJ_TT_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(ICEJ_TT_SHARED_LIBADD)

dnl fi
