#pragma once
#include "GameInfo.h"
#include "ServiceInterface.h"
#include "DeviceManager.h"
#include "InputManager.h"
#include "Share/TimeManager.h"
#include "Asset/AssetManager.h"
#include "Shader/ShaderManager.h"
#include "Scene/SceneManager.h"

class CGameManager : public IGame
{
	//DECLARE_SINGLE(CGameManager);
public:
	CGameManager();
	~CGameManager();

private:
	HINSTANCE mhInst = 0;
	HWND mhWnd = 0;
	TCHAR mClassName[256] = {};
	TCHAR mTitleName[256] = {};
	RECT rc = { 0, 0, 1280, 720 };
	float mClearColor[4] = { 0.5f, 0.5f,0.5f, };
	static bool mLoop;

private:
	CInputManager mInput;
	CDeviceManager mDevice;
	CTimeManager mTime;
	CAssetManager mAsset;
	CShaderManager mShader;
	CSceneManager mScene;

private:
	void RegisterWindowClass();
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL InitInstance();

	void logic();
	void input(float dt);
	void update(float dt);
	void collision(float dt);
	void render(float dt);

public:
	bool init(HINSTANCE hInst);
	int run();

public:
	HINSTANCE getInstance() const override { return mhInst; }
	HWND getHandle() const override { return mhWnd; }
};

