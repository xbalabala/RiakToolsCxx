cmake_minimum_required(VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
enable_language(CXX)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
endif()

include(CheckCXXCompilerFlag)
include(ExternalProject)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        error(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

project(RiakToolsCxx)

ExternalProject_Add(leveldb-basho
	PREFIX external
        GIT_REPOSITORY https://github.com/hw-dwalter/leveldb.git
	GIT_TAG develop
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make 
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ""
        INSTALL_COMMAND ""
)

set(LEVELDBBASHO_SRC "${CMAKE_CURRENT_BINARY_DIR}/external/src/leveldb-basho")
add_library(libleveldb STATIC IMPORTED)
set_target_properties(libleveldb PROPERTIES IMPORTED_LOCATION ${LEVELDBBASHO_SRC}/libleveldb.a)
add_dependencies(libleveldb leveldb-basho)

add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_LOG_USE_NATIVE_SYSLOG )

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS
  filesystem
)

include_directories(
  "${LEVELDBBASHO_SRC}/include"
  "${LEVELDBBASHO_SRC}"
)

add_subdirectory(src)
