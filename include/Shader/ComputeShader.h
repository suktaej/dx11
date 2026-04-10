#pragma once
#include "Shader.h"
class CComputeShader : public CShader
{
public:
	CComputeShader(ShaderKey key);
	virtual ~CComputeShader();

public:
	//bool init() override;
	bool create() override;
	void setShader() final;
};

