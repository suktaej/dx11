#pragma once
#include "GraphicShader.h"
class CColorMeshShader :
    public CGraphicShader
{
protected:
	CColorMeshShader();
	~CColorMeshShader();

public:
	bool init(class CDeviceManager& device) override;
};

