AUTOMAKE_OPTIONS=subdir-objects

# C compiler flags (compiling/linking)
AM_CFLAGS=

# C++ compiler flags (compiling/linking)
AM_CXXFLAGS=-Wall -ggdb

# C preprocessor flags
AM_CPPFLAGS=-D_GLIBCXX_USE_NANOSLEEP

# Installs pfdd into the bin directory
bin_PROGRAMS=salthttpd
salthttpd_SOURCES=main.cpp config/config_commandline.cpp config/config_default.cpp config/config_descriptor.cpp \
    config/config_file.cpp config/config_source.cpp config/configurator.cpp \
    concurrency/thread_pool.cpp
