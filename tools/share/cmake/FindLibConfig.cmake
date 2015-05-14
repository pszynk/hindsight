
# Find the CUnit includes and library
#
# This module defines
# LIBCONFIGPP_INCLUDE_DIR, where to find libconfig++ include files, etc.
# LIBCONFIGPP_LIBRARIES, the libraries to link against to use LibConfig++.
# LIBCONFIGPP_STATIC_LIBRARIY_PATH
# LIBCONFIGPP_FOUND, If false, do not try to use LibConfig++.
# also defined, but not for general use are
# LIBCONFIGPP_LIBRARY, where to find the LibConfig++ library.


MESSAGE("Searching for libconfig library")


FIND_PATH(LIBCONFIGPP_INCLUDE_DIR libconfig.h++
/usr/local/include
/usr/include
/opt/local/include
)


FIND_LIBRARY(LIBCONFIGPP_LIBRARY config++
/usr/local/lib
/usr/lib
/opt/local/lib
)


FIND_LIBRARY(LIBCONFIGPP_STATIC_LIBRARY "libconfig++${CMAKE_STATIC_LIBRARY_SUFFIX}"
/usr/local/lib
/usr/lib
/opt/local/lib
)


IF(LIBCONFIGPP_INCLUDE_DIR)
    IF(LIBCONFIGPP_LIBRARY)
        SET(LIBCONFIGPP_FOUND TRUE)
        SET(LIBCONFIGPP_LIBRARIES ${LIBCONFIGPP_LIBRARY})
        SET(LIBCONFIGPP_STATIC_LIBRARY_PATH ${LIBCONFIGPP_STATIC_LIBRARY})
    ENDIF(LIBCONFIGPP_LIBRARY)
ENDIF(LIBCONFIGPP_INCLUDE_DIR)


IF (LIBCONFIGPP_FOUND)
    IF (NOT LibConfig_FIND_QUIETLY)
        MESSAGE(STATUS "Found LibConfig++: ${LIBCONFIGPP_LIBRARIES}" )
        MESSAGE(STATUS "static LibConfig++ path: ${LIBCONFIGPP_STATIC_LIBRARY_PATH}")
    ENDIF (NOT LibConfig_FIND_QUIETLY)
ELSE (LIBCONFIGPP_FOUND)
    IF (LibConfig_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could NOT find LibConfig++")
    ENDIF (LibConfig_FIND_REQUIRED)
ENDIF (LIBCONFIGPP_FOUND)

MARK_AS_ADVANCED(LIBCONFIGPP_INCLUDE_DIR LIBCONFIGPP_LIBRARIES)

