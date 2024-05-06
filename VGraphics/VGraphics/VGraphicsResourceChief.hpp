#pragma once
#include "requirements.c"
#include <vector>
namespace VE 
{
	namespace Graphics 
	{

		class VGraphicsResourceChief
		{
		public:
			VGraphicsResourceChief();
			//will allow you to create a custom mesh 
			VE::Graphics::Resources::VMesh createMesh
			(
				//this should point to an array of verties 
				void* verties, 
				//size of each vertex
				unsigned int vertexSize,
				//number of verties
				unsigned int vertexCount, 
				//Instead of specifying the vertices directly for each triangle, an index buffer is
				//used to reference vertices from the vertex buffer.
				std::vector<unsigned int> indices, 
				//constant buffer
				void* cb, 
				//size of the const buffer struct or class 
				unsigned int cbSize
			);
			//will allow you to create a custom mesh 
			VE::Graphics::Resources::VMesh createMesh
			(
				//this should point to an array of verties 
				void* verties,
				//size of each vertex
				unsigned int vertexSize,
				//number of verties
				unsigned int vertexCount,
				//Instead of specifying the vertices directly for each triangle, an index buffer is
				//used to reference vertices from the vertex buffer.
				unsigned int* indices,
				//number of indices
				unsigned int indicesCount,
				//constant buffer
				void* cb,
				//size of the const buffer struct or class 
				unsigned int cbSize
			);
		private:
			//POINTERS TO LOW LEVEL API SPECIFIC WAYS OF CREATING GRAPHICS RESOURCES
			//WARNING ANY MODIFICATION TO THE INTERFACE CAN LEAD TO MEMORY LEAKS
			//________________________________________________________________
			void*(*createVertexShader)(void* rs, const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout);
			void*(*createFragmentShader)(void* rs, const wchar_t* file_path, const char* entry_point);
			//pointer the the LOW LEVEL render system
			void* renderSystem = nullptr;
			//________________________________________________________________
		public:
			//will a create a Pixel/Fragment shader for the render system to use 
			//@param file_path -> the destination of the Fragment/Pixel shader file
			//@param entry_point -> the entry point function
			void* CreateFragmentShader(const wchar_t* file_path, const char* entry_point);
			//will a create a Vertex shader for the render system to use 
			//@param file_path -> the destination of the Vertex shader file
			//@param entry_point -> the entry point function
			//@param inputLayout -> Custom Samantics YOU put in the shader. (nullptr if no custom Samantics)
			void* CreateVertexShader(const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout);

		};
	};
};