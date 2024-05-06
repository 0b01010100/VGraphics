#pragma once
namespace VE 
{
	namespace Window
	{
		struct VWRect
		{
			long    left;
			long    top;
			long    right;
			long    bottom;
		};
		//used for speciying the Api to use
		enum class VWAPI : unsigned int
		{
#ifdef _WIN32
			//Window Api by Microsoft
			MSw,
#endif
			//Window Api by Silicon Graphics, Inc.
			OGLw,
			//Other Window Api by Khronos Group
			VKw
		};
		//An Class that encapsulates the Api specific functionality of a Virtuel window
		class VWindow
		{
		public:
			//initialize the window
			//@param Name -> Name of the window 
			//@param SizeX -> Width of the window
			//@param SizeY -> Height of the window
			//@param Api -> the api that should be used for creating the window. For example MSw, OGLw, VKw 
			VWindow(const wchar_t* Name, long SizeX, long SizeY, VWAPI Api);
			//updates the window by calling its event handler
			void Update();
			//get the sizeof the client area of the window
			//@return VWRect
			VWRect GetClientWindowRect();
			//will return the a number that represents the current Api being used by this Windowing Engine
			//@return unsigned int
			unsigned int GetWindowingApi() const noexcept;
			//will return Api Specific Handle as a void * (warning you my only want to do this when creating the Graphics Engine)
			//@return void *
			void* GetWindowHandle() const noexcept;
			//Get weather the window has the user's focus or not
			//@return bool
			bool HasUserFocus() const noexcept;
		private:
			//POINTERS TO LOW LEVEL API SPECIFIC WAYS OF INTERACTING WITH A WINDOW
			//WARNING ANY MODIFICATION TO THE INTERFACE CAN LEAD TO MEMORY LEAKS
			//________________________________________________________________
			//used to point to an Api specfic function that will update the window
			void(*update)(void* phwnd);
			void(*getClientWindowRect)(void* phwnd, long rect[4]);
			bool(*hasUserFocus)(void* phwnd);
			//used to pointer to an Api specfic id for a window
			void* phwnd;
			//________________________________________________________________
		public:
			VWAPI RenderingApi;
		};
	}

}


