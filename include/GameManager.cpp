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
	ReleaseDC(mhWnd, GetDC(mhWnd));
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

BOOL CGameManager::InitInstance()
{
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

    int windowWidth = rc.right - rc.left;
    int windowHeight = rc.bottom - rc.top;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    mhWnd = CreateWindowW(mClassName, mTitleName, WS_OVERLAPPEDWINDOW,
        posX, posY, windowWidth, windowHeight, nullptr, nullptr, mhInst, nullptr);

    if (!mhWnd)
        return false;

    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);

    return true;
}

void CGameManager::logic()
{
    //float deltaTime = CTimeManager::update();
    float deltaTime = mTime.update();

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

    if (!InitInstance())
        return false;

    //if (!CDeviceManager::getInst()->init(mhWnd, rc.right, rc.bottom, true))
    //    return false;
    if(false == mDevice.init(mhWnd, rc.right, rc.bottom, true))
		return false;
    
    //CTimeManager::init();
    mTime.init();

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
