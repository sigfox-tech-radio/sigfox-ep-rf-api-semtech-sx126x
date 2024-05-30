cmake_minimum_required(VERSION 3.15)
project(sx126x_rf_api)
include(ExternalProject)

#Check if Project configure by himself or by a parent project (like sigfox-ep-lib)
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
    if (NOT DEFINED  SIGFOX_EP_LIB_DIR)
        message(FATAL_ERROR "${PROJECT_NAME} cannot be configured without -DSIGFOX_EP_LIB_DIR=<sigfox_ep_lib directory>")
    endif()
    add_subdirectory(${SIGFOX_EP_LIB_DIR} "${CMAKE_CURRENT_BINARY_DIR}/${SIGFOX_EP_LIB_DIR}")
 else()
    if (NOT DEFINED SIGFOX_EP_LIB_DIR)
        set(SIGFOX_EP_LIB_DIR ${CMAKE_SOURCE_DIR})
        message("${PROJECT_NAME} is Fetched by a parent project")
    endif()
endif()

set (SX126X_RF_API_SOURCES
    src/manuf/sx126x_rf_api.c
    src/manuf/smtc_dbpsk.c
)

set (SX126X_RF_API_HEADERS
    inc/manuf/sx126x_rf_api.h
    inc/manuf/smtc_dbpsk.h
    inc/board/sx126x_hw_api.h
)

set( CHIP_RF_HEADERS_LIST "")

if (NOT TARGET sx126x_driver)
    list(APPEND CHIP_RF_HEADERS_LIST  ${CMAKE_CURRENT_BINARY_DIR}/drivers-src/sx126x/src/)
    ExternalProject_Add(sx126x
        PREFIX drivers-build
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        UPDATE_COMMAND 
            COMMAND bash -c "git -C ${CMAKE_CURRENT_BINARY_DIR}/drivers-src/sx126x/ reset --hard"  
        SOURCE_DIR drivers-src/sx126x
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
        GIT_REPOSITORY    https://github.com/Lora-net/sx126x_driver.git
        GIT_TAG           v2.3.2
    )
else ()
    list(APPEND CHIP_RF_HEADERS_LIST "$<BUILD_INTERFACE:$<TARGET_PROPERTY:sx126x_driver,INTERFACE_INCLUDE_DIRECTORIES>>")
endif()

#Add Cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#Precompile module
include(precompile_sx126x_rf_api)

add_library(${PROJECT_NAME}_obj OBJECT EXCLUDE_FROM_ALL src/manuf/sx126x_rf_api.c  src/manuf/smtc_dbpsk.c)
if (NOT TARGET sx126x_driver)
add_dependencies(${PROJECT_NAME}_obj sx126x)
endif()
target_include_directories( ${PROJECT_NAME}_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${SIGFOX_EP_LIB_DIR}/inc
    ${CHIP_RF_HEADERS_LIST}
)
target_compile_definitions(${PROJECT_NAME}_obj PUBLIC ${DEF_FLAG_LIST})

add_library(${PROJECT_NAME} STATIC ${SX126X_RF_API_SOURCES})
add_dependencies(${PROJECT_NAME}  precompil_${PROJECT_NAME})
if (NOT TARGET sx126x_driver)
    add_dependencies(${PROJECT_NAME} sx126x)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${PRECOMPIL_DIR}/inc  ${CHIP_RF_HEADERS_LIST})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${DEF_FLAG_LIST})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PRECOMPIL_ADDON_RFP_PUBLIC_HEADERS}")
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_LOCATION}
    LIBRARY_OUTPUT_DIRECTORY ${LIB_LOCATION}
)
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${LIB_LOCATION}
    PUBLIC_HEADER DESTINATION ${API_LOCATION}  
)
