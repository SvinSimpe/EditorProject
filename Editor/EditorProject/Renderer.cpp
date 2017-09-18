#include "Renderer.h"
#include "Resolution.h"

void Renderer::Update( float deltaTime )
{}

void Renderer::Render()
{}

Renderer::Renderer( HWND windowHandle )
{
	mWindowHandle = windowHandle;
	mIsMinimized = false;
	mIsMaximized = false;
	mIsResizing = false;

	if( !CreateDeviceAndSwapChain() )
		OutputDebugStringA( "\nerror: Unable to create ID3D11Device\n" );

	if( !CreateRenderTargetView() )
		OutputDebugStringA( "\nerror: Unable to create ID3D11RenderTargetView\n" );
	
	if( !CreateDepthStencilView() )
		OutputDebugStringA( "\nerror: Unable to create ID3D11DepthStencilView\n" );
	
	if( !CreateRasterizerState() )
		OutputDebugStringA( "\nerror: Unable to create ID3D11RasterizerState\n" );
}

Renderer::~Renderer()
{}

HRESULT Renderer::CreateDeviceAndSwapChain()
{
	RECT rc;
	GetWindowRect( mWindowHandle, &rc );

	int border = 12;
	int width	= rc.right - rc.left - border;
	int height	= rc.bottom - rc.top - border;

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC sd;
	memset( &sd, 0, sizeof( sd ) );
	sd.BufferCount				= 1;
	sd.BufferDesc.Width						= Resolution::WIDTH;
	sd.BufferDesc.Height					= Resolution::HEIGHT;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= mWindowHandle;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.Windowed								= TRUE;
	sd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;


	
	D3D11_CREATE_DEVICE_FLAG deviceFlag = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#if defined(DEBUG) || defined(_DEBUG)
		deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
	#endif

	HRESULT hr = E_FAIL;
	for ( UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr ); driverTypeIndex++ )
	{
		hr = D3D11CreateDeviceAndSwapChain( nullptr,
											driverTypes[driverTypeIndex],
											NULL,
											deviceFlag,
											featureLevelsToTry,
											ARRAYSIZE( featureLevelsToTry ),
											D3D11_SDK_VERSION,
											&sd,
											&mSwapChain,
											&mDevice,
											&initiatedFeatureLevel,
											&mDeviceContext );
	}

	return hr;
}

HRESULT Renderer::CreateRenderTargetView()
{
	HRESULT hr = E_FAIL;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	if ( SUCCEEDED( mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf() );

	}

	return hr;
}

HRESULT Renderer::CreateDepthStencilView()
{
	HRESULT hr = E_FAIL;

	ComPtr<ID3D11Texture2D> depthStencil = nullptr;

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width				= Resolution::WIDTH;
	dsd.Height				= Resolution::HEIGHT;
	dsd.MipLevels			= 1;
	dsd.ArraySize			= 1;
	dsd.Format				= DXGI_FORMAT_D32_FLOAT;
	dsd.SampleDesc.Count	= 1;
	dsd.SampleDesc.Quality	= 0;
	dsd.Usage				= D3D11_USAGE_DEFAULT;
	dsd.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags		= 0;
	dsd.MiscFlags			= 0;

	if( FAILED( mDevice->CreateTexture2D( &dsd, nullptr, depthStencil.GetAddressOf() ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( mDevice->CreateDepthStencilView( depthStencil.Get(), &dsvd, mDepthStencilView.GetAddressOf() ) ) )
		return hr;

	return hr;
}

HRESULT Renderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.FillMode			= D3D11_FILL_SOLID;
	desc.CullMode			= D3D11_CULL_BACK;
	desc.DepthClipEnable	= true;

	return mDevice->CreateRasterizerState( &desc, mRasterizerState.GetAddressOf() );
}

