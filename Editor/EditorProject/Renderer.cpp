#include "Renderer.h"
#include "Resolution.h"
#include <DirectXColors.h>
#include <fstream>

void Renderer::Update( float deltaTime )
{}

void Renderer::Render()
{
	BeginFrame();

	EndFrame();
}

Renderer::Renderer( HWND windowHandle ) // Use init list
{
	mWindowHandle = windowHandle;
	mIsMinimized = false;
	mIsMaximized = false;
	mIsResizing = false;

	if( FAILED( CreateDeviceAndSwapChain() ) )
		OutputDebugStringA( "\nerror: Unable to create ID3D11Device\n" );

	if( FAILED( CreateRenderTargetView() ) )
		OutputDebugStringA( "\nerror: Unable to create ID3D11RenderTargetView\n" );
	
	if( FAILED( CreateDepthStencilView() ) )
		OutputDebugStringA( "\nerror: Unable to create ID3D11DepthStencilView\n" );
	
	if( FAILED( CreateRasterizerState() ) )
		OutputDebugStringA( "\nerror: Unable to create ID3D11RasterizerState\n" );

	// Add shader creation

}

Renderer::~Renderer()
{}

void Renderer::BeginFrame()
{
	SetViewport();
	mDeviceContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	mDeviceContext->ClearRenderTargetView( mRenderTargetView.Get(), DirectX::Colors::DarkSeaGreen );
	mDeviceContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );
}

void Renderer::EndFrame()
{
	mSwapChain->Present( 1, 0 );
	mDeviceContext->ClearState();
}

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
	HRESULT hr = S_OK;

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

void Renderer::SetViewport()
{
	RECT rc;
	GetClientRect( mWindowHandle, &rc );

	int width	= ( rc.right - rc.left );
	int height	= ( rc.bottom - rc.top );

	D3D11_VIEWPORT vp;
	vp.Width	= Resolution::WIDTH;
	vp.Height	= Resolution::HEIGHT;
	vp.MinDepth	= 0.0f;
	vp.MaxDepth	= 1.0f;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;

	mDeviceContext->RSSetViewports( 1, &vp );
	mDeviceContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );
}

HRESULT Renderer::CreateShaders()
{
	ComPtr<ID3DBlob> vs = nullptr;
	if ( CompileShader( "Shader.hlsl", "VertexShaderMain", "vs_5_0", nullptr, vs.GetAddressOf() ) )
	{
		HRESULT hr = S_OK;
		if( SUCCEEDED( hr = mDevice->CreateVertexShader( vs->GetBufferPointer(),
														 vs->GetBufferSize(),
														 nullptr,
														 effect.vertexShader.GetAddressOf() ) ) )
		{	
			if( effect.inputType == EInputType::Instanced )
			{ 
				hr = mDevice->CreateInputLayout( InputDesc::Instanced,
												 ARRAYSIZE( InputDesc::Instanced ),
												 vs->GetBufferPointer(),
												 vs->GetBufferSize(),
												 effect.inputLayout.GetAddressOf() );
			}
			else if( effect.inputType == EInputType::Debug )
			{ 
				hr = mDevice->CreateInputLayout( InputDesc::Debug,
												 ARRAYSIZE( InputDesc::Debug ),
												 vs->GetBufferPointer(),
												 vs->GetBufferSize(),
												 effect.inputLayout.GetAddressOf() );

			}
		}

		ComPtr<ID3DBlob> ps = nullptr;

		if( CompileShader( effect.shaderFile, "PixelShaderMain", "ps_5_0", nullptr, ps.GetAddressOf() ) )
		{
			hr = mDevice->CreatePixelShader( ps->GetBufferPointer(),
											 ps->GetBufferSize(),
											 nullptr,
											 effect.pixelShader.GetAddressOf() );

		}	
		else if( hr == E_FAIL )
			return false;
	}

	return true;

	return S_OK;
}

HRESULT Renderer::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget,
								 D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	// Full optimization level
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;

	#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | 
						D3DCOMPILE_IEEE_STRICTNESS |
						D3DCOMPILE_DEBUG | 
						D3DCOMPILE_WARNINGS_ARE_ERRORS;
	#endif


	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );

	if( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( static_cast<unsigned int>( in.tellg() ) );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3DCompile( shader_code.data(),
							 shader_code.size(),
							 NULL,
							 pDefines,
							 nullptr,
							 pEntrypoint,
							 pTarget,
							 dwShaderFlags,
							 NULL,
							 pCompiledShader,
							 &errorBlob );

	if( errorBlob )
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );

	return hr;
}
