#include "../VGraphics/VGraphics.hpp"
#include "../VWindow/VWindow.hpp"
int main() 
{
	VE::Window::VWindow w = VE::Window::VWindow(L"Scene", 0, 0, VE::Window::VWAPI::MSw);
	VE::Graphics::VGraphics g = VE::Graphics::VGraphics(w.GetWindowHandle(), w.GetWindowingApi());
	g.SetClearScreenColor(0, 0, 0, 0);
	while (true)
	{
		w.Update(); 
		g.Update(); 
	}
}