if(WIN32)
else(WIN32)
	TARGET_LINK_LIBRARIES({$ProjectName} c stdc++ dl rt m)
endif(WIN32)

