/*
	Filename: d3dclass.h
*/

#include"d3dclass.h"

D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthstencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{

}

D3DClass::~D3DClass()
{

}

bool D3DClass::initalize(int screenWidth, int ScreenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// vsync(수직동기화) 설정을 저장합니다.
	m_vsync_enabled = vsync;

	// DX 그래픽 인터페이스 팩토리를 만듭니다.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// 팩토리 객체를 사용해 그래픽 카드 인터페이스에 대한 아답터를 만듭니다.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(factory))
	{
		return false;
	}

	// 출력에 대한 첫번째 아답터를 나열합니다.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포멧에 맞는 모드의 개수를 구합니다.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성합니다.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 디스플레이 모드에 대한 리스트 구조를 채워넣습니다.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// 적합한 디스플레이 모드를 찾아 설정합니다.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)ScreenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// 아답터(그래픽카드)의 description을 가져옵니다.
	result = adapter->GetDesc((&adapterDesc));
	if (FAILED(result))
	{
		return false;
	}

	// 현재 그래픽카드의 메모리 용량을 메가바이트 단위로 저장합니다.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
	// 그래픽카드의 이름을 char 문자열 배열로 바꾼 뒤 저장합니다.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}
	// ==========================================================
	// 이 시점에서 우리는 저장된 새로고침 배율의 정보를 갖고있기 때문에 사용된 구조체들과
	// 인터페이스들을 반납해야 합니다.
	// ==========================================================

	// 디스플레이 모드 리스트의 할당을 해제합니다.
	delete[] displayModeList;
	displayModeList = 0;

	// 출력 아답터를 할당 해제합니다.
	adapterOutput->Release();
	adapterOutput = 0;

	// 팩토리 객체를 할당 해제합니다.
	factory->Release();
	factory = 0;

	// ==========================================================
	// 지금 시스템에서 얻어온 새로고침 비율을 갖고 있으므로 DX의 초기화를 시작할 수 있습니다.
	// 가장 먼저 해야 할 일은 스왑 체인의 description 구조체를 채워넣는 일입니다.
	// 스왑 체인은 프론트버퍼와 백버퍼로 이루어져 있는 우리가 실제로 렌더링을 할 때 쓰이는 곳입니다.
	// 보통 하나의 백버퍼를 사용해 프론트 버퍼와 바꿔치기합니다.
	// ==========================================================

	// 스왑 체인 description을 초기화합니다.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 하나의 백버퍼만을 사용하도록 합니다.
	swapChainDesc.BufferCount = 1;

	// 백버퍼의 너비와 높이를 설정합니다.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;

	// 백버퍼로 인반적인 32bit의 서페이스를 지정합니다.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ==========================================================
	// 스왑 체인 description 구조체 채우기의 다음 부분은 새로고침 비율입니다.
	// 새로고침 비율은 초당 몇장의 백버퍼를 그려 프론트 버퍼와 바꿔치기 하는지를 나타냅니다.
	// 만약 우리 graphicsclass.h의 vsync 변수가 true로 설정되있다면 시스템에서 정한 새로고침 비율로 고정됩니다.
	// 이것은 프로그램은 초당 60번의 화면을 그릴것이라는 의미입니다.
	// 반면에 vsync 변수가 false라면 프로그램은 최대한 화면을 그려내려고 할것이며 잔상을 남기게 됩니다.
	// ==========================================================

	// 백버퍼의 새로고침 비율을 설정합니다.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// 백버퍼의 용도를 설정합니다.
	swapChainDesc.OutputWindow = hwnd;

	// 멀티샘플링을 끕니다.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 윈도우 모드 또는 풀스크린 모드를 설정합니다.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로 설정합니다.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력된 이후의 백버퍼의 내용을 버리도록 합니다.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 옵션 플래그를 사용하지 않습니다.
	swapChainDesc.Flags = 0;

	// 피쳐 레밸을 DX 11로 설정합니다.
	// 피쳐 레밸은 우리가 어느 버전의 DX를 사용할 것인지 알려줍니다.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인, D3D 디바이스, D3D 디바이스 컨텍스트를 생성합니다.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(result))
	{
		return false;
	}

	// ==========================================================
	//이제 스왑 체인이 생겼으므로 우리는 백버퍼를 만들고 그 포인터를 스왑 체인에 
	// 연결시켜 주어야 합니다. 이를 위해 CreateRenderTargetView 함수를 사용해
	// 백버퍼를 스왑 체인에 연결합니다.
	// ==========================================================

	// 백버퍼의 포인터를 가져옵니다.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// 백버퍼의 포인터로 렌더타겟 뷰를 생성합니다.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// 백버퍼 포인터를 더이상 사용하지 않으므로 할당 해제합니다.
	backBufferPtr->Release();
	backBufferPtr = 0;
	// ==========================================================
	// 이에 더해 깊이 버퍼의 description 구조체도 작성해야 합니다.
	// 이를 이요하여 깊이버퍼를 만들어야 3D공간에서 우리의 폴리곤들이 올바르게 그려집니다.
	// 또한 동시에 스텐실 버퍼도 이 깊이 버퍼에 연결할 것입니다.
	// 스탠실 버퍼는 모션 블러라던가, 볼륨있는 그림자등의 효과를 나타낼 때 사용합니다.
	// ==========================================================

	// 깊이 버퍼의 description을 초기화합니다.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼의 description을 작성합니다.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// description을 사용하여 깊이 버퍼의 택스쳐를 생성합니다.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthstencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 필요한것은 깊이-스텐실 description을 작성하는 것입니다.
	// 이것은 우리가 D3D에서 각 픽셀에 어떤 깊이 테스트를 할 것인지 정할 수 있게 해 줍니다.
	
	// 스텐실 상태의 dscription을 초기화합니다.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태의 description을 작성합니다.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 위에서 작성한 description을 토대로 깊이-스텐실 상태 변수를 만듭니다.
	result - m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// 깊이-스텐실 상태를 설정합니다.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// 다음으로 할 일은 깊이-스텐실 버퍼의 뷰에 대한 description을 만드는 것입니다.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 깊이-스텐실 뷰의 description을 작성합니다.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성합니다.
	result = m_device->CreateDepthStencilView(m_depthstencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}
	// ==========================================================
	// 여기까지 왔다면 이제 OMsetRenderTarget 함수를 호출할 수 있습니다.
	// 이 함수는 렌더타겟뷰와 깊이-스텐실 뷰를 출력 렌더링 파이프라인에 바인딩시킵니다.
	// 이렇게 하여 파이프라인을 이용한 렌더링이 수행될 때 우리가 만들었던 백버퍼에 그려지게 됩니다.
	// 그리고 백버퍼에 그려진 것을 프론트 버퍼와 바꿔치기 하여 유저의 모니터에 출력하는 것입니다.
	// ==========================================================

	// 렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩합니다. 
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기 description
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// 작성한 description으로부터 래스터화기 상태를 생성합니다.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// 래스터화기 상태를 설정합니다.
	m_deviceContext->RSSetState(m_rasterState);

	// 렌더링을 위한 뷰포트를 설정합니다.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 뷰포트를 생성합니다.
	m_deviceContext->RSSetViewports(1, &viewport);

	// 이제 투영행렬을 생성할 차례입니다. 투영 행렬은 3D의 화면을 앞서 만들었던 2D뷰포트 공간으로
	// 변환하도록 해 주는 것입니다. 이 행렬의 복사본을 만들어서 셰이더에서 사용할 수 있도록
	// 해야 합니다.

	// 투영 행렬을 설정합니다. 
	fieldOfView = (float)D3DX_PI / 4.0f; 
	screenAspect = (float)screenWidth / (float)screenHeight;
	
	// 3D 렌더링을 위한 투영 행렬을 생성합니다. 
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// 월드 행렬을 단위 행렬로 초기화합니다.
	D3DXMatrixIdentity(&m_worldMatrix);

	// 2D 렌더링에 사용될 정사영 행렬을 생성합니다.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth); 
	
	return true;
}

void D3DClass::Shutdown()
{
	// 종료하기 전에 이렇게 윈도우 모드로 바꾸지 않으면 스왑체인을 할당 해제할 때
	// 예외가 발생합니다.

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthstencilBuffer)
	{
		m_depthstencilBuffer->Release();
		m_depthstencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// 버퍼를 어떤 색상으로 지울 것인지 설정합니다.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// 백버퍼의 내용을 지웁니다.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// 깊이 버퍼를 지웁니다.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// 랜더링이 완료되었으므로 백버퍼의 내용을 화면에 표시합니다.
	if (m_vsync_enabled)
	{
		// 새로고침 비율을 고정합니다.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// 가능한 빠르게 표시합니다.
		m_swapChain->Present(0, 0);
	}

	return;
}

// 다음 함수들은 단순히 D3D 디바이스와 디바이스 컨텍스트의 포인터를 가져오는 것입니다.
// 이 도우미 함수들은 프레임워크에서 자주 호출될 것입니다.
ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

// 다음 세 도우미 함수는 투영, 월드, 그리고 정사영 행렬을 반환합니다. 
// 대부분의 셰이더에서는 이 행렬들이 필요하기 때문에 이를 가져올 손쉬울 방법이 필요합니다.

void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = worldMatrix;
	return;
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = orthoMatrix;
	return;
}

// 마지막 도우미 함수는 그래픽카드의 이름과 사용가능한 메모리의 양을 반환합니다.
// 그래픽 카드의 이름과 메모리의 크기를 아는 것은 서로 다른 설정에서 디버깅하는데
// 도움이 될 수 있습니다.

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

// Initalize 함수에서는 DX11의 전체 Direct3D 설정이 일어난느 곳입니다.
// 인자로 넘어온 screenWidth와 screenHeight 변수는 SystemClass에서 만든 윈도우의 너비와 높이입니다.
// Direct3D에서는 이를 이용하여 동일한 크기의 영역을 초기화하고 이용합니다.
// hWnd는 만들어진 윈도우에 대한 핸들이고, Direct3D는 이 변수를 이용해 윈도우에 접근합니다.
// fullscreen 변수는 이 어플리케이션이 윈도우 모드인지 풀스크린 모드인지에 대한 정보를 가지고 있습니다.
// screenDepth 변수와 screenNear 변수는 윈도우에 그려질 3D환경에서의 깊이 값입니다.
// vsync 변수는 Direct3D 렌더링을 모니터의 새로고침 비율에 맞출 것인지 아니면 다시 그릴지를 지정합니다.