# Dediche method for edge detection
#file( GLOB Dediche_SRCS dediche.cpp shapes.cpp )
#add_executable( dediche ${Dediche_SRCS} )
#target_link_libraries( dediche ${OpenCV_LIBS} )

# Manuel method
file( GLOB Main_SRCS main.cpp shapes.cpp )
add_executable( detector ${Main_SRCS} )
target_link_libraries( detector ${OpenCV_LIBS} )
