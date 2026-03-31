#pragma once
#include "../GameInfo.h"

class CShader abstract
{
	friend class CShaderManager;

public:
	CShader();
	virtual ~CShader();

public:
	virtual bool init(class CDeviceManager& device) = 0;
	virtual void setShader() = 0;
};

