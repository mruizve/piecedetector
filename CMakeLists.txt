cmake_minimum_required( VERSION 2.8 )

project( smf )

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
#	include_directories( "${OPENCV_CONFIG_PATH}/include" )
else( OpenCV_FOUND )
	message( FATAL_ERROR "OPENCV = ${OpenCV_INCLUDE_DIRS}" )
endif( OpenCV_FOUND )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib" )

# allows to obtain relative paths when using the __FILE__ macro
string(LENGTH "${CMAKE_SOURCE_DIR}/" FILE_OFFSET)
add_definitions("-D__FILE_OFFSET__=${FILE_OFFSET}")

subdirs( src )
