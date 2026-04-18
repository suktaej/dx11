#pragma once
#include "GameInfo.h"
#include "ServiceInterface.h"

enum class EInputType : uint8_t
{
	Down,
	Hold,
	Up,
	End
};

enum class EModifier : uint8_t
{
	None = 0,
	Ctrl = 1 << 0,
	Alt = 1 << 1,
	Shift = 1 << 2
};

inline EModifier operator|(EModifier a, EModifier b)
{
	return static_cast<EModifier>(uint8_t(a) | uint8_t(b));
}

inline bool operator&(EModifier a, EModifier b)
{
	return (uint8_t(a) & uint8_t(b)) != 0;
}

struct FKeyState
{
	unsigned char key = 0;
	bool down = false;
	bool hold = false;
	bool up = false;
};

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
	void update(float dt);

public:
	EModifier getCurrentMod() const override;
	bool processMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	FKeyState* findKeyState(unsigned char key) override;
	FKeyState* addKeyState(unsigned char Key) override;
};
