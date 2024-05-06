if(WIN32)
    set(DX11 "DX/11/DX11.cpp")
endif()

add_library(VGraphics STATIC
	"${DX11}"
	"VGraphicsResourceChief.cpp"
	"VGraphics.cpp"
)



