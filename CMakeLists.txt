cmake_minimum_required (VERSION 2.8.12.1)
cmake_policy(SET CMP0015 NEW)
project (loot_api_c)
include(ExternalProject)

option(BUILD_SHARED_LIBS "Build a shared library" ON)
option(MSVC_STATIC_RUNTIME "Build with static runtime libs (/MT)" ON)

#######################################
# External Projects
#######################################

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)

IF (MSVC)
    set (Boost_USE_STATIC_RUNTIME ${MSVC_STATIC_RUNTIME})
ELSE()
    set (Boost_USE_STATIC_RUNTIME OFF)
ENDIF()

IF (NOT Boost_USE_STATIC_LIBS)
    add_definitions(-DBOOST_LOG_DYN_LINK)
ENDIF ()

find_package(Boost REQUIRED COMPONENTS atomic log log_setup regex locale thread date_time chrono filesystem system iostreams)

ExternalProject_Add(GTest
                    PREFIX "external"
                    URL "https://github.com/google/googletest/archive/release-1.7.0.tar.gz"
                    CMAKE_ARGS -Dgtest_force_shared_crt=${MSVC_SHARED_RUNTIME}
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(GTest SOURCE_DIR BINARY_DIR)
set (GTEST_INCLUDE_DIRS "${SOURCE_DIR}/include")
set (GTEST_LIBRARIES "${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Add(testing-metadata
                    PREFIX "external"
                    URL "https://github.com/loot/testing-metadata/archive/2.0.0.tar.gz"
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")

ExternalProject_Add(testing-plugins
                    PREFIX "external"
                    URL "https://github.com/WrinklyNinja/testing-plugins/archive/1.0.0.tar.gz"
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")

ExternalProject_Add(loot_api
                    PREFIX "external"
                    URL "https://github.com/loot/loot/archive/dc3a93ecf331465b17a28ac39e6c17370438331b.zip"
                    CMAKE_ARGS -DBOOST_INCLUDEDIR=${Boost_INCLUDE_DIR} -DMSVC_STATIC_RUNTIME=${MSVC_STATIC_RUNTIME} -DBUILD_SHARED_LIBS=OFF
                    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target loot_api --config $(CONFIGURATION)
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(loot_api SOURCE_DIR BINARY_DIR)
set(LOOT_API_INCLUDE_DIRS "${SOURCE_DIR}/include")
set(LOOT_API_LIBRARIES
    "${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}loot_api${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}loot_common${CMAKE_STATIC_LIBRARY_SUFFIX}"
    ${Boost_LIBRARIES}
    "${BINARY_DIR}/external/src/libgit2-build/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}git2${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BINARY_DIR}/external/src/libloadorder-build/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}loadorder${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${BINARY_DIR}/external/src/yaml-cpp-build/${CMAKE_CFG_INTDIR}/libyaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}")

##############################
# General Settings
##############################

set(LOOT_C_API_SRC "${CMAKE_SOURCE_DIR}/src/api/c_api.cpp"
                   "${CMAKE_SOURCE_DIR}/src/api/loot_db.cpp")

set(LOOT_C_API_HEADERS "${CMAKE_SOURCE_DIR}/include/loot/c_api.h"
                       "${CMAKE_SOURCE_DIR}/src/api/loot_db.h")

set(LOOT_C_API_TESTS_SRC "${CMAKE_SOURCE_DIR}/src/test/main.cpp")

set(LOOT_C_API_TESTS_HEADERS  "${CMAKE_SOURCE_DIR}/src/test/api_game_operations_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_apply_load_order_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_create_db_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_eval_lists_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_get_dirty_info_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_get_masterlist_revision_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_get_plugin_messages_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_get_plugin_tags_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_get_tag_map_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_load_lists_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_sort_plugins_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_update_masterlist_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/loot_write_minimal_list_test.h"
                              "${CMAKE_SOURCE_DIR}/src/test/test_api.h"
                              "${CMAKE_SOURCE_DIR}/src/test/common_game_test_fixture.h")

set(LOOT_C_API_TESTS_SRC "${CMAKE_SOURCE_DIR}/src/test/main.cpp")

set(LOOT_C_API_TESTS_HEADERS  "${CMAKE_SOURCE_DIR}/src/test/api_game_operations_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_create_db_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_eval_lists_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_get_dirty_info_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_get_masterlist_revision_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_get_plugin_messages_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_get_plugin_tags_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_load_lists_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_sort_plugins_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_update_masterlist_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/loot_write_minimal_list_test.h"
                            "${CMAKE_SOURCE_DIR}/src/test/test_api.h"
                            "${CMAKE_SOURCE_DIR}/src/test/common_game_test_fixture.h")

source_group("Header Files\\api" FILES ${LOOT_C_API_HEADERS})
source_group("Header Files\\tests" FILES ${LOOT_C_API_TESTS_HEADERS})

source_group("Source Files\\api" FILES ${LOOT_C_API_SRC})
source_group("Source Files\\tests" FILES ${LOOT_C_API_TESTS_SRC})

# Include source and library directories.
include_directories("${CMAKE_SOURCE_DIR}/src"
                    "${CMAKE_SOURCE_DIR}/include"
                    ${LOOT_API_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${GTEST_INCLUDE_DIRS})

##############################
# System-Specific Settings
##############################

# Settings when compiling for Windows.
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions(-DUNICODE -D_UNICODE -DLIBLO_STATIC -DLOOT_STATIC)
    set(LOOT_C_API_SRC ${LOOT_C_API_SRC} "${CMAKE_SOURCE_DIR}/src/api/resource.rc")
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};.")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14")
endif ()

if (MSVC)
    # Force static C++ runtime linkage.
    if (MSVC_STATIC_RUNTIME)
        foreach(flag
            CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
            CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
            string(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
            set("${flag}" "${${flag}} /EHsc")
        endforeach()
    endif ()

    # Set /bigobj to allow building Debug tests
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")


    set (LOOT_API_LIBRARIES ${LOOT_API_LIBRARIES}
                            version
                            shlwapi
                            winhttp
                            crypt32
                            Rpcrt4)
endif ()

##############################
# Define Targets
##############################

# Build API.
add_library          (loot_c_api ${LOOT_C_API_SRC} ${LOOT_C_API_HEADERS})
add_dependencies     (loot_c_api loot_api)
target_link_libraries(loot_c_api ${LOOT_API_LIBRARIES})

# Build API tests.
add_executable       (c_api_tests ${LOOT_C_API_TESTS_SRC} ${LOOT_C_API_TESTS_HEADERS})
add_dependencies     (c_api_tests loot_c_api GTest testing-metadata testing-plugins)
target_link_libraries(c_api_tests loot_c_api ${GTEST_LIBRARIES})

##############################
# Set Target-Specific Flags
##############################

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    if (BUILD_SHARED_LIBS)
        set_target_properties(loot_c_api PROPERTIES COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS} LOOT_C_API_EXPORT")
    else ()
        set_target_properties(loot_c_api PROPERTIES COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS} LOOT_C_API_STATIC")
    endif ()
endif ()

##############################
# Post-Build Steps
##############################

# Copy testing metadata
ExternalProject_Get_Property(testing-metadata SOURCE_DIR)
add_custom_command(TARGET loot_c_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}
        "$<TARGET_FILE_DIR:loot_c_api>/testing-metadata")

# Copy testing plugins
ExternalProject_Get_Property(testing-plugins SOURCE_DIR)
add_custom_command(TARGET loot_c_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}
        $<TARGET_FILE_DIR:loot_c_api>)
