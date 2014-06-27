AC_PREREQ([2.60])
AC_INIT([SALT HTTP Server], [0.1], [david.steinsland@saltship.com])

config_flags="$*"
AC_DEFINE_UNQUOTED([CONFIG_FLAGS],["$config_flags"],[Flags passed to configure])

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_include([m4/ax_cxx_check_lib.m4])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext])

m4_pattern_allow([AC_MSG_ERROR])

# Checks for required libraries.
AC_CHECK_LIB([event_pthreads], [evthread_use_pthreads], [], [
  AC_MSG_ERROR("libevent_pthreads not found")
])

AC_CHECK_LIB(event, [event_get_version], [], [
  AC_MSG_ERROR("libevent not found. Use configure --help to see how to specify the search path)
])

AX_CXX_CHECK_LIB(config++, [libconfig::Config], [], [
    AC_MSG_ERROR("libconfig not found. Use configure --help to see how to specify the search path)
])
AC_CHECK_HEADERS(libconfig.h++, [], [
  AC_MSG_ERROR("Unable to locate libconfig.h++. Use configure --help to see how to specify the search path")
])

AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h netinet/in.h stddef.h string.h sys/socket.h syslog.h unistd.h])

AC_CHECK_HEADERS(event2/event.h event2/buffer.h event2/bufferevent.h, [], [
  AC_MSG_ERROR("Unable to locate event2/event.h. Use configure --help to see how to specify the search path")
])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memset socket strerror])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
