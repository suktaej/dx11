#pragma once
#include "../GameInfo.h"
#include "../DeviceManager.h"

class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

private:
	CDeviceManager* mDeviceMgr;
	std::unordered_map<std::string, std::unique_ptr<class CShader>> mShaderMap;
	std::unordered_map<std::string, std::unique_ptr<class CConstantBuffer>> mConstantBufferMap;

public:
	bool init(CDeviceManager& device);
	CShader* findShader(const std::string& name);
	void releaseShader(const std::string& name);
	bool createConstantBuffer(const std::string& name, int size, int registerSlot, EShaderBufferType bufferType = EShaderBufferType::Graphic);
	CConstantBuffer* findConstantBuffer(const std::string& name);
	void releaseConstantBuffer(const std::string& name);

public:
	template <typename T>
	bool createShader(const std::string& name)
	{
		static_assert(std::is_base_of<CShader, T>::value, "T must inherit from CShader");
		
		if (findShader(name))
			return true;
		
		auto shader = std::make_unique<T>(typename T::ShaderKey{});
		
		if (!shader->create(mDeviceMgr->getDevice()))
			return false;
		
		mShaderMap.insert(std::make_pair(name, std::move(shader)));
		
		return true;
	}
};
