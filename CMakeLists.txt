cmake_minimum_required( VERSION 2.8 )

project( piecedetector )

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
#	include_directories( "${OPENCV_CONFIG_PATH}/include" )
else(OpenCV_FOUND )
	message( FATAL_ERROR "PB->OPENCV = ${OpenCV_INCLUDE_DIRS}" )
endif( OpenCV_FOUND )

# Dediche method for edge detection
#file( GLOB Dediche_SRCS dediche.cpp shapes.cpp )
#add_executable( dediche ${Dediche_SRCS} )
#target_link_libraries( dediche ${OpenCV_LIBS} )

# Manuel method
file( GLOB Main_SRCS main.cpp shapes.cpp )
add_executable( detector ${Main_SRCS} )
target_link_libraries( detector ${OpenCV_LIBS} )


