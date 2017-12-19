# CMake (meta-)build system for l3afpad
#
# Commonly used like this:
#
#	mkdir build
#	cd build
#	cmake ..
#	make
#	make install
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME "L3afpad")
SET(PACKAGE "l3afpad")
SET(VERSION "v0.8.18.1.11+")

PROJECT(${NAME} LANGUAGES C)


FIND_PACKAGE(PkgConfig)

PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
ADD_DEFINITIONS(${GTK3_CFLAGS} ${GTK3_CFLAGS_OTHER})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})



ADD_DEFINITIONS(-DPACKAGE="${PACKAGE}")
ADD_DEFINITIONS(-DPACKAGE_BUGREPORT="stevenhoneyman@gmail.com")
ADD_DEFINITIONS(-DPACKAGE_NAME="${NAME}")
ADD_DEFINITIONS(-DPACKAGE_STRING="${NAME} ${VERSION}")
ADD_DEFINITIONS(-DPACKAGE_TARNAME="${PACKAGE}")
ADD_DEFINITIONS(-DPACKAGE_URL="")
ADD_DEFINITIONS(-DPACKAGE_VERSION="${VERSION}")
ADD_DEFINITIONS(-DLOCALEDIR="/usr/local/share/locale")
ADD_DEFINITIONS(-DICONDIR="/usr/local/share/pixmaps")

# Define to 1 if you enable Emacs key theme.
#ADD_DEFINITIONS(-DENABLE_EMACS=1)

# always defined to indicate that i18n is enabled
ADD_DEFINITIONS(-DENABLE_NLS=1)

# Define to 0 if you disable print.
#ADD_DEFINITIONS(-DENABLE_PRINT=0)

# Define to 0 if you disable statistics.
#ADD_DEFINITIONS(-DENABLE_STATISTICS=0)

# Define to 0 if you disable Xinput2.
#ADD_DEFINITIONS(-DENABLE_XINPUT2=0)

# Define to 1 if you have the `bind_textdomain_codeset' function.
#ADD_DEFINITIONS(-DHAVE_BIND_TEXTDOMAIN_CODESET=1)

# Define to 1 if you have the `dcgettext' function.
#ADD_DEFINITIONS(-DHAVE_DCGETTEXT=1)

# Define if the GNU gettext() function is already present or preinstalled.
#ADD_DEFINITIONS(-DHAVE_GETTEXT=1)

# Define to 1 if you have the <inttypes.h> header file.
#ADD_DEFINITIONS(-DHAVE_INTTYPES_H=1)

# Define if your <locale.h> file defines LC_MESSAGES.
#ADD_DEFINITIONS(-DHAVE_LC_MESSAGES=1)

# Define to 1 if you have the <locale.h> header file.
#ADD_DEFINITIONS(-DHAVE_LOCALE_H=1)

# Define to 1 if you have the <memory.h> header file.
#ADD_DEFINITIONS(-DHAVE_MEMORY_H=1)

# Define to 1 if you have the <stdint.h> header file.
#ADD_DEFINITIONS(-DHAVE_STDINT_H=1)

# Define to 1 if you have the <stdlib.h> header file.
#ADD_DEFINITIONS(-DHAVE_STDLIB_H=1)

# Define to 1 if you have the <strings.h> header file.
#ADD_DEFINITIONS(-DHAVE_STRINGS_H=1)

# Define to 1 if you have the <string.h> header file.
#ADD_DEFINITIONS(-DHAVE_STRING_H=1)

# Define to 1 if you have the <sys/stat.h> header file.
#ADD_DEFINITIONS(-DHAVE_SYS_STAT_H=1)

# Define to 1 if you have the <sys/types.h> header file.
#ADD_DEFINITIONS(-DHAVE_SYS_TYPES_H=1)

# Define to 1 if you have the <unistd.h> header file.
#ADD_DEFINITIONS(-DHAVE_UNISTD_H=1)

# Define to 1 if you enable search history.
#ADD_DEFINITIONS(-DSEARCH_HISTORY=1)

# Define to 1 if you have the ANSI C header files.
#ADD_DEFINITIONS(-DSTDC_HEADERS=1)

# Enable large inode numbers on Mac OS X 10.5.
#ifndef _DARWIN_USE_64_BIT_INODE
#ADD_DEFINITIONS(-D_DARWIN_USE_64_BIT_INODE)
#endif

# Number of bits in a file offset, on hosts where this is settable.
#ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=???)

# Define for large files, on AIX-style hosts.
#ADD_DEFINITIONS(-D_LARGE_FILES)



INCLUDE_DIRECTORIES(src)

SET(SOURCES
	src/callback.c
	src/dialog.c
	src/dnd.c
	src/emacs.c
	src/encoding.c
	src/file.c
	src/font.c
	src/gtkprint.c
	src/gtksourceiter.c
	src/hlight.c
	src/indent.c
	src/linenum.c
	src/main.c
	src/menu.c
	src/search.c
	src/selector.c
	src/undo.c
	src/utils.c
	src/view.c
	src/window.c
	)

ADD_EXECUTABLE(${PACKAGE} ${SOURCES})
TARGET_LINK_LIBRARIES(${PACKAGE} ${GTK3_LIBRARIES})
INSTALL(TARGETS ${PACKAGE} DESTINATION bin)

# Setting up Intl
FIND_PACKAGE(Intl REQUIRED)
FIND_PACKAGE(Gettext REQUIRED)
INCLUDE_DIRECTORIES(${INTL_INCLUDE_DIRS})
LINK_DIRECTORIES(${INTL_LIBRARY_DIRS})

FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

IF(NOT GETTEXT_MSGFMT_EXECUTABLE)
    MESSAGE("------\nNOTE: msgfmt not found. Translations will *not* be installed\n------")
ELSE(NOT GETTEXT_MSGFMT_EXECUTABLE)

  SET(catalogname rkward)

  FILE(GLOB PO_FILES po/*.po)
  SET(GMO_FILES)

  FOREACH(_poFile ${PO_FILES})
    GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
    STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_poFileName} )
    STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )

    IF( _langCode )
      GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
      SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

      ADD_CUSTOM_COMMAND(OUTPUT ${_gmoFile}
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_poFile}
        DEPENDS ${_poFile})
      INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
      LIST(APPEND GMO_FILES ${_gmoFile})
    ENDIF( _langCode )

  ENDFOREACH(_poFile ${PO_FILES})

  ADD_CUSTOM_TARGET(translations ALL DEPENDS ${GMO_FILES})

ENDIF(NOT GETTEXT_MSGFMT_EXECUTABLE)

