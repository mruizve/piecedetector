cmake_minimum_required( VERSION 2.8 )

project( piecedetector )

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
#	include_directories( "${OPENCV_CONFIG_PATH}/include" )
else(OpenCV_FOUND )
	message( FATAL_ERROR "PB->OPENCV = ${OpenCV_INCLUDE_DIRS}" )
endif( OpenCV_FOUND )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib" )

subdirs( src )
