cmake_minimum_required (VERSION 3.0)

add_executable(rhlogger rhlogger.c)
add_library(librhlogger STATIC
  librhlogger.c)
