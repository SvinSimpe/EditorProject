#pragma once
#include "stdafx.h"
#include <wrl.h>
#include "IGameObject.h"
//#include <windef.h>

using namespace Microsoft::WRL;
class Renderer : public IGameObject
{
	public:
		// Inherited via IGameObject
		virtual void Update( float deltaTime ) override;
		virtual void Render() override;


		Renderer( HWND windowHandle );
		~Renderer();

	private:
		void BeginFrame();
		void EndFrame();

		HRESULT CreateDeviceAndSwapChain();
		HRESULT CreateRenderTargetView();
		HRESULT CreateDepthStencilView();
		HRESULT CreateRasterizerState();
		void SetViewport();

		HRESULT CreateShaders();
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget,
							   D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
	
	private:
		HWND mWindowHandle;
		bool mIsMinimized;
		bool mIsMaximized;
		bool mIsResizing;

		ComPtr<ID3D11Device> mDevice;
		ComPtr<ID3D11DeviceContext> mDeviceContext;
		ComPtr<IDXGISwapChain> mSwapChain;
		ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		ComPtr<ID3D11RasterizerState> mRasterizerState;
		D3D11_VIEWPORT mViewPort;

		// Shader
		ComPtr<ID3D11VertexShader> mVertexShader;
		ComPtr<ID3D11PixelShader> mPixelShader;
		ComPtr<ID3D11InputLayout> mInputLayout;

		// Buffer
		ComPtr<ID3D11Buffer> mPerFrameCBuffer;
		ComPtr<ID3D11Buffer> mPerObjectCBuffer;
		ComPtr<ID3D11Buffer> mVertexBuffer;

};