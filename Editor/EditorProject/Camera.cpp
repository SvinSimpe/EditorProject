#include "Camera.h"
#include "Resolution.h"

using namespace DirectX;

void Camera::Update( float deltaTime )
{
	XMStoreFloat3( &mRightVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
												  XMVectorSet( 0.0f, -1.0f, 0.0f, 0.0f ) ) );
	XMStoreFloat3( &mUpVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
											   XMLoadFloat3( &mRightVector ) ) );

	XMVECTOR E = XMLoadFloat3( &mLocation );
	XMVECTOR R = XMLoadFloat3( &mRightVector );
	XMVECTOR U = XMLoadFloat3( &mUpVector );
	XMVECTOR L = XMLoadFloat3( &mLookVector );

	// Compute look vector
	L = XMLoadFloat3( &mFocusPoint ) - XMLoadFloat3( &mLocation );
	
	// Normalize look vector
	L = XMVector3Normalize( L );

	// Compute new corrected Up Vector
	U = XMVector3Normalize( XMVector3Cross( L, R ) );

	// Compute new corrected Right Vector
	R = XMVector3Cross( U, L );


	// Fill View Matrix entries
	float x = -XMVectorGetX( XMVector3Dot( E, R ) );
	float y = -XMVectorGetX( XMVector3Dot( E, U ) );
	float z = -XMVectorGetX( XMVector3Dot( E, L ) );

	XMStoreFloat3( &mRightVector, R );
	XMStoreFloat3( &mUpVector, U );
	XMStoreFloat3( &mLookVector, L );

	mViewMatrix( 0, 0 ) = mRightVector.x;
	mViewMatrix( 1, 0 ) = mRightVector.y;
	mViewMatrix( 2, 0 ) = mRightVector.z;
	mViewMatrix( 3, 0 ) = x;

	mViewMatrix( 0, 1 ) = mUpVector.x;
	mViewMatrix( 1, 1 ) = mUpVector.y;
	mViewMatrix( 2, 1 ) = mUpVector.z;
	mViewMatrix( 3, 1 ) = y;

	mViewMatrix( 0, 2 ) = mLookVector.x;
	mViewMatrix( 1, 2 ) = mLookVector.y;
	mViewMatrix( 2, 2 ) = mLookVector.z;
	mViewMatrix( 3, 2 ) = z;

	mViewMatrix( 0, 3 ) = 0.0f;
	mViewMatrix( 1, 3 ) = 0.0f;
	mViewMatrix( 2, 3 ) = 0.0f;
	mViewMatrix( 3, 3 ) = 1.0f;
}

void Camera::SetFocusPoint( const XMFLOAT3& newFocusPoint ) noexcept
{
	mFocusPoint = newFocusPoint;
}

void Camera::Translate( float deltaTime, DirectX::XMFLOAT3 direction )
{
	XMMATRIX translation = XMMatrixTranslationFromVector( XMLoadFloat3( &direction ) * deltaTime );
	XMStoreFloat3( &mLocation, XMVector3Transform( XMLoadFloat3( &mLocation ), translation ) );
}

Camera::Camera() 
	: mLocation(XMFLOAT3()), mFocusPoint(XMFLOAT3()), mLookVector(XMFLOAT3()),
	mUpVector(XMFLOAT3()), mRightVector(XMFLOAT3()), 
	mViewMatrix(XMFLOAT4X4()), mProjectionMatrix(XMFLOAT4X4()),
	mMinCullDistance(0.0f), mMaxCullDistance(0.0f), mFieldOfView(0.0f)
{}

Camera::Camera( const XMFLOAT3 cameraLocation, const XMFLOAT3 focusPoint,
							const float minDrawDistance, const float maxDrawDistance, const float fieldOfView )
	: mLocation(cameraLocation),
	mFocusPoint(focusPoint)
{
	XMVECTOR look = XMLoadFloat3( &mFocusPoint ) - XMLoadFloat3( &mLocation );
	XMStoreFloat3( &mLookVector, XMVector3Normalize( look ) );

	XMStoreFloat3( &mRightVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
												  XMVectorSet( 0.0f, -1.0f, 0.0f, 0.0f ) ) );

	XMStoreFloat3( &mUpVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
											   XMLoadFloat3( &mRightVector ) ) );

	mMinCullDistance	= minDrawDistance;
	mMaxCullDistance	= maxDrawDistance;
	mFieldOfView		= fieldOfView;

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( XMLoadFloat3( &mLocation ), 
													 XMLoadFloat3( &mFocusPoint ),
													 XMLoadFloat3( &mUpVector ) ) );

	XMStoreFloat4x4( &mProjectionMatrix, XMMatrixPerspectiveFovLH( mFieldOfView, 
																   static_cast<float>(Resolution::WIDTH) / 
																   static_cast<float>(Resolution::HEIGHT),
																   mMinCullDistance, mMaxCullDistance ) );
}

Camera::~Camera()
{}

