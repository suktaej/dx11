#pragma once
#include "../GameInfo.h"

class CShader abstract
{
public:
	class ShaderKey
	{
		friend class CShaderManager;

	private:
		ShaderKey() {}
	public:
		ShaderKey(const ShaderKey&) {}
	};

	CShader(ShaderKey key);
	virtual ~CShader();

public:
	//virtual bool init() = 0;
	virtual bool create() = 0;
	virtual void setShader() = 0;
};

