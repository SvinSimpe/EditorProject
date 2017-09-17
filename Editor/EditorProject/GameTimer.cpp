#include "GameTimer.h"

const float GameTimer::ElapsedGameTime() const noexcept
{
	if( mIsStopped )
	{ 
		return static_cast<float>( ( (
			( mStopTime - mPausedTime ) -
			mBaseTime ) * mSecondsPerCount ) );
	}
	else
	{
		return static_cast<float>( ( (
			( mCurrentTimeStamp - mPausedTime ) -
			mBaseTime ) * mSecondsPerCount ) );
	}
}

const float GameTimer::DeltaTime() const noexcept
{
	return static_cast<float>(mDeltaTime);
}

const double GameTimer::DeltaTimeRaw() const noexcept
{
	return mDeltaTime;
}

void GameTimer::Reset()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&mCurrentTimeStamp );

	mBaseTime = mCurrentTimeStamp;
	mPreviousTimeStamp = mCurrentTimeStamp;
	mStopTime = 0;
	mIsStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter( (LARGE_INTEGER*)&startTime );

	if( mIsStopped )
	{
		mPausedTime += ( startTime - mStopTime );
		mPreviousTimeStamp = startTime;
		mStopTime = 0;
		mIsStopped = false;
	}
}

void GameTimer::Stop()
{
	if( !mIsStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );

		mStopTime = currTime;
		mIsStopped = true;
	}
}

void GameTimer::Tick()
{
	if( mIsStopped )
	{
		mDeltaTime = 0.0f;
		return;
	}

	// Get the time this frame
	QueryPerformanceCounter( (LARGE_INTEGER*)&mCurrentTimeStamp );

	// Time differens between this frame and the previous
	mDeltaTime = static_cast<double>( mCurrentTimeStamp - mPreviousTimeStamp ) * mSecondsPerCount;

	mPreviousTimeStamp = mCurrentTimeStamp;

	// Force non-negative
	if( mDeltaTime < 0.0f )
		mDeltaTime = 0.0f;
}

GameTimer::GameTimer()
{
	mSecondsPerCount = 0.0;
	mDeltaTime = 0.0;

	mBaseTime = 0;
	mPausedTime = 0;
	mStopTime = 0;
	mPreviousTimeStamp = 0;
	mCurrentTimeStamp = 0;

	mIsStopped = true;
}

GameTimer::~GameTimer()
{}
