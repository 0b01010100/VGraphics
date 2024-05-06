#pragma once
#include<vector>
namespace VE
{
	namespace Graphics 
	{
		namespace Resources 
		{

			enum V_INPUT_LAYOUT_FORMAT : unsigned int
			{
				//Represents a vector format with four 32-bit floating-point components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32A32_FLOAT = 2,
				//Represents a vector format with four 32 - bit unsigned integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32A32_UINT = 3,
				//Represents a vector format with four 32-bit signed integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32A32_SINT = 4,

				//Represents a vector format with three 32-bit floating-point components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32_FLOAT = 6,
				//Represents a vector format with three 32-bit unsigned integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32_UINT = 7,
				//Represents a vector format with three 32 - bit signed integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32B32_SINT = 8,

				//Represents a vector format with two 32-bit floating-point components.
				V_INPUT_LAYOUT_FORMAT_R32G32_FLOAT = 16,
				//Represents a vector format with two 32-bit unsigned integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32_UINT = 17,
				//Represents a vector format with two 32 - bit signed integer components.
				V_INPUT_LAYOUT_FORMAT_R32G32_SINT = 18
			};
			enum V_Primitive_Topology : unsigned int
			{
				//Interpret the vertex data as a list of points.
				V_Primitive_Topology_Point_List = 1,
				//Interpret the vertex data as a list of lines.
				V_Primitive_Topology_Line_List,
				//Interpret the vertex data as a line strip.
				V_Primitive_Topology_Line_Strip,
				//Interpret the vertex data as a list of triangles.
				V_Primitive_Topology_Triangle_List,
				//Interpret the vertex data as a triangle strip.
				V_Primitive_Topology_Triangle_Strip,
			};
			static bool operator>=(enum V_INPUT_LAYOUT_FORMAT ls, unsigned int rs)
			{
				return (static_cast<unsigned int>(ls) >= rs);
			}
			static bool operator<=(enum V_INPUT_LAYOUT_FORMAT ls, unsigned int rs)
			{
				return (static_cast<unsigned int>(ls) <= rs);
			}

			//Helps get the shader to know the memory layout of each Vertex Samantic or Attribute
			struct VInputLayout
			{
				//input layout
				enum V_INPUT_LAYOUT_FORMAT* inputLayouts;
				//number of user defined samatics
				unsigned int inputLayoutsElementCount;
				//Name of user defined samatics
				const char** inputLayoutElementNames;
			};
	
			class VMesh
			{
			public:
				VMesh
				(
				) 
				{
					this->verties = 0;
					this->vertexSize = 0;
					this->vertexCount = 0;
					this->indices;
					this->cb = 0;
					this->cbSize = 0;
				}
				VMesh
				(
					void* verties,
					unsigned int vertexSize,
					unsigned int vertexCount,
					std::vector<unsigned int> indices,
					void* cb,
					unsigned int cbSize
				)
				{
					this->verties = verties;
					this->vertexSize = vertexSize;
					this->vertexCount = vertexCount;
					this->indices = indices;
					this->cb = cb;
					this->cbSize = cbSize;
				}
			public:
				void* verties;
				unsigned int vertexSize;
				unsigned int vertexCount;
				std::vector<unsigned int> indices;
				void* cb;
				unsigned int cbSize;
				unsigned int id;
			};
		};
		
	};
};