if(WIN32)
   set(MSW "MS/Window.cpp")
endif()

add_library(VWindow STATIC 
	"${MSW}"
	"VWindow.cpp"
 )

target_include_directories(VWindow PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")