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
	WinNetWork/WinICOPManager.h
	WinNetWork/WinICOPManager.cpp
)

source_group("NetWorkCore\\NetWork" FILES ${SL_NETWORK_WINDOWS_CORE_SOURCES_files_SourceFiles})

set(SLWinNetWork_source_files ${SL_NETWORK_WINDOWS_CORE_SOURCES_files_SourceFiles})

#------------------------------------------------------------------

set(SL_NETWORK_WINDOWS_ICOP_SOURCES_files_SourceFiles
	WinNetWork/WinCompletionPort/WinCompletionPortListener.h
	WinNetWork/WinCompletionPort/WinCompletionPortListener.cpp
	WinNetWork/WinCompletionPort/WinCompletionPortWorker.h
	WinNetWork/WinCompletionPort/WinCompletionPortWorker.cpp
	WinNetWork/WinCompletionPort/WinCompletionPortQueue.h
	WinNetWork/WinCompletionPort/WinCompletionPortQueue.cpp
)

source_group("NetWorkCore\\NetWork\\WinCompletionPort" FILES ${SL_NETWORK_WINDOWS_ICOP_SOURCES_files_SourceFiles})

set(SLWinNetWorkComPort_source_files ${SL_NETWORK_WINDOWS_ICOP_SOURCES_files_SourceFiles})

#------------------------------------------------------------------

set(SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles
	NetWorkCore.h
	LinuxNetWork/NetWorkCore.cpp
)

source_group("NetWorkCore\\NetWork" FILES ${SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles})

set(SLLinuxNetWork_source_files ${SL_NETWORK_LINUX_CORE_SOURCES_files_SourceFiles})

#------------------------------------------------------------------

set(SL_NETWORK_COMMON_SOURCES_files_SourceFiles
	CommonDefine/SocketOperateDefine.h
	CommonDefine/SocketOperateDefine.cpp
	CommonDefine/NetWorkCoreEnumDefine.h
)

source_group("NetWorkCore\\Common" FILES ${SL_NETWORK_COMMON_SOURCES_files_SourceFiles})

set(SLNetWorkCommon_source_files ${SL_NETWORK_COMMON_SOURCES_files_SourceFiles})

#------------------------------------------------------------------

#******************************************************************

if (WIN32)
	add_definitions(-D_WIN_)

	set(SOURCES_
		${SLNetWorkCore_source_files}
		${SLWinNetWork_source_files}
		${SLNetWorkCommon_source_files}
		${SLWinNetWorkComPort_source_files}
		${SOURCES_Interface}
	)
else(WIN32)
	set(SOURCES_
		${SLNetWorkCore_source_files}
		${SLLinuxNetWork_source_files}
		${SLNetWorkCommon_source_files}
		${SOURCES_Interface}
	)
endif(WIN32)



SET(ProjectName SLNetWorkCore)
ADD_LIBRARY(${ProjectName} SHARED ${SOURCES_})
ADD_DEPENDENCIES(${ProjectName} PublicLib)

if(WIN32)
	TARGET_LINK_LIBRARIES(${ProjectName} PublicLib "ws2_32.lib")
else(WIN32)
	TARGET_LINK_LIBRARIES(${ProjectName} PublicLib)
endif(WIN32)

SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ProtectedCore")
