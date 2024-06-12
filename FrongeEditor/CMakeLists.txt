add_executable(FrongeEditor
"Code/FrongeEditor.cpp")

target_compile_definitions(FrongeEditor PRIVATE
	${CONFIGURATION_MACRO}
	${PLATFORM_MACRO})
set_target_properties(FrongeEditor PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/FrongeEditor
	LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/FrongeEditor
	RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_DIRECTORY}/FrongeEditor)

target_link_libraries(FrongeEditor PRIVATE Fronge)
add_custom_command(TARGET FrongeEditor
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
$<TARGET_FILE:Fronge>
$<TARGET_FILE_DIR:FrongeEditor>)