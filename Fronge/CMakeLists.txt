add_library(Fronge SHARED
"Source/Application.cpp")

target_compile_definitions(Fronge PRIVATE
	${CONFIGURATION_MACRO}
	${PLATFORM_MACRO})
set_target_properties(Fronge PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/Fronge
	LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/Fronge
	RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/Fronge)

target_compile_definitions(Fronge PRIVATE Fronge_EXPORTS= FRO_BUILD_DLL)

target_include_directories(Fronge PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/Include)