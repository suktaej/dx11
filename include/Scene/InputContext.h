#pragma once
#include "../GameInfo.h"

struct FBindFunction
{
	void* obj = nullptr;
	std::function<void(float)> func;
};

struct FBindKey
{
	FKeyState* state = nullptr;
	EModifier modifier = EModifier::None;
	bool keyHold = false;
	std::vector<FBindFunction> funcArr[static_cast<int>(EInputType::End)];
};

class CInputContext
{
public:
	class InputKey
	{
		friend class CScene;
		InputKey() {}
	};

	CInputContext(InputKey key);
	~CInputContext();

private:
	std::unordered_map<std::string, std::unique_ptr<FBindKey>> mBindKeyMap;

public:
	bool init();
	void update(float dt);

public:
	FBindKey* findBindKey(const std::string& name);
	void addBindKey(const std::string& name, const unsigned char& key);
	void setModifier(const std::string& name, EModifier mod);
	
public:
	template <typename T>
	void BindAction(T* object, void (T::* func)(float), const std::string& name, EInputType type = EInputType::Down)
	{
		FBindKey* BindKey = findBindKey(name);

		if (!BindKey)
			return;

		FBindFunction bindFunc;

		bindFunc.obj = object;
		// std::bind는 함수주소와 객체를 이용하여 function 객체를 생성
		// 멤버함수의 경우 단독호출이 불가능하므로 객체가 필요
		// std::bind(callable function, target object, params...)
		bindFunc.func = std::bind(func, object, std::placeholders::_1);
		
		int numType = static_cast<int>(type);
		BindKey->funcArr[numType].emplace_back(bindFunc);
	}
};

