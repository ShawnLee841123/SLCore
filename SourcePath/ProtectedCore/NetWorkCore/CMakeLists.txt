##-----------------------------------------------------------------
##
##
##
##
##
##
##
##
##-----------------------------------------------------------------

##-----------------------------------------------------------------
##	Import files
##-----------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/CoreInterface.cmake)
include(${CMAKE_SOURCE_DIR}/Common.cmake)

##-----------------------------------------------------------------
set(SL_NETWORK_CORE_SOURCES_files_SourceFiles
	Module.h
	Module.cpp
)

source_group("NetWorkCore" FILES ${SL_NETWORK_CORE_SOURCES_files_SourceFiles})

set(SLNetWorkCore_source_files ${SL_NETWORK_CORE_SOURCES_files_SourceFiles})



#******************************************************************

if (WIN32)
	add_definitions(-D_WIN)
else(WIN32)
endif(WIN32)

set(SOURCES_
	${SLNetWorkCore_source_files}
	${SOURCES_Interface}
)

SET(ProjectName SLNetWorkCore)
ADD_LIBRARY(${ProjectName} SHARED ${SOURCES_})
ADD_DEPENDENCIES(${ProjectName} PublicLib)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ProtectedCore")
