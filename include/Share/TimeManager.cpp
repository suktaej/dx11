#include "TimeManager.h"

LARGE_INTEGER CTimeManager::mSecond = {};
LARGE_INTEGER CTimeManager::mTime = {};
float CTimeManager::mDeltaTime = 0.f;
float CTimeManager::mFPS = 0.f;
float CTimeManager::mFPSTime = 0.f;
int CTimeManager::mFPSTick = 0;

void CTimeManager::init()
{
    QueryPerformanceFrequency(&mSecond);
    QueryPerformanceCounter(&mTime);
}

float CTimeManager::update()
{
	LARGE_INTEGER time;

	QueryPerformanceCounter(&time);

	mDeltaTime = (time.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;
	mTime = time;

	mFPSTime += mDeltaTime;
	++mFPSTick;

	if (mFPSTick == 60)
	{
		mFPS = mFPSTick / mFPSTime;
		mFPSTick = 0;
		mFPSTime = 0.f;
	}

	//char FPSText[64] = {};

	//sprintf_s(FPSText, "FPS : %.5f", mFPS);
	//CLog::PrintLog(FPSText);

	return mDeltaTime;
}

float CTimeManager::getDeltaTime()
{
    return mDeltaTime;
}
