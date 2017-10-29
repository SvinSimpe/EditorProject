#pragma once

#include <DirectXMath.h>

class Camera
{
	public:
		// Rebuilds view matrix
		void Update( float deltaTime );
		void SetFocusPoint( const DirectX::XMFLOAT3& newFocusPoint = DirectX::XMFLOAT3() ) noexcept;
		void Translate( float deltaTime, DirectX::XMFLOAT3 direction );
		inline DirectX::XMFLOAT4X4 GetViewMatrix() const noexcept { return mViewMatrix; }
		inline DirectX::XMFLOAT4X4 GetProjectionMatrix() const noexcept { return mProjectionMatrix; }

		Camera();
		Camera( const DirectX::XMFLOAT3 cameraLocation = DirectX::XMFLOAT3( 10.0f, 10.0f, -10.0f ),
				const DirectX::XMFLOAT3 focusPoint = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ), 
				const float minCullDistance = 0.5f,
				const float maxCullDistance = 5000.0f,
				const float fieldOfView = 0.75f );
		~Camera();


	private:
		DirectX::XMFLOAT3 mLocation;
		DirectX::XMFLOAT3 mFocusPoint;
		DirectX::XMFLOAT3 mLookVector;
		DirectX::XMFLOAT3 mUpVector;
		DirectX::XMFLOAT3 mRightVector;

		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjectionMatrix;

		float	mMinCullDistance;
		float	mMaxCullDistance;
		float	mFieldOfView;
};