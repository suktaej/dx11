#pragma once
#include "GameInfo.h"
#include "DeviceManager.h"

class CGameManager
{
	DECLARE_SINGLE(CGameManager);
//public:
//	CGameManager();
//	~CGameManager();

private:
	HINSTANCE mhInst = 0;
	HWND mhWnd = 0;
	TCHAR mClassName[256] = {};
	TCHAR mTitleName[256] = {};
    RECT rc = { 0, 0, 1280, 720 };
	static bool mLoop;

private:
	void RegisterWindowClass();
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL InitInstance();
	
	void logic();
	void input(float dt);
	bool update(float dt);
	void collision(float dt);
	void render(float dt);

public:
	bool init(HINSTANCE hInst);
	int run();
};

