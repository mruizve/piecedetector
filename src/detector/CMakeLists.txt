file( GLOB Detect_SRCS detector.cpp )
add_library( detect SHARED ${Detect_SRCS} )
target_link_libraries( detect operators ${OpenCV_LIBS} )

file( GLOB Main_SRCS main.cpp )
add_executable( detector ${Main_SRCS} )
target_link_libraries( detector detect ${OpenCV_LIBS} )
