# mbrtowc.m4 serial 4 (fileutils-4.1.3)
dnl Copyright (C) 2001-2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

dnl From Paul Eggert

AC_DEFUN([jm_FUNC_MBRTOWC],
[
  AC_CACHE_CHECK([whether mbrtowc and mbstate_t are properly declared],
    jm_cv_func_mbrtowc,
    [AC_TRY_LINK(
       [#include <wchar.h>],
       [mbstate_t state; return ! (sizeof state && mbrtowc);],
       jm_cv_func_mbrtowc=yes,
       jm_cv_func_mbrtowc=no)])
  if test $jm_cv_func_mbrtowc = yes; then
    AC_DEFINE(HAVE_MBRTOWC, 1,
      [Define to 1 if mbrtowc and mbstate_t are properly declared.])
  fi
])
