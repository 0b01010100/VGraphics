#pragma once
#include "VGraphicsResourceChief.hpp"

namespace VE
{
	namespace Graphics
	{
		/*extends IGraphicsEngine_Base*/
	
		//A GRAPHICS ENGINE 
		class VGraphics
		{
		public:
			//initialize the Graphics Engine\n
			//
			//@param WindowHandle -> Handle to an Api specfic id for the window. Tip if using VWindow API by Virtunity then call the  VWindow::GetWindowHandle function 
			//@param RenderingApi -> Helps the Graphics engine know what rendering api to use for the window. Tip if using VWindow API by Virtunity then call the  VWindow::GetRenderingAPi function 
			VGraphics(void* WindowHandle, unsigned int RenderingApi);
			~VGraphics();
			//will a set a Pixel/Fragment shader for the render system to use 
			//@param fs -> Pointer to Fragment/Pixel shader 
			void SetFragmentShader(void* fs);
			//will a set a Vertex shader for the render system to use 
			//@param vs -> Pointer to Vertex shader 
			void SetVertexShader(void* vs);
			//will set the clear the color of the screen to a given color
			//@param r -> Red color channel
			//@param g -> Green color channel
			//@param b -> Blue color channel
			//@param a -> Alpha color channel
			void SetClearScreenColor(float r, float g, float b, float a);
			//will present what was draw on the render texture so we can see it 
			void Update();
			//will a set a Mesh the render system to use 
			//@param mesh -> The Mesh to draw
			void DrawMesh(VE::Graphics::Resources::VMesh mesh);
			//will allows us to tell the DX11 how to renderer to render a mesh or shape
			//@param topology -> The way to draw the Mesh
			void SetPrimitiveTopology(VE::Graphics::Resources::V_Primitive_Topology topology);
			//will return the a number that represents the current Api being used by this Graphics Engine
			//@return unsigned int
			unsigned int GetRenderingAPi() const noexcept;
		private:
			//POINTERS TO LOW LEVEL API SPECIFIC WAYS OF CREATING GRAPHICS RESOURCES
			//WARNING ANY MODIFICATION TO THE INTERFACE CAN LEAD TO MEMORY LEAKS
			//________________________________________________________________
		
			void(*setFragmentShader)(void* rs, void* fs);
			void(*setVertexShader)(void* rs, void* vs);
			void(*update)(void* rs, float r, float g, float b, float a);
			void(*drawMesh)(void* rs, VE::Graphics::Resources::VMesh mesh);
			void(*setTopology)(void* rs, VE::Graphics::Resources::V_Primitive_Topology topology);
			//pointer the the LOW LEVEL render system
			void* renderSystem;
			//________________________________________________________________
		public:
			VGraphicsResourceChief* resourceManager;
			
		private:
			//window to render to
			void * WindowHandle;
			//The rendering APi
			unsigned int RenderingApi;	
			struct VWClearColor
			{
				float r = 0, g = 0, b = 0, a = 0;
			}clearColor;
		};

	};
};