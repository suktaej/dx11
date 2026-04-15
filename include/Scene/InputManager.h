#pragma once
#include "../GameInfo.h"

#define DIRECTINPUT_VERSION 0x0800
#define	DIK_MOUSELBUTTON	0xfc
#define	DIK_MOUSERBUTTON	0xfd
#define	DIK_MOUSEWHEEL		0xfe

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

enum class EInputSystem { DInput, RawInput, XInput, Win32 };
namespace EMouseButtonType { enum Type { LButton, RButton, MButton, End }; }
namespace EInputType { enum Type { Down, Hold, Up, End }; }

//enum EInputState
//{
//	STATE_NONE = 0,
//	STATE_DOWN = 1 << 0,
//	STATE_HOLD = 1 << 1,
//	STATE_UP = 1 << 2
//};

struct FKeyState
{
	unsigned char	Key = 0;
	bool			Down = false;
	bool			Hold = false;
	bool			Up = false;
};

struct FBindFunction
{
	void* Obj = nullptr;
	std::function<void(float)>	Func;
};

struct FBindKey
{
	FKeyState* Key = nullptr;
	bool	Ctrl = false;
	bool	Alt = false;
	bool	Shift = false;
	bool	KeyHold = false;
	std::vector<FBindFunction>	FunctionList[static_cast<int>(EInputType::End)];
};

class CInputManager
{
public:
	class InputKey
	{
		friend class CScene;
		InputKey() {}
	};

	CInputManager(InputKey key);
	~CInputManager();

private:
	HINSTANCE mhInst;
	HWND mhWnd;
	ComPtr<IDirectInput8> mInput = nullptr;
	ComPtr<IDirectInputDevice8> mKeyboard = nullptr;
	ComPtr<IDirectInputDevice8> mMouse = nullptr;
	EInputSystem mInputType = EInputSystem::DInput;
	bool mWindowMode;

	unsigned char mKeyState[256] = {};
	DIMOUSESTATE mMouseState = {};

	bool mCtrl[EInputType::End] = {};
	bool mAlt[EInputType::End] = {};
	bool mShift[EInputType::End] = {};
	bool mMouseDown[EMouseButtonType::End] = {};
	bool mMouseHold[EMouseButtonType::End] = {};
	bool mMouseUp[EMouseButtonType::End] = {};

	DirectX::XMFLOAT2 mMousePos;
	DirectX::XMFLOAT2 mMouseWorldPos2D;
	DirectX::XMFLOAT2 mMouseMove;

	std::unordered_map<unsigned char, FKeyState*>	mKeyStateMap;
	std::unordered_map<std::string, FBindKey*>		mBindKeyMap;
	// 첫 번째 프레임은 이전 마우스 위치가 의미가 없으므로
	// 두 번째 프레임부터 마우스 이동을 계산
	bool mMouseCompute = false;

	//unsigned char mCtrl = STATE_NONE;
	//unsigned char mAlt = STATE_NONE;
	//unsigned char mShift = STATE_NONE;

private:
	bool initInput();
	bool updateKeyboard();
	bool updateMouse();
	void updateInput(float dt);
	FKeyState* findKeyState(unsigned char Key);
	FBindKey* findBindKey(const std::string& Name);

public:
	bool init();
	void update(float dt);

public:
	void addBindKey(const std::string& Name, unsigned char Key);
	void changeKeyCtrl(const std::string& Name, bool Ctrl);
	void changeKeyAlt(const std::string& Name, bool Alt);
	void changeKeyShift(const std::string& Name, bool Shift);

private:
	void updateMousePos(float DeltaTime);
	void updateBind(float DeltaTime);

public:
	bool getMouseDown(EMouseButtonType::Type Type) { return mMouseDown[Type]; }
	bool getMouseHold(EMouseButtonType::Type Type) { return mMouseHold[Type]; }
	 bool getMouseUp(EMouseButtonType::Type Type) { return mMouseUp[Type]; }
	const DirectX::XMFLOAT2& getMousePos()	const { return mMousePos; }
	const DirectX::XMFLOAT2& getMouseWorldPos2D()	const { return mMouseWorldPos2D; }
	const DirectX::XMFLOAT2& getMouseMove()	const { return mMouseMove; }

public:
	template <typename T>
	void addBindFunction(const std::string& KeyName, EInputType::Type Type,
		T* Object, void (T::* Func)(float))
	{
		FBindKey* BindKey = findBindKey(KeyName);

		if (!BindKey)
			return;

		FBindFunction	BindFunc;

		BindFunc.Obj = Object;
		// std::bind를 이용해서 함수주소와 객체를 이용하여 function 객체를
		// 만들어준다.
		BindFunc.Func = std::bind(Func, Object, std::placeholders::_1);

		BindKey->FunctionList[Type].emplace_back(BindFunc);
	}

private:
	unsigned char convertKey(unsigned char Key);
};

