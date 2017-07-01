AC_PREREQ([2.68])
AC_INIT([prayer-clock],[4.0.0])
AM_INIT_AUTOMAKE([1.11 no-define foreign])

#AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
CXXFLAGS+=" -std=c++11"

#This is for mingw-cross-env
AM_CONDITIONAL([MINGW_CROSS],[test "x$host" = xi686-pc-mingw32])
AM_COND_IF([MINGW_CROSS],
	[PKG_CONFIG_PATH=$PKG_CONFIG_PATH_i686_pc_mingw32])

#This is for Windows MinGW32
#AM_CONDITIONAL([MINGW32],[test "x$target" = xmingw32])

AM_CONDITIONAL([MINGW],[test "x$target" = xming32 || test "x$host" = xi686-pc-mingw32 ])

PKG_CHECK_MODULES([GTK3],[gtk+-3.0 >= 3])
PKG_CHECK_MODULES([GTKMM3],[gtkmm-3.0])

AC_ARG_ENABLE([minimize-to-tray],
	AS_HELP_STRING([--disable-minimize-to-tray],[Disable minimize to tray]),
	[minimize_to_tray=false],
	[minimize_to_tray=true])

AM_CONDITIONAL([NO_TO_TRAY],[test "x$minimize_to_tray" = xfalse])


AC_CONFIG_FILES([Makefile
	src/Makefile
	res/Makefile])
AC_OUTPUT
