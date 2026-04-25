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
	std::vector<std::unique_ptr<FMeshSlot>> mMeshSlots;
	D3D11_PRIMITIVE_TOPOLOGY mPrimitive;

private:
	bool createBuffer(
		ID3D11Buffer** buffer,
		D3D11_BIND_FLAG flag,
		void* data,
		UINT size,
		UINT count,
		D3D11_USAGE usage);
	
	bool createIndexBuffer(
		bool keepCPUdata,
		void* indexData,
		UINT indexSize,
		UINT indexCount,
		DXGI_FORMAT indexFormat = DXGI_FORMAT_UNKNOWN,
		D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT);
	
	bool createVertexBuffer(
		bool keepCPUdata,
		void* vertexData,
		UINT vertexSize,
		UINT vertexCount,
		D3D11_USAGE vertexUsage);

public:
	// D3D11_USAGE_DEFAULT : GPU에서 읽고 쓰기가 모두 가능한 버퍼를 생성할 때 사용 (Video memory에 저장, Lock/Unlock으로 CPU에서 접근 가능)
	// D3D11_USAGE_IMMUTABLE : 초기 데이터로만 버퍼를 설정하고 이후에는 변경하지 않을 때 사용 (GPU에서 읽기 전용, vertexData가 nullptr 이어서는 안됨)
	// D3D11_USAGE_DYNAMIC: CPU가 Map(D3D11_MAP_WRITE_DISCARD 등)으로 직접 GPU 버퍼에 쓰는 방식
	// D3D11_USAGE_STAGING : CPU에서 읽고 쓰기가 모두 가능한 버퍼를 생성할 때 사용 (GPU에서 직접 접근 불가하므로 출력할 수 없음, 갱신 전용 버퍼)
	virtual bool createMesh(
		bool keepVertexData,
		void* vertexData,
		UINT vertexSize,
		UINT vertexCount,
		D3D11_USAGE vertexUsage,
		D3D11_PRIMITIVE_TOPOLOGY primitive,
		bool keepIndexData,
		void* indexData,
		UINT indexSize = 0,
		UINT indexCount = 0,
		DXGI_FORMAT indexFormat = DXGI_FORMAT_UNKNOWN,
		D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT);

	void render();
	void renderInstanced(UINT instanceCount);
};

