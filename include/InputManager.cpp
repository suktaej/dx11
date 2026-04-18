#include "InputManager.h"
#include "ServiceLocator.h"

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

void CInputManager::update(float dt)
{
	for (auto& [key, state] : mKeyStateMap)
	{
		bool curPressed = mKeyState[state->key] & 0x80;
		bool prevPressed = mPrevState[state->key] & 0x80;

		state->down = curPressed && !prevPressed;
		state->hold = curPressed && prevPressed;
		state->up = !curPressed && prevPressed;
	}

	memcpy(mPrevState, mKeyState, 256);
}
