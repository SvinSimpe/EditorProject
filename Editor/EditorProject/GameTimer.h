#pragma once
#include <Windows.h>

class GameTimer
{
	public:
		/* Returns the total elpsed game time in seconds since the timer was started */
		const float ElapsedGameTime() const noexcept;
		const float DeltaTime() const noexcept;
		const double DeltaTimeRaw() const noexcept;

		void Reset();
		void Start();
		void Stop();
		void Tick();

		GameTimer();
		~GameTimer();

	private:
		double mSecondsPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPreviousTimeStamp;
		__int64 mCurrentTimeStamp;

		bool mIsStopped : 1;
};