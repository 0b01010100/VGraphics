#pragma once

//Creates a Microsift specific window
static void* MSW_CreateWindow(const wchar_t* name, long SizeX, long SizeY);

//Update window
__declspec(noinline) static void MSW_UpdateWindow(void * hwnd);

//Gets the client Size of the window
__declspec(noinline) static void MSW_GetClientWindowRect(void* hwnd, long rect[4]);

//Get weather the window has the user's focus or not
__declspec(noinline) static bool MSW_HasUserFocus(void* hwnd);

