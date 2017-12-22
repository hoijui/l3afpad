# CMake script to add our executable to the list
# of choosable default GUI text editors in gnome.

#SET(INSTALLED_TARGET_BIN $<TARGET_FILE:l3afpad>)
SET(INSTALLED_TARGET_BIN "${CMAKE_INSTALL_PREFIX}/bin/l3afpad")

MESSAGE(STATUS "Adding \"${INSTALLED_TARGET_BIN}\" as Gnome default GUI text-editor alternative")

EXECUTE_PROCESS(
	COMMAND updateAlternatives "${INSTALLED_TARGET_BIN}"
	WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}
	)

