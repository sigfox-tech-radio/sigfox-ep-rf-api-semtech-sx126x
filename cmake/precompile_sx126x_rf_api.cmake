find_program(UNIFDEF unifdef REQUIRED)
if(NOT UNIFDEF)
    message(FATAL_ERROR "unifdef not found!")
endif()

#List of precompileInc and precompileSrc files
foreach(X IN LISTS SX126X_RF_API_SOURCES)
	LIST(APPEND PRECOMPIL_SX126X_RF_API_SOURCES "${PRECOMPIL_DIR}/${X}")
endforeach()
foreach(X IN LISTS SX126X_RF_API_HEADERS)
	LIST(APPEND PRECOMPIL_SX126X_RF_API_HEADERS "${PRECOMPIL_DIR}/${X}")
endforeach()

#Custom command Loop for all RF api Sources and RF api Header
foreach(X IN LISTS SX126X_RF_API_SOURCES SX126X_RF_API_HEADERS)
add_custom_command(
	OUTPUT "${PRECOMPIL_DIR}/${X}"
	DEPENDS ${CMAKE_BINARY_DIR}/undefs_rf_api_file
	DEPENDS ${CMAKE_BINARY_DIR}/defs_rf_api_file
    DEPENDS ${X}
	COMMAND	${CMAKE_COMMAND} -E make_directory ${PRECOMPIL_DIR}/src/manuf ${PRECOMPIL_DIR}/inc/manuf ${PRECOMPIL_DIR}/src/board ${PRECOMPIL_DIR}/inc/board
    COMMAND unifdef -B -k -x 2 -f ${CMAKE_BINARY_DIR}/undefs_file -f ${CMAKE_BINARY_DIR}/undefs_rf_api_file -f ${CMAKE_BINARY_DIR}/defs_file -f ${CMAKE_BINARY_DIR}/defs_rf_api_file ${PROJECT_SOURCE_DIR}/${X} > "${PRECOMPIL_DIR}/${X}" 
	VERBATIM
)
endforeach()

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/defs_rf_api_file
	DEPENDS ${RF_API_HEADERS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND  cc -E -dMM ${DEF_FLAG_LIST} -I${CMAKE_CURRENT_SOURCE_DIR}/inc -I${CMAKE_SOURCE_DIR}/inc -I${SIGFOX_EP_LIB_DIR}/inc  ${SX126X_RF_API_HEADERS}  > "${CMAKE_BINARY_DIR}/defs_rf_api_file.tmp"
    COMMAND  grep -v __ "${CMAKE_BINARY_DIR}/defs_rf_api_file.tmp" | sort -u > "${CMAKE_BINARY_DIR}/defs_rf_api_file"
    COMMAND  rm "${CMAKE_BINARY_DIR}/defs_rf_api_file.tmp"
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/undefs_rf_api_file
	DEPENDS ${RF_API_HEADERS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/src/manuf/*c ${CMAKE_CURRENT_SOURCE_DIR}/inc/manuf/*.h | unifdef -s | sort -u | grep -v __ | sed "s/^/#undef /" >"${CMAKE_BINARY_DIR}/undefs_rf_api_file"
)

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

add_custom_target(precompil_${PROJECT_NAME}
	DEPENDS precompil
    DEPENDS ${PRECOMPIL_SX126X_RF_API_SOURCES}
    DEPENDS ${PRECOMPIL_SX126X_RF_API_HEADERS}
  	VERBATIM
)



