#include "..\..\VGraphicsResourceChief.hpp"
// filesystem standard header by MICROSOFT
#include <filesystem>
// filesystem standard header by MICROSOFT
#include <iostream>
// fstream standard header by MICROSOFT
#include <fstream>
// string standard header by MICROSOFT
#include <string>
// sstream standard header by MICROSOFT
#include <sstream>
//allows us to be able to the the d3dcompiler library my MICROSOFT
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
//allows us to be able to the the d3d11 library 
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <wrl/client.h>
//rendering devices
struct DX11RenderingDevs
{
	//messages swap chain buffers
	struct IDXGISwapChain* sw;
	//for creation of resources for rendering
	struct ID3D11Device* dev;
	//Used for rendering. This struct has all the fansy function for rendering 
	struct ID3D11DeviceContext* context;
	//allows us to view a swapChain buffer, so we do things like render to it
	Microsoft::WRL::ComPtr <struct ID3D11RenderTargetView> rtv;
	/*An IDXGIFactory interface implements methods for generating DXGI objects
	(which handle full screen transitions).*/
	struct IDXGIFactory* factory;
	//For DXGI objects that produce image data.
	struct IDXGIDevice* idxgi_dev;
	//Window handle
	HWND hwnd;
};


//////////////////////////////////////////////////Resource Manager (DEV)

// //creates a DX11 swap chain
//@param rs -> a pointer to DX11RenderingDevs struct, which will represent the DX11 rendering devices and swapChain
//We need this id in order to to tell DX11 what window should have the swapchain Buffers
__declspec(noinline) static void DX11_createSwapChain(struct DX11RenderingDevs* rs);

//create a Vertex Shader the DX11 way
//@param rs -> a pointer to DX11RenderingDevs struct, which will represent the DX11 rendering devices and swapChain
//@param file_path -> location in the project to acces the Vertex Shader file
//@param entry_point -> the name of the shader programs;s entry point 
//@param inputLayout -> the HIGH LEVEL input layout
__declspec(noinline) static void* DX11_createVertexShader(void* rs, const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout);

//create a Pixel Shader the DX11 way
//@param rs -> a pointer to DX11RenderingDevs struct, which will represent the DX11 rendering devices and swapChain
//@param file_path -> location in the project to acces the FragmentShader in a file
//@param entry_point -> the name of the shader programs;s entry point 
__declspec(noinline) static void* DX11_createFragmentShader(void* rs, const wchar_t* file_path, const char* entry_point);


//////////////////////////////////////////////////Renderer calls (DEVICE CONTEXT)


//allows us to present what we drew on the screen and then clear the color 
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
__declspec(noinline) static void DX11_update(void* rs, float r, float g, float b, float a);

//allows us to Set the Vertex Shader to be used in the input assembly stage of rendering 
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
//@param vertexShaderAttribs -> a generic pointer to VertexShader data (like inputLayout, VertexShader btye Code, And Samatics)
//__declspec(noinline) static void DX11_setVertexShader(void* rs, struct V_VertexShaderInfo* vsInfo);
__declspec(noinline) static void DX11_setVertexShader(void* rs, void* vsCode);

//allows us to Set the Vertex Shader to be used in the input assembly stage of rendering
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
//@param vs -> a generic pointer to Fagment/Pixel Shader btye code and meta data  
__declspec(noinline) static void DX11_setFragmentShader(void* rs, void* fs);

//allows us to tell the DX11 renderer to render a mesh or shape
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
//@param mesh -> the mesh or shap to render
__declspec(noinline) static void DX11_drawMesh(void* rs, VE::Graphics::Resources::VMesh mesh);

//allows us to tell the DX11 how to renderer to render a mesh or shape
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
//@param topology -> the way to draw the mesh
__declspec(noinline) static void DX11_SetPrimitiveTopology(void* rs, VE::Graphics::Resources::V_Primitive_Topology topology);

//Used for Error Handling related to windows only.
#define DX11_ERROR(hr, error, retvalue)\
{\
	if(FAILED(hr)){\
		std::wostringstream ws; \
		ws << L"DX11 ERROR \n" << error<< L"\n"<< L"Error Code: " << hr << "\n"; \
		OutputDebugStringW(ws.str().c_str()); \
		MessageBoxW(0, ws.str().c_str(), L"DX11 ERROR" , 0);\
		retvalue;\
	}\
\
}
//Used for warrning Handling related to windows only.
#define DX11_WARNING(expr,err_code, details)\
{\
	if(expr != 0){\
		std::wostringstream ws; \
		ws << L"DX11 WARNING \n" << details << "\n" << L"Error Code: " << err_code << "\n"; \
		OutputDebugStringW(ws.str().c_str()); \
		MessageBoxW(0, ws.str().c_str(), L"DX11 WARNING" , 0);\
	}\
\
}

//Assert if value is null
#define NULL_ERROR(arg, error, retvalue)\
{\
	if(arg == 0){\
		std::wostringstream ws; \
		ws << error<< L"\n"; \
		OutputDebugStringW(ws.str().c_str()); \
		MessageBoxW(0, ws.str().c_str(), L"NULL_ERROR" , 0);\
		retvalue;\
	}\
}