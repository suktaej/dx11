#include "InputManager.h"
#include "ServiceLocator.h"
#include <format>

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

bool CInputManager::processMessage(UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
	case WM_KEYDOWN:	// 일반 키
	case WM_SYSKEYDOWN: // Modi key
	{
		if (lParam & (1 << 30))  // 이전 프레임에도 눌려있었으면 스킵
			break;

		unsigned char keyCode = static_cast<unsigned char>(wParam);
		mKeyState[keyCode] = 0x80; // 눌림
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		unsigned char keyCode = static_cast<unsigned char>(wParam);
		mKeyState[keyCode] = 0x00; // 뗌
		break;
	}
	case WM_LBUTTONDOWN: mKeyState[VK_LBUTTON] = 0x80; break;
	case WM_LBUTTONUP:   mKeyState[VK_LBUTTON] = 0x00; break;
	case WM_RBUTTONDOWN: mKeyState[VK_RBUTTON] = 0x80; break;
	case WM_RBUTTONUP:   mKeyState[VK_RBUTTON] = 0x00; break;
	case WM_MBUTTONDOWN: mKeyState[VK_MBUTTON] = 0x80; break;
	case WM_MBUTTONUP:   mKeyState[VK_MBUTTON] = 0x00; break;
	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (delta > 0)
		{
			mKeyState[VK_WHEEL_UP] = 0x80; 
			mKeyState[VK_WHEEL_DOWN] = 0x00;
		}
		else if (delta < 0)
		{
			mKeyState[VK_WHEEL_UP] = 0x00; 
			mKeyState[VK_WHEEL_DOWN] = 0x80;
		}
		else
		{
			mKeyState[VK_WHEEL_UP] = 0x00;
			mKeyState[VK_WHEEL_DOWN] = 0x00;
		}

		break;
	}
	default:
		return false;
	}

	return true;
}

EModifier CInputManager::getCurrentMod() const
{
	EModifier mod = EModifier::None;

	if (mKeyState[VK_CONTROL] & 0x80) 
		mod = mod | EModifier::Ctrl;

	if (mKeyState[VK_MENU] & 0x80) 
		mod = mod | EModifier::Alt;

	if (mKeyState[VK_SHIFT] & 0x80) 
		mod = mod | EModifier::Shift;

	return mod;
}

FKeyState* CInputManager::findKeyState(unsigned char key)
{
	auto it = mKeyStateMap.find(key);

	if (it == mKeyStateMap.end())
		return nullptr;

	return it->second.get();
}

FKeyState* CInputManager::addKeyState(unsigned char key)
{
	FKeyState* state = findKeyState(key);
	if (state)
		return state;
	
	std::unique_ptr<FKeyState> newState = std::make_unique<FKeyState>();
	newState->key = key;
	state = newState.get();
	mKeyStateMap.insert(std::make_pair(key, std::move(newState)));
	
	return state;
}


bool CInputManager::init()
{ 
	return true;
}

void CInputManager::update()
{
	updateMousePos();

	for (auto& [key, state] : mKeyStateMap)
	{
		bool curPressed = mKeyState[state->key] & 0x80;
		bool prevPressed = mPrevState[state->key] & 0x80;

		state->down = curPressed && !prevPressed;
		state->hold = curPressed && prevPressed;
		state->up = !curPressed && prevPressed;
	}

	memcpy(mPrevState, mKeyState, 256);
	
	// 다음 프레임을 위해 휠 플래그 초기화
	// memcpy 이후에 초기화해야 mPrevState에 이전 휠 상태가 남아서 
	// down 계산이 1프레임 정상 동작
	mKeyState[VK_WHEEL_UP] = 0x00;
	mKeyState[VK_WHEEL_DOWN] = 0x00;
}

void CInputManager::updateMousePos()
{
	using namespace DirectX;

	mMouseMove = { 0.f, 0.f };
	
	IGame& game = CServiceLocator::getGame();

	// 윈도우 창에서의 마우스 위치 확인
	// GetCursorPos 함수는 스크린 좌표를 반환
	// 스크린 좌표는 모니터상에서의 좌표를 도출
	POINT MousePT;
	GetCursorPos(&MousePT);

	// 스크린 좌표를 클라이언트 좌표로 변환
	ScreenToClient(game.getHandle(), &MousePT);

	// 디바이스로부터 다이렉트 해상도와 윈도우 창 크기의 비율을 얻어옴
	IDevice& device = CServiceLocator::getDevice();

	XMFLOAT2 Ratio = device.getResolutionRatio();
	FResolution	ViewportRS = device.getResolution();

	XMFLOAT2 MousePos;

	// 윈도우 창에서의 마우스 위치를 해상도 비율을 곱
	// DirectX Viewport 상에서의 위치를 도출
	MousePos.x = MousePT.x * Ratio.x;
	MousePos.y = MousePT.y * Ratio.y;

	// 윈도우는 Y좌표가 아래로 Y+ 방향
	// DirectX에서는 Y좌표가 위로 Y+ 방향
	// 그러므로 뷰포트 해상도를 이용하여 Y좌표를 반전
	MousePos.y = ViewportRS.height - MousePos.y;

	if (mMouseCompute)
	{
		mMouseMove.x = MousePos.x - mMousePos.x;
		mMouseMove.y = MousePos.y - mMousePos.y;
	}
	else
		mMouseCompute = true;

	mMousePos = MousePos;

	//TODO : 카메라 위치값 입력
}

void CInputManager::printMousePos()
{
	std::string debugStr = std::format("Pos: ({:.2f}, {:.2f}) Move: ({:.2f}, {:.2f})\n",
		mMousePos.x, mMousePos.y, mMouseMove.x, mMouseMove.y);
	OutputDebugStringA(debugStr.c_str());
}
