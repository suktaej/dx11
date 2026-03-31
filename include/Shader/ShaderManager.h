#pragma once
#include "../GameInfo.h"

class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

private:
	class CDeviceManager* mDeviceMgr;
	std::unordered_map<std::string, std::unique_ptr<class CShader>> mShaderMap;

public:
	bool init(CDeviceManager& device);
	CShader* findShader(const std::string& name);

public:
	template <typename T>
	bool createShader(const std::string& name)
	{
		static_assert(std::is_base_of<CShader, T>::value, "T must inherit from CShader");
		
		if (findShader(name))
			return true;

		auto shader = std::make_unique<T>();
		
		shader->setName(name);
		
		if (!shader->init(*mDeviceMgr))
			return false;
		
		mShaderMap.insert(std::make_pair(name, std::move(shader)));
		
		return true;
	}
	
};
