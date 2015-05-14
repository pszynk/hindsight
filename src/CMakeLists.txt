# libraries needed
SET (project_LIBS ${EO_LIBRARIES} ${TOKEXT_LIBRARIES} ${LOG4CXX_LIBRARIES} ${LIBCONFIGPP_LIBRARIES})
MESSAGE("libraries " ${project_LIBS})
# name of binaries
SET (project_BIN ${PROJECT_NAME})
# file with main
SET (main_SRC "main.cpp")

INCLUDE_DIRECTORIES(".")
# all headers and sources
FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
# remove file with test main
#LIST (REMOVE_ITEM project_SRCS ${main_SRC})

#SET (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

#ADD_LIBRARY(corelib OBJECT ${project_SRCS})

#ADD_EXECUTABLE(hindsightexe ${main_SRC} $<TARGET_OBJECTS:corelib>)
ADD_EXECUTABLE(hindsightexe ${project_SRCS})
TARGET_LINK_LIBRARIES(hindsightexe ${project_LIBS})
SET_TARGET_PROPERTIES(
    hindsightexe PROPERTIES
    VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
    OUTPUT_NAME ${project_BIN}
    CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS hindsightexe  DESTINATION bin)
