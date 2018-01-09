file( GLOB Dediche_SRCS dediche.cpp shapes.cpp )
add_executable( dediche ${Dediche_SRCS} )
target_link_libraries( dediche ${OpenCV_LIBS} )
