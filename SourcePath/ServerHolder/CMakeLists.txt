##-------------------------------------------------------------------------
##
##-------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/CoreInterface.cmake)
include(${CMAKE_SOURCE_DIR}/ExecuteAble.cmake)

if(WIN32)
	add_definitions(-D_WIN_)
	
endif(WIN32)

set(SERVER_HOLDER_files_sources_files
	ServerHolder.cpp
	ServerCore.h
	ServerCore.cpp
	ServerCoreModuleInterfaceContainer.h
	ServerCoreModuleInterfaceContainer.cpp
)

source_group("ServerHolder" FILES ${SERVER_HOLDER_files_sources_files})

set(ServerHolder_source_files  ${SERVER_HOLDER_files_sources_files})


set(SOURCES_
	${ServerHolder_source_files}
	${SOURCES_Interface}
)

	
SET(ProjectName ServerHolder)

##-----------------------------------------------------------------
##	set stack memory size 999999KB	not right
##-----------------------------------------------------------------
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl, --stack, 999999")
#MATH(EXPR stack_size "4*1024*1024")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

ADD_EXECUTABLE(${ProjectName} ${SOURCES_})
#ADD_DEPENDENCIES(${ProjectName} PublicLib InterNetCore)
#TARGET_LINK_LIBRARIES(${ProjectName} PublicLib InterNetCore)
ADD_DEPENDENCIES(${ProjectName} PublicLib LibTools)
#if(WIN32)
#else(WIN32)
#TARGET_LINK_LIBRARIES(${ProjectName} c stdc++ dl rt m )
#endif(WIN32)
TARGET_LINK_LIBRARIES(${ProjectName} PublicLib LibTools)
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "ExecuteAble")
