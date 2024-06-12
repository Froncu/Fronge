add_executable(FrongeEditor
"Source/FrongeEditor.cpp")

target_compile_definitions(FrongeEditor PRIVATE
	${CONFIGURATION_MACRO}
	${CONFIGURATION_MACRO})
set_target_properties(FrongeEditor PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/FrongeEditor
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/FrongeEditor
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME}/FrongeEditor)

target_include_directories(FrongeEditor PRIVATE ${CMAKE_SOURCE_DIR}/Fronge/Source)
target_link_libraries(FrongeEditor Fronge)