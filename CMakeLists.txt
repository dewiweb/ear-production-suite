cmake_minimum_required(VERSION 3.10)
project(ear-production-combined VERSION 0.6.0.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

include(eps_set_install_paths)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    eps_set_install_paths()
endif()

include(CTest)
option(BUILD_TESTING "Build tests" ON)
set(Boost_USE_STATIC_LIBS ON)
include("enable_msvc_static_runtime")
find_package(nng REQUIRED)
add_subdirectory(submodules)
add_subdirectory(ear-production-suite)
add_subdirectory(reaper-adm-extension)
add_subdirectory(reaper_adm_stem_plugin)

if(EPS_BUILD_PACKAGE)
    add_subdirectory(packaging)
endif()

