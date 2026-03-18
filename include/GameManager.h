#pragma once
#include "GameInfo.h"

class CGameManager
{
	DECLARE_SINGLE(CGameManager);

private:
	HINSTANCE mhInst = 0;
	HWND mhWnd = 0;
	TCHAR mClassName[256] = {};
	TCHAR mTitleName[256] = {};
	static bool mLoop;

private:
	void RegisterWindowClass();
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool create();
	void logic();
	void input(float dt);
	bool update(float dt);
	void collision(float dt);
	void render(float dt);

public:
	bool init(HINSTANCE hInst);
	int run();
};

