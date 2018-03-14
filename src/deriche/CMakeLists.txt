file( GLOB Deriche_SRCS deriche.cpp shapes.cpp )
add_executable( deriche ${Deriche_SRCS} )
target_link_libraries( deriche ${OpenCV_LIBS} )
