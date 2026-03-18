#pragma once
#include "../GameInfo.h"

class CTimeManager
{
	friend class CGameManager;

private:
	static LARGE_INTEGER mSecond;
	static LARGE_INTEGER mTime;
	static float mDeltaTime;
	static float mFPS;
	static float mFPSTime;
	static int mFPSTick;

private:
	static void init();
	static float update();

public:
	static float getDeltaTime();
};

