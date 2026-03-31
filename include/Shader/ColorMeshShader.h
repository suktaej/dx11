#pragma once
#include "GraphicShader.h"
class CColorMeshShader :
    public CGraphicShader
{
public:
	CColorMeshShader();
	~CColorMeshShader() override;

public:
	bool init(class CDeviceManager& device) override;
};

