# flexibowl library
add_library( flexibowl SHARED
	command.cpp
	flexibowl.cpp
	interface.cpp
	magic.cpp )
target_link_libraries( flexibowl utils network pthread rt )

# flexibowl command line interface
add_executable( flexibowl-bin main.cpp )
set_target_properties( flexibowl-bin PROPERTIES OUTPUT_NAME flexibowl )
target_link_libraries( flexibowl-bin flexibowl utils network pthread rt )
