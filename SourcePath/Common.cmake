

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(WIN32)
	add_definitions(/W3)
else(WIN32)
	add_definitions(-fPIC -std=c++11)
	add_definitions(-DNDEBUG)
	add_definitions(-g -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
endif(WIN32)


