#include "VGraphicsResourceChief.hpp"

VE::Graphics::VGraphicsResourceChief::VGraphicsResourceChief()
{
}

VE::Graphics::Resources::VMesh VE::Graphics::VGraphicsResourceChief::createMesh
(
	void* verties,//please allocate using the new keyword. memory will be handled for you
	unsigned int vertexSize, 
	unsigned int vertexCount, 
	std::vector<unsigned int> indices, 
	void* cb, 
	unsigned int cbSize
)
{
	return VE::Graphics::Resources::VMesh(verties, vertexSize, vertexCount, indices, cb, cbSize);
}
void* VE::Graphics::VGraphicsResourceChief::CreateFragmentShader(const wchar_t* file_path, const char* entry_point)
{
	return this->createFragmentShader(this->renderSystem, file_path, entry_point);
}

void* VE::Graphics::VGraphicsResourceChief::CreateVertexShader(const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout)
{
	return this->createVertexShader(this->renderSystem, file_path, entry_point, inputLayout);
}


VE::Graphics::Resources::VMesh VE::Graphics::VGraphicsResourceChief::createMesh
(
	void* verties,//please allocate using the new keyword. memory will be handled for you
	unsigned int vertexSize,
	unsigned int vertexCount,
	unsigned int indices[],
	unsigned int indicesCount,
	void* cb,
	unsigned int cbSize
)
{
	std::vector<unsigned int> vindices;
	for (unsigned int i = 0; i < indicesCount; i++) 
	{
		vindices.push_back(indices[i]);
	}
	return VE::Graphics::Resources::VMesh(verties, vertexSize, vertexCount, vindices, cb, cbSize);
}