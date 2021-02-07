cmake_minimum_required (VERSION 3.0)

add_executable(logging-daemon rhlogger.c rhlogger_loop.c)
#add_library(librhlogger STATIC
#  librhlogger.c)
