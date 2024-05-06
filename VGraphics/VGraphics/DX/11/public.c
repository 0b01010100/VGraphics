#pragma once
//initialize DirectX 11 Api using hwnd
//@param hwnd -> a generic pointer to a HWND__ data type which is an the id of a window in windows.h. 
//@param graphics -> a generic pointer to a HIGH LEVEL VERSION OF THE GRAPHICS ENGINE(for example VGraphics from Virtuniy Engine Standard)
//We need this id in order to to tell DX11 what window should be set up with a rendered system
void DX11_init(void* hwnd, void* graphics, void* manager);


//will realese all of the DX11 rendering devices and resources to avoid memory leaks  
//allows us to tell the DX11 renderer to render a mesh or shap
//@param rs -> a pointer to a DX11RenderingDevs which will be represented on a hight level as a render system (or rs)
//@param graphics -> a generic pointer to a HIGH LEVEL VERSION OF THE GRAPHICS ENGINE(for example VGraphics from Virtuniy Engine Standard)
void DX11_uninit(void* rs, void* graphics);

