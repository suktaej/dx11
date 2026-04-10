#pragma once
#include "GraphicShader.h"
class CColorMeshShader :
    public CGraphicShader
{
public:
	CColorMeshShader(ShaderKey key);
	~CColorMeshShader() override;

public:
	//bool init() override;
	bool create() override;
};
