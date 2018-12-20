# tetrominoes detection library
add_library( tetrominoes SHARED
	detector.cpp
	operators/binary.cpp
	operators/blurring.cpp
	operators/border.cpp
	operators/canny.cpp
	operators/hough.cpp
	operators/lines.cpp
	operators/morphological.cpp
	operators/operator.cpp
	operators/rectangle.cpp
	operators/suzuki.cpp )
target_link_libraries( tetrominoes ${OpenCV_LIBS} )

# tetrominoes detector example
add_executable( detector main.cpp )
target_link_libraries( detector tetrominoes ${OpenCV_LIBS} )
