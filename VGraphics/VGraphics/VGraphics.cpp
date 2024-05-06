#include "VGraphics.hpp"
#include "DX/11/public.c"

VE::Graphics::VGraphics::VGraphics(void* WindowHandle, unsigned int RenderingApi)
{
	this->WindowHandle = WindowHandle;
	this->RenderingApi = RenderingApi;
	resourceManager = new VGraphicsResourceChief();
	switch (RenderingApi)
	{
#ifdef _WIN32
	case 0 : //VE::Window::VAPI::WINDOWS:
			DX11_init(WindowHandle, this, resourceManager);
			break;
#endif
	case  1: //VE::Window::VAPI::OGL:
			break;

	case 2:// VE::Window::VAPI::VULKAN:
			break;

		default:
			break;
	}

	//check if phwnd in nullptr if so there was a problem creating the window
	if (renderSystem == nullptr)
	{
		printf("VGraphics creation didn't go so well in File: %ls", __FILEW__);
	}
	else
	{
	
	}
	
}
VE::Graphics::VGraphics::~VGraphics()
{
	DX11_uninit(this->renderSystem, this);
}
void VE::Graphics::VGraphics::SetFragmentShader(void* fs)
{
	this->setFragmentShader(this->renderSystem, fs);
}
void VE::Graphics::VGraphics::SetVertexShader(void* vs)
{
	this->setVertexShader(this->renderSystem, vs);
}
void VE::Graphics::VGraphics::DrawMesh(VE::Graphics::Resources::VMesh mesh)
{
	this->drawMesh(this->renderSystem, mesh);
}
void VE::Graphics::VGraphics::SetClearScreenColor(float r, float g, float b, float a)
{
	this->clearColor.r = r;
	this->clearColor.g = g;
	this->clearColor.b = b;
	this->clearColor.a = a;
}
void VE::Graphics::VGraphics::Update()
{
	this->update(this->renderSystem, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}
void VE::Graphics::VGraphics::SetPrimitiveTopology(VE::Graphics::Resources::V_Primitive_Topology topology)
{
	this->setTopology(this->renderSystem, topology);
}
unsigned int VE::Graphics::VGraphics::GetRenderingAPi() const noexcept
{
	return this->RenderingApi;
}



