#include "public.c"
#include "private.cpp"

enum D3D_DRIVER_TYPE drivers[] =
{
	//Drawing is mainly Executed to the GPU when using this Driver
	D3D_DRIVER_TYPE_HARDWARE,
	//This is a very fast Driver that has the CPU to do most of the Math to get thing draw on the screen.
	D3D_DRIVER_TYPE_WARP,
	//Compatbale with most DirectX Features, but is pretty slow compared the the First two Drivers.
	D3D_DRIVER_TYPE_REFERENCE
};
struct IResourceManager
{
	void*(*createVertexShader)(void* rs, const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout);
	void*(*createFragmentShader)(void* rs, const wchar_t* file_path, const char* entry_point);
	//pointer the the render system
	void* renderSystem = nullptr;
};
struct IGraphicsEngine
{
	//WARNING ANY MODIFICATION TO THE INTERFACE CAN LEAD TO MEMORY LEAKS
	//________________________________________________________________
	//will a set a Pixel/Fragment shader for the render system to use 
	void(*setFragmentShader)(void* rs, void* fs) = nullptr;
	//will a set a Vertex shader for the render system to use 
	void(*setVertexShader)(void* rs, void* vsCode) = nullptr;
	//will present what was draw on the render texture so we can see it and then will clear the screen 
	void(*update)(void* rs, float r, float g, float b, float a) = nullptr;
	//allows us to tell the renderer to render a mesh or shape
	void(*drawMesh)(void* rs, VE::Graphics::Resources::VMesh mesh) = nullptr;
	//allows us to tell the DX11 how to renderer to render a mesh or shape
	void(*setTopology)(void* rs, VE::Graphics::Resources::V_Primitive_Topology topology) = nullptr;
	//pointer the the render system
	void* renderSystem = nullptr;
	IResourceManager* manager;

};

void DX11_init(void* hwnd, void* graphics, void* manager)
{
	//GIVE THE HIGH LEVEL INTERFACE THE LOW LEVEL FUNCTIONS 
	IGraphicsEngine* ge_b = reinterpret_cast<IGraphicsEngine*>(graphics);
	
	//renderer 
	ge_b->setFragmentShader = DX11_setFragmentShader;
	ge_b->setVertexShader = DX11_setVertexShader;
	ge_b->update = DX11_update;
	ge_b->drawMesh = DX11_drawMesh;
	ge_b->setTopology = DX11_SetPrimitiveTopology;

	//init resource manager 
	IResourceManager* gem_b = reinterpret_cast<IResourceManager*>(manager);
	gem_b->createFragmentShader = DX11_createFragmentShader;
	gem_b->createVertexShader = DX11_createVertexShader;

	struct DX11RenderingDevs* rs = reinterpret_cast<DX11RenderingDevs*>(malloc(sizeof(struct DX11RenderingDevs)));
	rs->hwnd = reinterpret_cast<HWND>(hwnd);
	ge_b->renderSystem = (gem_b->renderSystem = rs);


	HRESULT hr;
	for (unsigned char index = 0; index < ARRAYSIZE(drivers);)
	{
		hr = D3D11CreateDevice(0, drivers[index], 0, 0, 0, 0, D3D11_SDK_VERSION, &rs->dev, 0, &rs->context);
		//if the creation of devices SUCCEEDED exit the loop
		if (SUCCEEDED(hr))
		{
			break;
		}
		index++;
	}
	//check for errors
	DX11_ERROR(hr, "Failed to set up DirectX 11 rendering system", return);
	//if the creation of devices FAILED throw Error
	DX11_ERROR(hr, L"Faild to get rendering Devices for DirectX 11 ", return);
	//else continue on, to get IDXGIDevice struct
	DX11_ERROR(rs->dev->QueryInterface(__uuidof(struct IDXGIDevice), reinterpret_cast<void**>(&rs->idxgi_dev)), 
		L"Faild to get a rendering Device for DirectX 11. Device is IDXGIDevice", return);
	
	//Represents your Graphics adapter 
	struct IDXGIAdapter* apdt = __nullptr;

	//else continue on, to get IDXGIAdapter struct
	DX11_ERROR(rs->idxgi_dev->GetAdapter(&apdt), 
		L"Faild to get a rendering Device for DirectX 11. Device is IDXGIAdapter( or Graphics Adapter)", return);

	//else continue on, to get IDXGIFactory struct
	DX11_ERROR(apdt->GetParent(__uuidof(struct IDXGIFactory), reinterpret_cast<void**>(&rs->factory)), 
		L"Faild to get a rendering Device for DirectX 11. Device is IDXGIFactory", return);
	//now create swapChain
	DX11_createSwapChain(rs);

	//set up defualt vertex shader and Pixel Shader for use 
	
	//INPUT LAYOUT
	VE::Graphics::Resources::VInputLayout inputLayout = {};

	const char* eleNames[2] =
	{
		"POSITION", "COLOR"
	};

	inputLayout.inputLayoutElementNames = eleNames;

	inputLayout.inputLayoutsElementCount = 2 ;

	VE::Graphics::Resources::V_INPUT_LAYOUT_FORMAT formats[2] =
	{
		//for position
		VE::Graphics::Resources::V_INPUT_LAYOUT_FORMAT::V_INPUT_LAYOUT_FORMAT_R32G32B32_FLOAT,
		//for color
		VE::Graphics::Resources::V_INPUT_LAYOUT_FORMAT::V_INPUT_LAYOUT_FORMAT_R32G32B32_FLOAT,
	};
	//VERTEX SHADER
	inputLayout.inputLayouts = formats;

	void* vsCode = DX11_createVertexShader(rs, __FILEW__ "\\.." "\\Shaders\\VSDefault.hlsl", "vsmain", &inputLayout);
	
	NULL_ERROR(
		vsCode, 
		"Failed to set succesfully vertex shader for use. Your vertex Shader was null. Check your V_VertexShaderInfo struct and or check your" __FILEW__ "\\.." "\\Shaders\\VSDefault.hlsl file",
		return
	);

	DX11_setVertexShader(rs, vsCode);

	//PIXEL SHADER
	void* psCode = DX11_createFragmentShader(rs, __FILEW__ "\\.." "\\Shaders\\PSDefault.hlsl", "psmain");
	
	NULL_ERROR(
		psCode, 
		"Failed to set succesfully pixel shader for use. Your pixel Shader was null. Check your your" __FILEW__ "\\.." "\\Shaders\\VSDefault.hlsl file",
		return
	);

	DX11_setFragmentShader(rs, psCode);
}
struct WindowData
{
	void (*resizeSwapChainBuffers)(HWND hwnd);
	DX11RenderingDevs* DirectXData;
	bool hasUserFocus;

};
static void DX11_resizeSwapChainBuffers(HWND hwnd)
{
	WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	//Get client size of the window 
	RECT wr = { 0 };
	GetClientRect(hwnd, &wr);
	
	data->DirectXData->sw->ResizeBuffers(1, wr.right - wr.left, wr.bottom - wr.top, DXGI_FORMAT_UNKNOWN, 0);

	data->DirectXData->rtv.Reset();
	//get 1 Buffer/Texture from the swapChain
	struct ID3D11Texture2D* TEX = __nullptr;
	DX11_ERROR
	(
		data->DirectXData->sw->GetBuffer(0U,
			__uuidof(struct ID3D11Texture2D),
			reinterpret_cast<void**>(&TEX)
		), L"Faild to get a buffer from the swap chain in Win32SwapChain class\n", return);

	//create render target view and give it the TEX variable, so we can draw to the Buffer/Texture
	DX11_ERROR 
	(
		data->DirectXData->dev->CreateRenderTargetView(
			TEX,
			__nullptr,
			&data->DirectXData->rtv
	), L"Faild to create render target view in Win32SwapChain class", return);

}
__declspec(noinline) static
void DX11_createSwapChain(struct DX11RenderingDevs* rs)
{
	DXGI_SWAP_CHAIN_DESC sw_desc = { 0 };
	sw_desc.BufferCount = 1U;// one extrax buffer so two will be used. 

	//Get client size of the window 
	RECT wr = { 0 };
	GetClientRect(rs->hwnd, &wr);
	//Adjust the Width of the window based on the Width of the client of the window 
	sw_desc.BufferDesc.Width = wr.right - wr.left;
	//Adjust the Height of the window based on the Height of the client of the window 
	sw_desc.BufferDesc.Height = wr.bottom - wr.top;
	//Set window to present frame buffers on
	sw_desc.OutputWindow = rs->hwnd;
	//Yes output should be in windowed mode. 
	sw_desc.Windowed = 1;//true
	//Allows us to change the size of the swapchain buffer and swicth from Windowed mode to another mode
	sw_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//How to format the color channels of each the pixel 
	sw_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Setting RefreshRate
	DXGI_RATIONAL RefreshRate = { 10U, 0U };
	sw_desc.BufferDesc.RefreshRate = RefreshRate;
	//How well to sample colors from a texture on to each pixel
	DXGI_SAMPLE_DESC SampleDesc = { 1U, 0U };
	sw_desc.SampleDesc = SampleDesc;
	//Yes the buffer will be used a Render target view to draw on them 
	sw_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//discard the contents of the back buffer after we call IDXGISwapChain::Present.
	sw_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//finally try to create swap Chain and do Error checking to make sure creation was ok
	DX11_ERROR(rs->factory->CreateSwapChain
	(
		rs->dev,
		&sw_desc,
		&rs->sw
	), L"Faild to Create SwapChain for DirectX 11 in Win32SwapChain class\n", return);

	//get 1 Buffer/Texture from the swapChain
	struct ID3D11Texture2D* TEX = __nullptr;
	DX11_ERROR
	(
		rs->sw->GetBuffer(0U,
			__uuidof(struct ID3D11Texture2D),
			reinterpret_cast<void**>(&TEX)
		), L"Faild to get a buffer from the swap chain in Win32SwapChain class\n", return);

	//create render target view and give it the TEX variable, so we can draw to the Buffer/Texture
	DX11_ERROR
	(
		rs->dev->CreateRenderTargetView(
			TEX,
			__nullptr,
			rs->rtv.GetAddressOf()
		), L"Faild to create render target view in Win32SwapChain class", return);


	WindowData* de = reinterpret_cast<WindowData*>(GetWindowLongPtr(rs->hwnd, GWLP_USERDATA));
	de->DirectXData = rs;
	de->resizeSwapChainBuffers =  DX11_resizeSwapChainBuffers;


	D3D11_VIEWPORT vp = { 0 };
	vp.Width = wr.right - wr.left;
	vp.Height = wr.bottom - wr.top;
	vp.MinDepth = 0b0;
	vp.MaxDepth = 0b1;
	rs->context->RSSetViewports(1, &vp);
	//Release ref to resource when done using it
	TEX->Release();
}
__declspec(noinline) static
void* DX11_createVertexShader(void* rs, const wchar_t* file_path, const char* entry_point, VE::Graphics::Resources::VInputLayout* inputLayout)
{
	//CREATING VERTEX SHADER
	//get full file path
	std::filesystem::path absolote_path = std::filesystem::absolute(file_path);
	//check if file exists
	bool exists = std::filesystem::exists(absolote_path);
	//if file doesn't exist throw error
	NULL_ERROR(exists, "File:" << absolote_path.string().c_str()<< "Doesn't Exists", return 0);
	//get renderring devices
	struct DX11RenderingDevs* s = reinterpret_cast<DX11RenderingDevs*>(rs);

	ID3DBlob* shaderCode = nullptr;//shader code
	ID3DBlob* errCode = nullptr;//holds warning if any
	
	//compile the shader code and check for errors
	DX11_ERROR(D3DCompileFromFile(absolote_path.c_str(), 0, 0, entry_point, "vs_5_0", 0, 0, &shaderCode, &errCode), "Failed to set succesfully vertex shader for use. Please Check your shader your \n\n" << absolote_path << "Shader", return 0);
	
	//check if shader compiled with warrnings
	NULL_ERROR(!errCode, L"HLSL SHADEER COMPILED WITH ERRORS\n\n" << reinterpret_cast<char*>(errCode->GetBufferPointer()) << L"in file:" << file_path, return 0);
	
	struct ID3D11VertexShader* outVertexShader = __nullptr;
	//use the byte code to create a DX11 vertex shader 
	DX11_ERROR(s->dev->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), 0L, &outVertexShader), "Failed to set succesfully vertex shader for use. Please Check your shader code", return 0);
	
	//SET UP INPUT LAYOUT 
	//DXGI_FORMAT_R32G32B32A32_FLOAT = 2U 
	//DXGI_FORMAT_R32G32B32_FLOAT = 6U 
	//DXGI_FORMAT_R32G32_FLOAT = 16U 		
	unsigned int AlignedByteOffset = 0U;
	//create DX11 InputLayout using non Api specific input Layout
	ID3D11InputLayout* input_layout = __nullptr;
	if (inputLayout->inputLayoutsElementCount != 0U)
	{
		struct D3D11_INPUT_ELEMENT_DESC* desc = reinterpret_cast<struct D3D11_INPUT_ELEMENT_DESC*>(malloc(sizeof(struct D3D11_INPUT_ELEMENT_DESC) * inputLayout->inputLayoutsElementCount));
		//init the memory to zero
		ZeroMemory(desc, sizeof(struct D3D11_INPUT_ELEMENT_DESC) * inputLayout->inputLayoutsElementCount);

		for (unsigned int i = 0U; i < inputLayout->inputLayoutsElementCount; i++)
		{
			desc[i].Format = static_cast<DXGI_FORMAT>(inputLayout->inputLayouts[i]);
			desc[i].AlignedByteOffset = AlignedByteOffset;
			desc[i].SemanticName = inputLayout->inputLayoutElementNames[i];
			if (inputLayout->inputLayouts[i] >= 2U && inputLayout->inputLayouts[i] <= 4U) 
			{
				//Size of (unsigned int, int, or float) = sizeof(int), amount of them used = 4
				AlignedByteOffset += static_cast<unsigned int>(sizeof(int) * 4ULL);
				continue;
			}
			else if (inputLayout->inputLayouts[i] >= 6U && inputLayout->inputLayouts[i] <= 8U)
			{
				//Size of (unsigned int, int, or float) = sizeof(int), amount of them used = 3
				AlignedByteOffset += static_cast<unsigned int>(sizeof(int) * 3ULL);
				continue;
			}
			else if (inputLayout->inputLayouts[i] >= 16U && inputLayout->inputLayouts[i] <= 18U)
			{
				//Size of (unsigned int, int, or float) = sizeof(int), amount of them used = 2
				AlignedByteOffset += static_cast<unsigned int>(sizeof(int) * 2ULL);
				continue;
			}
		}

		HRESULT hr = s->dev->CreateInputLayout(desc, inputLayout->inputLayoutsElementCount, shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), &input_layout);
		free(desc);
		DX11_ERROR(hr, "Failed to fully set vertex shader for use. Please Check your V_VertexShaderInfo struct's input layout and or your vertex shader code", return 0);
		
	}
	struct VertexShaderData
	{
		struct ID3D11VertexShader* VertexShader;
		struct ID3D11InputLayout* input_layout;
	} *vertexShaderData = new(VertexShaderData);
	ZeroMemory(vertexShaderData, sizeof(VertexShaderData));

	vertexShaderData->input_layout = input_layout;
	vertexShaderData->VertexShader = outVertexShader;

	return vertexShaderData;
}
__declspec(noinline) static
void* DX11_createFragmentShader(void* rs, const wchar_t* file_path, const char* entry_point)
{
	std::filesystem::path absolote_path = std::filesystem::absolute(file_path);
	//check if file exists
	bool exists = std::filesystem::exists(absolote_path);
	//if file doesn't exist throw error
	NULL_ERROR(exists, "File:" << absolote_path.string().c_str() << "Doesn't Exists", return 0);
	//get renderring devices
	struct DX11RenderingDevs* s = reinterpret_cast<DX11RenderingDevs*>(rs);

	ID3DBlob* shaderCode = nullptr;//shader code
	ID3DBlob* errCode = nullptr;//holds warning if any

	//compile the shader code and check for errors
	DX11_ERROR(D3DCompileFromFile(absolote_path.c_str(), 0, 0, entry_point, "ps_5_0", 0U, 0U, &shaderCode, &errCode), "Failed to set succesfully pixel shader for use. Check your shader your \n\n" << file_path << "Shader", return 0);
	
	//check of there are more warnings
	NULL_ERROR(!errCode, L"HLSL SHADEER COMPILED WITH ERRORS\n\n" << reinterpret_cast<char*>(errCode->GetBufferPointer()) << L"in file:" << file_path, return 0);


	struct ID3D11PixelShader* outPixelShader = __nullptr;
	//use the byte code to create a DX11 vertex shader 
	DX11_ERROR(s->dev->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), 0L, &outPixelShader), "Failed to set succesfully pixel shader for use. Please Check your shader code", return 0);


	return outPixelShader;
}

__declspec(noinline) static
void DX11_update(void* rs, float r, float g, float b, float a)
{
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	//Get client size of the window 
	RECT wr = { 0 };
	
	//present to make make the render target seeable to human eyes
	s->sw->Present(1U, 0U);

	//clear the color of the wholl render target to the color reprsented as [r] [g] [b] [a]
	float color[4U] = { r,g,b,a };
	s->context->ClearRenderTargetView(s->rtv.Get(), color);
	s->context->OMSetRenderTargets(1U, &s->rtv, 0);

	GetClientRect(s->hwnd, &wr);
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = wr.right - wr.left;
	vp.Height = wr.bottom - wr.top;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	s->context->RSSetViewports(1, &vp);


}
__declspec(noinline) static
void DX11_setVertexShader(void* rs, void* vsCode)
{
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	struct VertexShaderData
	{
		struct ID3D11VertexShader* VertexShader;
		struct ID3D11InputLayout* input_layout;
	} * vertexShaderData = reinterpret_cast<struct VertexShaderData*>(vsCode);
	
	//Set the Vertex Shader to be used in the input assembly stage of rendering 
	s->context->VSSetShader(vertexShaderData->VertexShader, __nullptr, 0U);

		//release allocated memory

	if (vertexShaderData->input_layout) {
		s->context->IASetInputLayout(vertexShaderData->input_layout);
	}
}
__declspec(noinline) static
void DX11_setFragmentShader(void* rs, void* fs)
{
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	struct ID3D11PixelShader* outPixelShader = reinterpret_cast<struct ID3D11PixelShader*>(fs);
	//Set the Pixel Shader to be used in the input assembly stage of rendering 
	s->context->PSSetShader(outPixelShader, __nullptr, 0U);
}
__declspec(noinline) static
void DX11_drawMesh(void* rs, VE::Graphics::Resources::VMesh mesh)
{
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	//CREATE VERTEX BUFFER
	{
		D3D11_BUFFER_DESC desc = { 0 };
		//Size of the data 
		desc.ByteWidth = mesh.vertexCount * mesh.vertexSize;
		//the resource will be read and wirten to by the GPU
		desc.Usage = D3D11_USAGE_DEFAULT;
		//Bind to Input l Assembly as a Vertex buffer
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//0 for no CPU access is necessary
		desc.CPUAccessFlags = 0U;
		D3D11_SUBRESOURCE_DATA srd =
		{
			mesh.verties,
			0U,
			0U
		};
		ID3D11Buffer* vertexBuffer = __nullptr;
		DX11_ERROR(s->dev->CreateBuffer(&desc, &srd, &vertexBuffer), "Failed to set succesfully vertex shader for use. Check your V_VertexShaderInfo struct's Vertex Mesh data", return);
		unsigned int stride = mesh.vertexSize;
		unsigned int offset = 0U;
		s->context->IASetVertexBuffers(0U, 1U, &vertexBuffer, &stride, &offset);
	};
	//CREATE CONSTANT BUFFER
	if (mesh.cbSize != 0U)
	{
		D3D11_BUFFER_DESC desc = { 0 };
		//Size of the data 
		desc.ByteWidth = mesh.cbSize;
		//the resource will be read and wirten to by the GPU
		desc.Usage = D3D11_USAGE_DEFAULT;
		//Bind to Input l Assembly as a Vertex buffer
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//0 for no CPU access is necessary
		desc.CPUAccessFlags = 0U;
		D3D11_SUBRESOURCE_DATA srd =
		{
			mesh.cb,
			0U,
			0U
		};
		ID3D11Buffer* constantBuffer = __nullptr;
		DX11_ERROR(s->dev->CreateBuffer(&desc, &srd, &constantBuffer), "Failed to set succesfully index buffer for vertex shader. Check your V_VertexShaderInfo struct's cb and csSize data", return);
		s->context->VSSetConstantBuffers(0U, 1U, &constantBuffer);	
	};
	//CREATE INDEX BUFFER
	if (!mesh.indices.empty())
	{
		unsigned int indexSize = sizeof(unsigned int);
		unsigned int indiceCount = mesh.indices.size();

		D3D11_BUFFER_DESC desc = { 0 };
		//Size of the data 
		desc.ByteWidth = indiceCount * indexSize;
		//the resource will be read and wirten to by the GPU
		desc.Usage = D3D11_USAGE_DEFAULT;
		//Bind to Input l Assembly as a Vertex buffer
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//0 for no CPU access is necessary
		desc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA srd =
		{
			mesh.indices.data(),
			0U,
			0U
		};
		ID3D11Buffer* indexBuffer = __nullptr;
		DX11_ERROR(s->dev->CreateBuffer(&desc, &srd, &indexBuffer), 
			"Failed to set succesfully constant buffer for vertex shader. Check your V_VertexShaderInfo struct's cb and csSize data", return);
		
		s->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, indexSize);

		s->context->DrawIndexed(indiceCount, 0U, 0U);
		
	}
	else
	{
		s->context->Draw(mesh.vertexCount, 0U);
	}
}

void DX11_SetPrimitiveTopology(void* rs, VE::Graphics::Resources::V_Primitive_Topology topology)
{	
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	D3D11_PRIMITIVE_TOPOLOGY DX11_topology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology);
	s->context->IASetPrimitiveTopology(DX11_topology);
}

void DX11_uninit(void* rs, void* graphics)
{
	IGraphicsEngine* ge_b = reinterpret_cast<IGraphicsEngine*>(graphics);
	struct DX11RenderingDevs* s = reinterpret_cast<struct DX11RenderingDevs*>(rs);
	memset(ge_b, 0, sizeof(IGraphicsEngine));
	if(s->context)s->context->Release();
	if(s->dev)s->dev->Release();
	if(s->factory)s->factory->Release();
	if(s->idxgi_dev)s->idxgi_dev->Release();
	if (s->rtv)s->rtv->Release();
	if (s->sw)s->sw->Release();
	free(s);
	delete ge_b;
}
