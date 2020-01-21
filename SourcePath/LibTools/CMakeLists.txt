##-------------------------------------------------------------------------
##
##-------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/CoreInterface.cmake)
include(${CMAKE_SOURCE_DIR}/Common.cmake)

if(WIN32)
	add_definitions(-D_WIN_)
else(WIN32)
endif(WIN32)

set(TOOLS_LIB_INI_FILE_READER_files_sources_files	
	ExecuteIniConfigReader.cpp
	ExecuteIniConfigReader.h
)

source_group("IniFileReader" FILES ${TOOLS_LIB_INI_FILE_READER_files_sources_files})

set(IniFileReader_source_files  ${TOOLS_LIB_INI_FILE_READER_files_sources_files})


set(SOURCES_
	${IniFileReader_source_files}
	${SOURCES_Interface}
)

	
SET(ProjectName LibTools)

##-----------------------------------------------------------------
##	set stack memory size 999999KB	not right
##-----------------------------------------------------------------
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl, --stack, 999999")
#MATH(EXPR stack_size "4*1024*1024")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

ADD_LIBRARY(${ProjectName} STATIC ${SOURCES_})
#ADD_DEPENDENCIES(${ProjectName} PublicLib InterNetCore)
#TARGET_LINK_LIBRARIES(${ProjectName} PublicLib InterNetCore)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)

SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "Lib")
