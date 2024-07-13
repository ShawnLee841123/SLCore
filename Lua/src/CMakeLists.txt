cmake_minimum_required(VERSION 3.0.0)

#lib:	13
#	lauxlib		
set(LibLauxlib "lauxlib")
#	lbaselib	
set(LibLbaselib "lbaselib")
#	lbitlib		
set(LibLcorolib "lbitlib")
#	lcorolib	
set(LibLdblib "lcorolib")
#	ldblib		
set(LibLiolib "ldblib")
#	liolib		
set(LibLinitlib "liolib")
#	linit
set(LibLmathlib "linit")
#	lmathlib
set(LibLoadlib "lmathlib")
#	loadlib
set(LibLoadlib "loadlib")
#	loslib
set(LibLoslib "loslib")
#	lstrlib
set(LibLstrlib "lstrlib")
#	ltablib
set(LibLtablib "ltablib")
#	lutf8lib
set(LibLutf8lib "lutf8lib")

#Core object:	20
#	lapi
#	lcode
#	lctype
#	ldebug
#	ldo
#	ldump
#	lfunc
#	lgc
#	llex
#	lmem
#	lobject
#	lopcodes
#	lparser
#	lstate
#	lstring
#	ltable
#	ltm
#	lundump
#	lvm
#	lzio
set(Liblualib "liblua")
#lib	dll	1
#	lua

#Executable	2
#	lua
#	luac

#------------------------------------------------------------------------------
#	public base include files
set(public_include_base_head_files
	lprefix.h
	lua.h
	luaconf.h
	)
source_group("Lib\\public\\Include" FILES ${public_include_base_head_files})
set(public_base_include_files ${public_include_base_head_files})

#	public base lib include files
set(public_include_base_lib_head_files
	lauxlib.h
	lualib.h
	)
source_group("Lib\\baselib\\Include" FILES ${public_include_base_lib_head_files})
set(public_base_lib_include_files ${public_include_base_lib_head_files})
#------------------------------------------------------------------------------
#	lauxlib
set(lauxlib_files_source_files
	lauxlib.c
	)
source_group("Lib\\lauxlib\\Srouce" FILES ${lauxlib_files_source_files})
set(lauxlib_source_files ${lauxlib_files_source_files})

set(LAUXLIB_ 
	${public_base_include_files}
	${public_include_base_lib_head_files}
	${lauxlib_files_source_files}
	)
#------------------------------------------------------------------------------
#	lbaselib
set(lbaselib_files_source_files
	lbaselib.c
	)
source_group("Lib\\lbaselib\\Source" FILES ${lbaselib_files_source_files})
set(lbaselib_source_files ${lbaselib_files_source_files})

set(LBASELIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lbaselib_source_files}
	)
#------------------------------------------------------------------------------
#	lbitlib
set(lbitlib_files_source_files
	lbitlib.c)
source_group("Lib\\lbitlib\\Source" FILES ${lbitlib_files_source_files})
set(lbitlib_source_files ${lbitlib_files_source_files})

set(LBITLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lbitlib_source_files}
	)
#------------------------------------------------------------------------------
#	lcorolib
set(lcorolib_files_source_files
	lcorolib.c)
source_group("Lib\\lcorolib\\Source" FILES ${lcorolib_files_source_files})
set(lcorolib_source_files ${lcorolib_files_source_files})

set(LCOROLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lcorolib_source_files}
	)
#------------------------------------------------------------------------------
#	ldblib
set(ldblib_files_source_files
	ldblib.c)
source_group("Lib\\ldblib\\Source" FILES ${ldblib_files_source_files})
set(ldblib_source_files ${ldblib_files_source_files})

set(LDBLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${ldblib_source_files}
	)
#------------------------------------------------------------------------------
#	liolib
set(liolib_files_source_files
	liolib.c)
source_group("Lib\\liolib\\Source" FILES ${liolib_files_source_files})
set(liolib_source_files ${liolib_files_source_files})

set(LIOLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${liolib_source_files}
	)
#------------------------------------------------------------------------------
#	linit
set(linit_files_source_files
	linit.c)
source_group("Lib\\linit\\Source" FILES ${linit_files_source_files})
set(linit_source_files ${linit_files_source_files})

set(LINIT_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${linit_source_files}
	)
#------------------------------------------------------------------------------
#	lmathlib
set(lmathlib_files_source_files
	lmathlib.c)
source_group("Lib\\lmathlib\\Source" FILES ${lmathlib_files_source_files})
set(lmathlib_source_files ${lmathlib_files_source_files})

set(LMATHLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lmathlib_source_files}
	)
#------------------------------------------------------------------------------
#	loadlib
set(loadlib_files_source_files
	loadlib.c)
source_group("Lib\\loadlib\\Source" FILES ${loadlib_files_source_files})
set(loadlib_source_files ${loadlib_files_source_files})

set(LOADLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${loadlib_source_files}
	)
#------------------------------------------------------------------------------
#	loslib
set(loslib_files_source_files
	loslib.c)
source_group("Lib\\loslib\\Source" FILES ${loslib_files_source_files})
set(loslib_source_files ${loslib_files_source_files})

set(LOSLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${loslib_source_files}
	)
#------------------------------------------------------------------------------
#	lstrlib
set(lstrlib_files_source_files
	lstrlib.c)
source_group("Lib\\lstrLib\\Source" FILES ${lstrlib_files_source_files})
set(lstrlib_source_files ${lstrlib_files_source_files})

set(LSTRLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lstrlib_source_files}
	)
#------------------------------------------------------------------------------
#	ltablib
set(ltablib_files_source_files
	ltablib.c)
source_group("Lib\\ltablib\\Source" FILES ${ltablib_files_source_files})
set(ltablib_source_files ${ltablib_files_source_files})

set(LTABLIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${ltablib_source_files}
	)
#------------------------------------------------------------------------------
#	lutf8lib
set(lutf8lib_files_source_files
	lutf8lib.c)
source_group("Lib\\lutf8lib\\Source" FILES ${lutf8lib_files_source_files})
set(lutf8lib_source_files ${lutf8lib_files_source_files})

set(LUTF8LIB_ 
	${public_base_include_files}
	${public_base_lib_include_files}
	${lutf8lib_source_files}
	)
#------------------------------------------------------------------------------
#Core Object
set(liblua_core_files_source_files
	lapi.c
	lcode.c
	lctype.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	llex.c
	lmem.c
	lobject.c
	lopcodes.c
	lparser.c
	lstate.c
	lstring.c
	ltable.c
	ltm.c
	lundump.c
	lvm.c
	lzio.c)
source_group("Lib\\liblua\\Source" FILES ${liblua_core_files_source_files})
set(liblua_core_source_files ${liblua_core_files_source_files})

set(liblua_core_files_head_files
	lapi.h
	llimits.h
	lstate.h
	lobject.h
	ltm.h
	lzio.h
	lmem.h
	ldebug.h
	ldo.h
	lfunc.h
	lgc.h
	lstring.h
	ltable.h
	lundump.h
	lvm.h
	lcode.h
	llex.h
	lopcodes.h
	lparser.h
	ldebug.h
	ltm.h
	)
source_group("Lib\\liblua\\Include" FILES ${liblua_core_files_head_files})
set(liblua_core_head_heads ${liblua_core_files_head_files})

set(LIBLUA_
	${public_base_include_files}
	${public_base_lib_include_files}
	${liblua_core_head_heads}
	${liblua_core_source_files}
	${lauxlib_source_files}
	${lbaselib_source_files}
	${lbitlib_source_files}
	${lcorolib_source_files}
	${ldblib_source_files}
	${liolib_source_files}
	${linitlib_source_files}
	${lmathlib_source_files}
	${loslib_source_files}
	${loadlib_source_files}
	${lstrlib_source_files}
	${ltablib_source_files}
	${lutf8lib_source_files}
	)
#------------------------------------------------------------------------------
#lua
set(lua_files_source_files
	lua.c)
source_group("Execute\\lua\\Source" FILES ${lua_files_source_files})
set(lua_source_files ${lua_files_source_files})

#	need base public include files
#	public base lib include files
set(LUA_EXE_
	${public_base_include_files}
	${public_base_lib_include_files}
	${lua_source_files})
set(EXE_LUA "lua")
#------------------------------------------------------------------------------
#luac
set(luac_files_source_files
	luac.c)
source_group("Execute\\luac\\Source" FILES ${luac_files_source_files})
set(luac_source_files ${luac_files_source_files})

#	public base include files
#	liblua_core_head_heads
#set(luac_files_head_files
#	lauxlib.h
#	)
#source_group("Execute\\luac\\Include" FILES ${luac_files_head_files})
#set(luac_head_files ${luac_files_head_files})

set(LUAC_EXE_
	${public_base_include_files}
	${public_base_lib_include_files}
	${luac_source_files})
set(EXE_LUAC "luac")
#------------------------------------------------------------------------------

#Add_LIBRARY(LibLauxlib STATIC ${LAUXLIB_})
#Add_LIBRARY(LibLbaselib STATIC ${LBASELIB_})
#Add_LIBRARY(LibLbitlib STATIC ${LBITLIB_})
#Add_LIBRARY(LibLcorolib STATIC ${LCOROLIB_})
#Add_LIBRARY(LibLdblib STATIC ${LDBLIB_})
#Add_LIBRARY(LibLiolib STATIC ${LIOLIB_})
#Add_LIBRARY(LibLinit STATIC ${LINIT_})
#Add_LIBRARY(LibLmathlib STATIC ${LMATHLIB_})
#Add_LIBRARY(LibLoslib STATIC ${LOSLIB_})
#Add_LIBRARY(LibLstrlib STATIC ${LSTRLIB_})
#Add_LIBRARY(LibLtablib STATIC ${LTABLIB_})
#Add_LIBRARY(LibLoadlib STATIC ${LOADLIB_})
#Add_LIBRARY(LibLutf8lib STATIC ${LUTF8LIB_})
ADD_LIBRARY(${Liblualib} STATIC ${LIBLUA_})

ADD_EXECUTABLE(${EXE_LUA} ${LUA_EXE_})
ADD_DEPENDENCIES(${EXE_LUA} ${Liblualib})
TARGET_LINK_LIBRARIES(${EXE_LUA} ${Liblualib})

ADD_EXECUTABLE(${EXE_LUAC} ${LUAC_EXE_})
ADD_DEPENDENCIES(${EXE_LUAC} ${Liblualib}) 
TARGET_LINK_LIBRARIES(${EXE_LUAC} ${Liblualib})




