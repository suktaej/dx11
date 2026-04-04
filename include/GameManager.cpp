#include "GameManager.h"
#include "resource.h"

#include "Asset/Mesh/StaticMesh.h"
#include "Asset/Mesh/MeshManager.h"
#include "Shader/ColorMeshShader.h"

#include "Shader/TransformConstantBuffer.h" //TEST

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
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

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
    return false;
}

void CGameManager::collision(float dt)
{
}

void CGameManager::render(float dt)
{
    // 1. 초기화
    mDevice.clearBuffers(mClearColor);

    // 2. 기하 단계 (G-Buffer)
    mDevice.BeginGeometryPass();

    testRender();

    // TODO: 모든 오브젝트 Draw (깊이 쓰기 ON 상태)

    /*
    // 3. 라이팅 단계 (Lighting Buffer)
    mDevice.unbindShaderResources(); // G-Buffer RTV를 해제하기 위해
    mDevice.setLightingTarget(); // 여기서 깊이 쓰기 OFF 권장
    mDevice.setPostProcessSource(mDevice.getGBufferSRV(0)); // t0, t1, t2... 바인딩 필요
    // TODO: FSQ Draw (Lighting Shader)

    // 4. Resolve (MSAA -> Non-MSAA)
    mDevice.unbindShaderResources(); // 입력으로 썼던 G-Buffer 해제
    mDevice.preparePostProcess();

    // 5. 포스트 프로세스 핑퐁
    // Step A: HDR -> Post0
    mDevice.setPostProcessTarget(0);
    mDevice.setPostProcessSource(mDevice.getResolvedHDRSRV());
    // TODO: FSQ Draw (Effect 1)
    mDevice.unbindShaderResources();

    // Step B: Post0 -> Post1
    mDevice.setPostProcessTarget(1);
    mDevice.setPostProcessSource(mDevice.getPostProcessSRV(0));
    // TODO: FSQ Draw (Effect 2)
    mDevice.unbindShaderResources();

    // 6. 최종 출력 (톤 매핑 및 백버퍼)
    mDevice.setFinalTarget();
    mDevice.setPostProcessSource(mDevice.getPostProcessSRV(1));
    // TODO: FSQ Draw (Tone-mapping Shader)

    // 7. UI 및 Present
    // mDevice.drawUI(); // UI는 보통 백버퍼 위에 직접 그림
    mDevice.present(); // mSwapChain->Present(1, 0)
    */
}

bool CGameManager::init(HINSTANCE hInst)
{
	mhInst = hInst;
	lstrcpy(mClassName, TEXT("DX11"));
	lstrcpy(mTitleName, TEXT("DX11"));

	RegisterWindowClass();

    if (!InitInstance())
        return false;

    if(false == mDevice.init(mhWnd, rc.right, rc.bottom, true))
		return false;
 
    if (false == mAsset.init(mDevice))
        return false;

    if (false == mShader.init(mDevice))
        return false;

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
            logic();
        }
    }

    return (int)msg.wParam;
}

void CGameManager::testRender()
{
	static DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 1.0f);
	static DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	float testSpeed = 5.0f;

	// case1: scalar 연산
	float dtSpeed = testSpeed * mTime.getDeltaTime();
	// case2: vector 연산
    using DirectX::operator*;
    using DirectX::operator-=;
    DirectX::XMVECTOR vSpeed = DirectX::XMVectorReplicate(testSpeed * mTime.getDeltaTime());
    DirectX::XMVECTOR vDir = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	if (GetAsyncKeyState('W') & 0x8000)
        pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), dtSpeed));

	if (GetAsyncKeyState('S') & 0x8000)
        pos -= vDir * vSpeed;
    
	static CTransformConstantBuffer testCBuffer;
    DirectX::XMFLOAT4X4 world, view, projection;

    // world
    DirectX::XMFLOAT4X4 translate, rotate, scale;
    // translate
    DirectX::XMStoreFloat4x4(&translate, DirectX::XMMatrixTranslationFromVector(pos));
    //DirectX::XMStoreFloat4x4(&translate, DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f));
    // rotation
    float pitch = DirectX::XMConvertToRadians(45.0f);
    float yaw = DirectX::XMConvertToRadians(45.0f);
    float roll = 0.0f;
    //XMStoreFloat4x4(&rotate, XMMatrixRotationY(XMConvertToRadians(45.0f)));
    DirectX::XMStoreFloat4x4(&rotate, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
    // scale
    DirectX::XMStoreFloat4x4(&scale, DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
    // SRT
    DirectX::XMStoreFloat4x4(&world, XMLoadFloat4x4(&scale) * XMLoadFloat4x4(&rotate) * XMLoadFloat4x4(&translate));

    // view
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixLookAtLH(eye, at, up));

	// projection
    DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)rc.right / rc.bottom, 0.1f, 100.0f));

    testCBuffer.setWorld(world);
    testCBuffer.setView(view);
    testCBuffer.setProjection(projection);
    testCBuffer.init(mShader);
	testCBuffer.updateBuffer(mDevice.getContext());

    CShader* testShader = mShader.findShader("TestShader");
    testShader->setShader(mDevice.getContext());

    CMesh* testMesh = mAsset.getMeshManager()->findMesh("ColoredBox");
    testMesh->render(mDevice.getContext());

    mDevice.testRender();
}
