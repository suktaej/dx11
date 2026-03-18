#include "GameManager.h"
#include "resource.h"
#include "Share/TimeManager.h"

DEFINITION_SINGLE(CGameManager)
bool CGameManager::mLoop = true;

CGameManager::CGameManager()
{

}

CGameManager::~CGameManager()
{

}

void CGameManager::RegisterWindowClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = mhInst;
    wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = mClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassExW(&wcex);
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        mLoop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool CGameManager::create()
{
	RECT WindowRC = { 0, 0, 1280, 720 };
    AdjustWindowRect(&WindowRC, WS_OVERLAPPEDWINDOW, FALSE);

    int windowWidth = WindowRC.right - WindowRC.left;
    int windowHeight = WindowRC.bottom - WindowRC.top;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;
    
    mhWnd = CreateWindowW(mClassName, mTitleName, WS_OVERLAPPEDWINDOW,
        posX, posY, windowWidth, windowHeight, nullptr, nullptr, mhInst, nullptr);

    if (!mhWnd)
        return false;

    //SetWindowPos(mhWnd, HWND_TOPMOST, 100, 100, WindowRC.right - WindowRC.left,
    //    WindowRC.bottom - WindowRC.top, SWP_NOMOVE | SWP_NOZORDER);

    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);

    return true;
}

void CGameManager::logic()
{
    float deltaTime = CTimeManager::update();

    input(deltaTime);
    if (update(deltaTime)) return;
    collision(deltaTime);
    render(deltaTime);
}

void CGameManager::input(float dt)
{
}

bool CGameManager::update(float dt)
{
    return true;
}

void CGameManager::collision(float dt)
{
}

void CGameManager::render(float dt)
{
}

bool CGameManager::init(HINSTANCE hInst)
{
	mhInst = hInst;
	lstrcpy(mClassName, TEXT("DX11"));
	lstrcpy(mTitleName, TEXT("DX11"));

	RegisterWindowClass();

    if (!create())
        return false;

	return true;
}

int CGameManager::run()
{
    MSG msg;

    while (mLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
        }
    }

    return (int)msg.wParam;
}
