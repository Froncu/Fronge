add_library(Fronge SHARED
"Source/Fronge/Application.cpp")

target_compile_definitions(Fronge PRIVATE
	${CONFIGURATION_MACRO}
	${CONFIGURATION_MACRO})
set_target_properties(Fronge PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/Fronge
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/Fronge
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/Fronge)

target_compile_definitions(Fronge PRIVATE Fronge_EXPORTS= FRO_BUILD_DLL)