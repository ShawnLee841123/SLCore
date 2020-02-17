
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
include(${CMAKE_SOURCE_DIR}/Common.cmake)
include(${CMAKE_SOURCE_DIR}/CoreInterface.cmake)


##-----------------------------------------------------------------
set(SL_LOG_CORE_SOURCES_files_SourceFiles
	Module.h
	Module.cpp
	SLC_LogCore.h
	SLC_LogCore.cpp
)

source_group("LogCore" FILES ${SL_LOG_CORE_SOURCES_files_SourceFiles})

set(SLLogCore_source_files ${SL_LOG_CORE_SOURCES_files_SourceFiles})


##-----------------------------------------------------------------

if(WIN32)
	add_definitions(-D_WIN_)
else(WIN32)
endif(WIN32)

set(SOURCES_
	${SLLogCore_source_files}	
	${SOURCES_Interface}
)

SET(ProjectName SLLogCore)
ADD_LIBRARY(${ProjectName} SHARED ${SOURCES_})
ADD_DEPENDENCIES(${ProjectName} PublicLib)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ProtectedCore")


