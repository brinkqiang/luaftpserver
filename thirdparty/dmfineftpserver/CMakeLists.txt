cmake_minimum_required(VERSION 3.5.1)

PROJECT(dmfineftpserver)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

ModuleInclude("asio" "thirdparty/asio")
LibImport("libfineftp" "src/libfineftp")
ExeImport("example" "libfineftp")