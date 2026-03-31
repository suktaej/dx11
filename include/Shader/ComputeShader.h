#pragma once
#include "Shader.h"
class CComputeShader : public CShader
{
protected:
	CComputeShader();
	virtual ~CComputeShader();

public:
	bool init(class CDeviceManager& device) override;
	void setShader() final;
};

