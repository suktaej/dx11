#pragma once
#include "../GameInfo.h"
#include "../ServiceInterface.h"

class CShaderManager : public IShader
{
public:
	CShaderManager();
	~CShaderManager();

private:
	std::unordered_map<std::string, std::unique_ptr<class CShader>> mShaderMap;
	std::unordered_map<std::string, std::unique_ptr<class CConstantBuffer>> mConstantBufferMap;

public:
	bool init();
	class CShader* findShader(const std::string& name);
	void releaseShader(const std::string& name);
	bool createConstantBuffer(const std::string& name, int size, int registerSlot, EShaderBufferType bufferType = EShaderBufferType::Graphic);
	class CConstantBuffer* findConstantBuffer(const std::string& name);
	void releaseConstantBuffer(const std::string& name);

	void serviceInit() override;
	class CShader* getShader(const std::string& name) override;
	class CConstantBuffer* getConstant(const std::string& name) override;

public:
	template <typename T>
	bool createShader(const std::string& name)
	{
		static_assert(std::is_base_of<CShader, T>::value, "T must inherit from CShader");
		
		if (findShader(name))
			return true;
		
		auto shader = std::make_unique<T>(typename T::ShaderKey{});

		if (!shader->create())
			return false;
		
		mShaderMap.insert(std::make_pair(name, std::move(shader)));
		
		return true;
	}
};
