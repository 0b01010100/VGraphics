#include "VWindow.hpp"
#include "MS/public.c"
#include <stdio.h>
//a window that wiill encapsulate the complexity of window managment
VE::Window::VWindow::VWindow(const wchar_t* Name, long SizeX, long SizeY, VWAPI Api)
{
	this->RenderingApi = Api;
	//selection of which api to use the create the window
	switch (Api)
	{
	case VWAPI::MSw:
			this->phwnd = MSW_init(this, Name, SizeX, SizeY);
			break;
	case VWAPI::OGLw:
			break;
	case VWAPI::VKw:
			break;
		default:
			break;
	}
	//check if phwnd in nullptr if so there was a problem creating the window
	if (phwnd == nullptr) 
	{
		printf("Window creation didn't go so well in File: %ls", __FILEW__);
	}
}
//update the window
void VE::Window::VWindow::Update()
{
	this->update(this->phwnd);
}

VE::Window::VWRect VE::Window::VWindow::GetClientWindowRect()
{
	long rect[4] = {};
	this->getClientWindowRect(this->phwnd, rect);
	return VWRect(rect[0],rect[1], rect[2], rect[3]);
}

unsigned int VE::Window::VWindow::GetWindowingApi() const noexcept
{
	return static_cast<unsigned int>(this->RenderingApi);
}

void* VE::Window::VWindow::GetWindowHandle() const noexcept
{
	return this->phwnd;
}

bool VE::Window::VWindow::HasUserFocus() const noexcept
{
	return this->hasUserFocus(this->phwnd);
}

