file( GLOB Main_SRCS main.cpp )
add_executable( detector ${Main_SRCS} )
target_link_libraries( detector ${OpenCV_LIBS} )
