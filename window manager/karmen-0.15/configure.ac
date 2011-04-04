AC_INIT(karmen, 0.15, [veenhuizen@users.sourceforge.net])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

AC_PATH_XTRA

AC_ARG_ENABLE(ewmh,
[  --disable-ewmh          disable support for Extended Window Manager Hints],
[case "${enableval}" in
    yes) ewmh=yes ;;
    no) ewmh=no ;;
    *) AC_MSG_ERROR(bad value ${enableval} for --enable-ewmh) ;;
esac],[ewmh=yes])
AM_CONDITIONAL(CONFIG_EWMH, test x$ewmh = xyes)

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
