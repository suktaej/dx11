#include "MeshManager.h"
#include "StaticMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::init()
{
    coloredBox();
    coloredSphere();
    coloredTetrahedron();

	return true;
}

void CMeshManager::serviceInit()
{
}

CMesh* CMeshManager::getMesh(const std::string& name)
{
    return findMesh(name);
}

CMesh* CMeshManager::findMesh(const std::string& name)
{
    // std::unordered_map<std::string, std::unique_ptr<CMesh>>::iterator
    auto it = mMeshMap.find(name);

    if (it != mMeshMap.end()) 
        return it->second.get();
    
    return nullptr;
}

void CMeshManager::coloredBox()
{
    // TestCode
    FVertexPosColor boxVertex[8] =
    {
        // 앞면 (Z = 0.0f)
        { DirectX::XMFLOAT3(-1.f, -1.f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }, // 0: 검정 (좌하단)
		{ DirectX::XMFLOAT3(-1.f, 1.f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 1: 초록 (좌상단)
        { DirectX::XMFLOAT3(1.f, 1.f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }, // 2: 노랑 (우상단)
        { DirectX::XMFLOAT3(1.f, -1.f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // 3: 빨강 (우하단)

		// 뒷면 (Z = 1.0f)
		{ DirectX::XMFLOAT3(-1.f,-1.f, 1.f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // 4: 파랑 (좌하단)
		{ DirectX::XMFLOAT3(-1.f, 1.f, 1.f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }, // 5: 청록 (좌상단)
		{ DirectX::XMFLOAT3(1.f, 1.f, 1.f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }, // 6: 하양 (우상단)
		{ DirectX::XMFLOAT3(1.f, -1.f, 1.f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }  // 7: 보라 (우하단)
    };

    uint32_t indices[36] =
    {
        // 앞면
        0, 1, 2, 0, 2, 3,
        // 뒷면
        4, 6, 5, 4, 7, 6,
        // 왼쪽면
        4, 5, 1, 4, 1, 0,
        // 오른쪽면
        3, 2, 6, 3, 6, 7,
        // 윗면
        1, 5, 6, 1, 6, 2,
        // 아랫면
        4, 0, 3, 4, 3, 7
    };

    FMeshDesc desc;
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

void CMeshManager::coloredSphere()
{
    using namespace DirectX;

    const float radius = 1.0f;
    const uint32_t sliceCount = 20; // 가로 분할
    const uint32_t stackCount = 20; // 세로 분할

    std::vector<FVertexPosColor> vertices;
    std::vector<uint32_t> indices;

    // 정점 생성
    for (uint32_t i = 0; i <= stackCount; ++i)
    {
        float theta = XM_PI * i / stackCount;

        for (uint32_t j = 0; j <= sliceCount; ++j)
        {
            float phi = XM_2PI * j / sliceCount;

            FVertexPosColor v;

            v.pos.x = radius * sinf(theta) * cosf(phi);
            v.pos.y = radius * cosf(theta);
            v.pos.z = radius * sinf(theta) * sinf(phi);

            // 색상: 위치 기반
            v.color = DirectX::XMFLOAT4(
                (v.pos.x + 1.f) * 0.5f,
                (v.pos.y + 1.f) * 0.5f,
                (v.pos.z + 1.f) * 0.5f,
                1.f
            );

            vertices.push_back(v);
        }
    }

    // 인덱스 생성
    uint32_t ringVertexCount = sliceCount + 1;

    for (uint32_t i = 0; i < stackCount; ++i)
    {
        for (uint32_t j = 0; j < sliceCount; ++j)
        {
            indices.push_back(i * ringVertexCount + j);
            indices.push_back((i + 1) * ringVertexCount + j);
            indices.push_back((i + 1) * ringVertexCount + j + 1);

            indices.push_back(i * ringVertexCount + j);
            indices.push_back((i + 1) * ringVertexCount + j + 1);
            indices.push_back(i * ringVertexCount + j + 1);
        }
    }

    FMeshDesc desc;
    desc.Name = "ColoredSphere";
    desc.pVertexData = vertices.data();
    desc.VertexSize = sizeof(FVertexPosColor);
    desc.VertexCount = (uint32_t)vertices.size();
    desc.VertexUsage = D3D11_USAGE_DEFAULT;
    desc.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    desc.pIndexData = indices.data();
    desc.IndexSize = sizeof(uint32_t);
    desc.IndexCount = (uint32_t)indices.size();
    desc.IndexFormat = DXGI_FORMAT_R32_UINT;
    desc.IndexUsage = D3D11_USAGE_DEFAULT;

    desc.bKeepVertexData = true;
    desc.bKeepIndexData = true;

    createMesh<CStaticMesh>(desc);
}

void CMeshManager::coloredTetrahedron()
{
    FVertexPosColor vertices[4] =
    {
        // Top
        { {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f, 1.f} },

        // Base
        { {-1.f, -1.f, -1.f}, {0.f, 1.f, 0.f, 1.f} },
        { {1.f, -1.f, -1.f},  {0.f, 0.f, 1.f, 1.f} },
        { {0.f, -1.f, 1.f},   {1.f, 1.f, 0.f, 1.f} },
    };

    uint32_t indices[12] =
    {
        // 옆면
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,

        // 바닥
        1, 3, 2
    };

    FMeshDesc desc;
    desc.Name = "ColoredTetrahedron";
    desc.pVertexData = vertices;
    desc.VertexSize = sizeof(FVertexPosColor);
    desc.VertexCount = 4;
    desc.VertexUsage = D3D11_USAGE_DEFAULT;
    desc.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    desc.pIndexData = indices;
    desc.IndexSize = sizeof(uint32_t);
    desc.IndexCount = 12;
    desc.IndexFormat = DXGI_FORMAT_R32_UINT;
    desc.IndexUsage = D3D11_USAGE_DEFAULT;

    desc.bKeepVertexData = true;
    desc.bKeepIndexData = true;

    createMesh<CStaticMesh>(desc);
}
