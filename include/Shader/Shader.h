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
	virtual bool create(ID3D11Device* device) = 0;
	virtual void setShader(ID3D11DeviceContext* context) = 0;
};

