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

##-----------------------------------------------------------------

set(SL_NETWORK_WINDOWS_CORE_SOURCES_files_SourceFiles
	NetWorkCore.h
	WinNetWork/NetWorkCore.cpp
)

source_group("NetWorkCore\\NetWork" FILES ${SL_NETWORK_WINDOWS_CORE_SOURCES_files_SourceFiles})

set(SLWinNetWork_source_files ${SL_NETWORK_WINDOWS_CORE_SOURCES_files_SourceFiles})

#------------------------------------------------------------------

set(SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles
	NetWorkCore.h
	LinuxNetWork/NetWorkCore.cpp
)

source_group("NetWorkCore\\NetWork" FILES ${SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles})

set(SLLinuxNetWork_source_files ${SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles})

#******************************************************************

if (WIN32)
	add_definitions(-D_WIN)

	set(SOURCES_
		${SLNetWorkCore_source_files}
		${SLWinNetWork_source_files}
		${SOURCES_Interface}
	)
else(WIN32)
	set(SOURCES_
		${SLNetWorkCore_source_files}
		${SLLinuxNetWork_source_files}
		${SOURCES_Interface}
	)
endif(WIN32)



SET(ProjectName SLNetWorkCore)
ADD_LIBRARY(${ProjectName} SHARED ${SOURCES_})
ADD_DEPENDENCIES(${ProjectName} PublicLib)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ProtectedCore")
