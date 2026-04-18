#include "InputContext.h"
#include "../ServiceLocator.h"

CInputContext::CInputContext(InputKey key)
{
}

CInputContext::~CInputContext()
{
}

FBindKey* CInputContext::findBindKey(const std::string& name)
{
	auto it = mBindKeyMap.find(name);

	if (it == mBindKeyMap.end())
		return nullptr;

	return it->second.get();
}

void CInputContext::addBindKey(const std::string& name, const unsigned char& key)
{
	if (findBindKey(name))
		return;

	std::unique_ptr<FBindKey> newKey = std::make_unique<FBindKey>();

	IInput& input = CServiceLocator::getInput();
	FKeyState* state = input.findKeyState(key);

	if (!state)
		state = input.addKeyState(key);

	newKey->state = state;
	mBindKeyMap.insert(std::make_pair(name, std::move(newKey)));
}

void CInputContext::setModifier(const std::string& name, EModifier mod)
{
	FBindKey* bind = findBindKey(name);

	if (!bind) 
		return;

	bind->modifier = bind->modifier | mod;
}

void CInputContext::update(float dt)
{
	IInput& input = CServiceLocator::getInput();
	EModifier curMod = input.getCurrentMod();

	for (auto& [name, bind] : mBindKeyMap)
	{
		if (bind->modifier != curMod)
			continue;

		bool active[static_cast<int>(EInputType::End)] = 
		{
			bind->state->down,
			bind->state->hold,
			bind->state->up
		};

		for (int i = 0; i < static_cast<int>(EInputType::End); ++i)
		{
			if (!active[i]) 
				continue;
			
			for (auto& bindFunc : bind->funcArr[i])
				bindFunc.func(dt);
		}
	}
}
