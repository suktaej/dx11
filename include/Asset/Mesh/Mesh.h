#pragma once
#include "../Asset.h"

struct FMeshSlot
{
	FIndexBuffer indexBuffer;
};

class CMesh abstract : public CAsset
{
public:
	CMesh();
	virtual ~CMesh();

protected:
	FVertexBuffer mVertexBuffer;
	std::vector<FMeshSlot*> mMeshSlots;
	D3D11_PRIMITIVE_TOPOLOGY mPrimitive;

public:
	// D3D11_USAGE_DEFAULT : GPU에서 읽고 쓰기가 모두 가능한 버퍼를 생성할 때 사용 (Video memory에 저장, Lock/Unlock으로 CPU에서 접근 가능)
	// D3D11_USAGE_IMMUTABLE : 초기 데이터로만 버퍼를 설정하고 이후에는 변경하지 않을 때 사용 (GPU에서 읽기 전용)
	// D3D11_USAGE_DYNAMIC : CPU에서 자주 업데이트해야 하는 버퍼를 생성할 때 사용 (GPU에서 쓰기 전용, Video memory에 저장)
	//						 system memory에 동일한 버퍼를 생성하며 data 갱신 시 system memory 버퍼에 먼저 데이터를 업데이트한 후  
	//						 UpdateSubresource로 GPU 버퍼에 복사 (빈번한 수정이 필요한 버퍼에 적합)
	// D3D11_USAGE_STAGING : CPU에서 읽고 쓰기가 모두 가능한 버퍼를 생성할 때 사용 (GPU에서 직접 접근 불가하므로 출력할 수 없음, 갱신 전용 버퍼)
	virtual bool createMesh(
		void* vertexData,
		int vertexSize,
		int vertexCount,
		D3D11_USAGE vertexUsage,
		D3D11_PRIMITIVE_TOPOLOGY primitive,
		void* indexData,
		int indexSize,
		int indexCount,
		DXGI_FORMAT indexFormat = DXGI_FORMAT_UNKNOWN,
		D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT) = 0;

	bool CreateBuffer(ID3D11Buffer** buffer, D3D11_BIND_FLAG flag, void* data, int size, int count, D3D11_USAGE usage);
};

