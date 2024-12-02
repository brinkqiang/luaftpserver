cmake_minimum_required(VERSION 3.8)

PROJECT(luaftpserver)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/LuaModule.cmake)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(LUA_MODULE lua54)
SET(LUAFTPSERVER_VERSION_MAJOR "1")
SET(LUAFTPSERVER_VERSION_MINOR "0")
SET(LUAFTPSERVER_VERSION_PATCH "1")
SET(LUAFTPSERVER_VERSION "${LUAFTPSERVER_VERSION_MAJOR}.${LUAFTPSERVER_VERSION_MINOR}.${LUAFTPSERVER_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${LUAFTPSERVER_VERSION}")

ADD_DEFINITIONS(-DUSE_LUAFTPSERVER)
ADD_DEFINITIONS(-DLUA_COMPAT_MODULE -DLUA_COMPAT_APIINTCASTS)

ModuleImport("dmfineftpserver" "thirdparty/dmfineftpserver")

LuaModuleImport("${LUA_MODULE}" "luaftpserver" "src/luaftpserver" "libfineftp")

IF (WIN32)
ADD_CUSTOM_COMMAND(
    TARGET luaftpserver
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/config/lua_dkm_debug.json
    $<TARGET_FILE_DIR:luaftpserver>
)
ENDIF(WIN32)


IF(WIN32)
ADD_CUSTOM_COMMAND(
TARGET luaftpserver
PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/setup/nssm.exe $<TARGET_FILE_DIR:luaftpserver>
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/setup/install.bat $<TARGET_FILE_DIR:luaftpserver>
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/setup/uninstall.bat $<TARGET_FILE_DIR:luaftpserver>
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE $<TARGET_FILE_DIR:luaftpserver>
)
ENDIF(WIN32)

install(DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/
        DESTINATION bin
        FILES_MATCHING PATTERN "*"
        PATTERN "debug" EXCLUDE
        PATTERN "*.pdb" EXCLUDE
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.zip" EXCLUDE
        PATTERN "*.json" EXCLUDE
        PATTERN "*fineftp*" EXCLUDE
        PATTERN "*.exp" EXCLUDE
        PATTERN "*test*" EXCLUDE
        )


# 配置 CPack 打包选项
set(CPACK_GENERATOR "ZIP;NSIS64") # 选择打包格式，这里以 ZIP 为例
set(CPACK_PACKAGE_NAME "luaftpserver") # 设置包的名字
set(CPACK_PACKAGE_VENDOR "dmsoft") # 设置包的作者
set(CPACK_PACKAGE_DESCRIPTION "dmsoft luaftpserver") # 设置包的描述
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "dmsoft luaftpserver summary")

set(CPACK_PACKAGE_CONTACT "https://github.com/brinkqiang/luaftpserver")
set(CPACK_PACKAGE_VERSION "1.0.1")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

IF(WIN32)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    ExecWait '\\\"$INSTDIR\\\\bin\\\\RelWithDebInfo\\\\install.bat\\\"'
    ")

    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    ExecWait '\\\"$INSTDIR\\\\bin\\\\RelWithDebInfo\\\\uninstall.bat\\\"'
    ")
ENDIF(WIN32)

set(CPACK_CMD "cpack -V -G NSIS64")

include(CPack)