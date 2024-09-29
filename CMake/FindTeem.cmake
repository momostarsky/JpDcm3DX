# FindTeem.cmake
# Try to find Teem
# Once done, this will define:
#
#   Teem_FOUND        - system has Teem
#   Teem_INCLUDE_DIRS - the Teem include directories
#   Teem_LIBRARIES    - Link these to use Teem

# 查找 Teem 的头文件目录
set(Teem_INCLUDE_DIRS /opt/teem/include)

# 查找 Teem 的库文件
find_library(Teem_LIBRARIES
        NAMES teem
        PATHS /opt/teem/lib
        NO_DEFAULT_PATH
)

# 使用 GLOB 获取所有 .h 文件
execute_process(
        COMMAND find /opt/teem/include -name "*.h"
        OUTPUT_VARIABLE Teem_USE_FILE
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
#string(REPLACE ";" "\n" Teem_HEADERS "${Teem_HEADERS}")
#string(REPLACE "\r\n" ";" Teem_HEADERS "${Teem_HEADERS}")
#string(REPLACE "\n" ";" Teem_HEADERS "${Teem_HEADERS}")
# 查找 Teem 的头文件路径
#find_file(Teem_USE_FILE
#        NAMES teem/nrrd.h air.h
#        PATHS /opt/teem/include
#)

# 检查是否找到了所有必需的头文件和库文件
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Teem
        REQUIRED_VARS Teem_USE_FILE  Teem_LIBRARIES  Teem_INCLUDE_DIRS
)

# 输出找到的 Teem 信息
#message(STATUS "Teem_LIBRARIES: ${Teem_LIBRARIES}")
#message(STATUS "Teem_USE_FILE: ${Teem_USE_FILE}")
#message(STATUS "Teem_INCLUDE_DIRS: ${Teem_INCLUDE_DIRS}")

mark_as_advanced(Teem_USE_FILE Teem_LIBRARIES  Teem_INCLUDE_DIRS)