#include "MeshManager.h"
#include "../../DeviceManager.h"
#include "StaticMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::init(CDeviceManager& device)
{
    mDeviceMgr = &device;
    testCode();

	return true;
}

CMesh* CMeshManager::findMesh(const std::string& name)
{
    // std::unordered_map<std::string, std::unique_ptr<CMesh>>::iterator
    auto it = mMeshMap.find(name);

    if (it != mMeshMap.end()) 
        return it->second.get();
    
    return nullptr;
}

void CMeshManager::testCode()
{
    // TestCode
    FVertexPosColor boxVertex[8] =
    {
        // ¾Õ¸é (Z = 0.0f)
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }, // 0: °ËÁ¤ (ÁÂÇÏ´Ü)
		{ XMFLOAT3(-0.5f, 0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 1: ÃÊ·Ï (ÁÂ»ó´Ü)
        { XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }, // 2: ³ë¶û (¿ì»ó´Ü)
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // 3: »¡°­ (¿ìÇÏ´Ü)

		// µÞ¸é (Z = 1.0f)
		{ XMFLOAT3(-1.f,-1.f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // 4: ÆÄ¶û (ÁÂÇÏ´Ü)
		{ XMFLOAT3(-1.f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }, // 5: Ã»·Ï (ÁÂ»ó´Ü)
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }, // 6: ÇÏ¾ç (¿ì»ó´Ü)
		{ XMFLOAT3(1.0f, -1.f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }  // 7: º¸¶ó (¿ìÇÏ´Ü)
    };

    uint32_t indices[36] =
    {
        // ¾Õ¸é
        0, 1, 2, 0, 2, 3,
        // µÞ¸é
        4, 6, 5, 4, 7, 6,
        // ¿ÞÂÊ¸é
        4, 5, 1, 4, 1, 0,
        // ¿À¸¥ÂÊ¸é
        3, 2, 6, 3, 6, 7,
        // À­¸é
        1, 5, 6, 1, 6, 2,
        // ¾Æ·§¸é
        4, 0, 3, 4, 3, 7
    };

    FMeshDesc desc;
    desc.device = mDeviceMgr->getDevice();
    desc.Name = "ColoredBox";
    desc.pVertexData = boxVertex;
    desc.VertexSize = sizeof(FVertexPosColor);
    desc.VertexCount = 8;
    desc.VertexUsage = D3D11_USAGE_DEFAULT;
    desc.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.pIndexData = indices;
    desc.IndexSize = sizeof(uint32_t);
    desc.IndexCount = 36;
    desc.IndexFormat = DXGI_FORMAT_R32_UINT;
    desc.IndexUsage = D3D11_USAGE_DEFAULT;
    desc.bKeepVertexData = true;
    desc.bKeepIndexData = true;

    createMesh<CStaticMesh>(desc);
}
