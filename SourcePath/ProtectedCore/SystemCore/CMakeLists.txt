
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
set(SL_SYSTEM_CORE_SOURCES_files_SourceFiles
	SLC_SystemCore.h
	SLC_SystemHelper.h
	Module.h
	SLC_SystemCore.cpp
	SLC_SystemHelper.cpp
	Module.cpp
)

source_group("SystemCore" FILES ${SL_SYSTEM_CORE_SOURCES_files_SourceFiles})

set(SLSystemCore_source_files ${SL_SYSTEM_CORE_SOURCES_files_SourceFiles})

if(WIN32)
	add_definitions(-D_WIN)
else(WIN32)
endif(WIN32)

set(SOURCES_
	${SLSystemCore_source_files}
	${SOURCES_Interface}
)

SET(ProjectName SLSystemCore)
ADD_LIBRARY(${ProjectName} SHARED ${SOURCES_})
ADD_DEPENDENCIES(${ProjectName} PublicLib)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ProtectedCore")

