﻿#pragma once
#include "stdafx.h"
#include <wrl.h>
#include "IGameObject.h"
#include <windef.h>

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
		HRESULT CreateDevice();
		HRESULT CreateSwapChain();
		HRESULT CreateRTV();
	
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
		ComPtr<D3D11_VIEWPORT> mViewPort;

};