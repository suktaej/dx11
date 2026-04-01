#pragma once
#include "Shader.h"
class CComputeShader : public CShader
{
protected:
	CComputeShader();
	virtual ~CComputeShader();

public:
	//bool init() override;
	bool create(ID3D11Device* device) override;
	void setShader(ID3D11DeviceContext* context) final;
};

