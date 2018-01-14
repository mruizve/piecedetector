file( GLOB Operator_SRCS *.cpp )
add_library( operators SHARED ${Operator_SRCS} )
target_link_libraries( operators ${OpenCV_LIBS} )
