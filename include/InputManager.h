#pragma once
#include "GameInfo.h"
#include "ServiceInterface.h"

class CInputManager : public IInput
{
	friend class CGameManager;

private:
	CInputManager();
	~CInputManager();

private:
	unsigned char mKeyState[256] = {};
	unsigned char mPrevState[256] = {};
	std::unordered_map<unsigned char, std::unique_ptr<FKeyState>> mKeyStateMap;

public:
	bool init();
	void update();

public:
	EModifier getCurrentMod() const override;
	bool processMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	FKeyState* findKeyState(unsigned char key) override;
	FKeyState* addKeyState(unsigned char Key) override;
};
