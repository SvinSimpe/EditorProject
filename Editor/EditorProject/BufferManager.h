#pragma once

#include "stdafx.h"
#include <wrl.h>
#include "IGameObject.h"

using namespace Microsoft::WRL;
class BufferManager : public IGameObject
{
	public:
		// Inherited via IGameObject
		virtual void Update( float deltaTime ) override;
		virtual void Render() override;

		BufferManager();
		~BufferManager();

	private:
		HRESULT UpdateFrameBuffer();
		HRESULT UpdateObjectBuffer();
		HRESULT UpdateVertexBuffer();

		HRESULT CreateFrameBuffer();
		HRESULT CreateObjectBuffer();
		HRESULT CreateVertexBuffer();

	private:
		ComPtr<ID3D11Buffer> mVertexBuffer;
		ComPtr<ID3D11Buffer> mFrameBuffer;
		ComPtr<ID3D11Buffer> mObjectBuffer;
};