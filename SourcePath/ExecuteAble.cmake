if(WIN32)
	add_definitions(-D_CONSOLE)
	add_definitions(-D_UNICODE)
else(WIN32)
	TARGET_LINK_LIBRARIES({$ProjectName} c stdc++ dl rt m)
endif(WIN32)

