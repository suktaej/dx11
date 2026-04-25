#pragma once
#include "GameInfo.h"
#include "ServiceInterface.h"
#define	VK_WHEEL_UP 0xfc
#define	VK_WHEEL_DOWN 0xfd

//#define INPUTDEBUG

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

	bool mMouseCompute = false;
	DirectX::XMFLOAT2 mMousePos;
	DirectX::XMFLOAT2 mMouseMove;

private:
	void updateMousePos();

public:
	bool init();
	void update();

public:
	EModifier getCurrentMod() const override;
	bool processMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	FKeyState* findKeyState(unsigned char key) override;
	FKeyState* addKeyState(unsigned char Key) override;
	const DirectX::XMFLOAT2& getMouseMoveAmount() const override { return mMouseMove; }

#ifdef INPUTDEBUG
private:
	void printMousePos();
#endif
};
