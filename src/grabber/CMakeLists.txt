# pylon stuff
include_directories( grabber /opt/pylon5/include )
link_directories( /opt/pylon5/lib64 )

# images grabber
add_executable( grabber openCVGrab.cpp )
target_link_libraries( grabber ${OpenCV_LIBS} pylonutility pylonbase pylonc GCBase_gcc_v3_0_Basler_pylon_v5_0 )

# images grabber
add_executable( parameters GenericParameterAccess.cpp )
target_link_libraries( parameters pylonutility pylonbase pylonc GCBase_gcc_v3_0_Basler_pylon_v5_0 )

# video player
add_executable( player openCVPlay.cpp )
target_link_libraries( player ${OpenCV_LIBS} )
