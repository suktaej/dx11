#pragma once
#include "Mesh.h"

class CStaticMesh : public CMesh
{
public:
	CStaticMesh(class CDeviceManager& deviceMgr);
	virtual ~CStaticMesh();
};

