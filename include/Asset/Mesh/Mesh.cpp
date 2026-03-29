#include "Mesh.h"
#include "../../DeviceManager.h"

CMesh::CMesh(class CDeviceManager& deviceMgr)
{
	mAssetType = EAssetType::Mesh;
	mDevice = deviceMgr.getDevice();
	mPrimitive = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
}

CMesh::~CMesh()
{
}

bool CMesh::createBuffer(ID3D11Buffer** buffer, D3D11_BIND_FLAG flag, void* data, UINT size, UINT count, D3D11_USAGE usage)
{
	D3D11_BUFFER_DESC bufferDesc = {};

	// optional
	//bufferDesc.ByteWidth = (bufferDesc.ByteWidth + 15) & ~15;
	bufferDesc.ByteWidth = size * count;
	bufferDesc.Usage = usage;
	bufferDesc.BindFlags = flag;
	// D3D11_CPU_ACCESS_WRITE 플래그는 버퍼가 D3D11_USAGE_DYNAMIC으로 생성될 때 CPU에서 쓰기 접근이 가능하도록 설정
	// D3D11_CPU_ACCESS_READ 플래그는 버퍼가 D3D11_USAGE_STAGING으로 생성될 때 CPU에서 읽기/쓰기 접근이 가능하도록 설정
	if (usage == D3D11_USAGE_DYNAMIC)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (usage == D3D11_USAGE_STAGING)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	else
		bufferDesc.CPUAccessFlags = 0;

	// data가 없을 경우 CreateBuffer의 초기값을 nullptr로 전달하기 위함
	D3D11_SUBRESOURCE_DATA* initData = nullptr;
	D3D11_SUBRESOURCE_DATA bufferData = {};

	if (data)
	{
		bufferData.pSysMem = data;
		initData = &bufferData;
	}

	if (FAILED(mDevice->CreateBuffer(&bufferDesc, initData, buffer)))
		return false;

	return true;
}

bool CMesh::createIndexBuffer(bool keepCPUdata, void* indexData, UINT indexSize, UINT indexCount, DXGI_FORMAT indexFormat, D3D11_USAGE indexUsage)
{
	std::unique_ptr<FMeshSlot> slot = std::make_unique<FMeshSlot>();

	slot->indexBuffer.size = indexSize;
	slot->indexBuffer.count = indexCount;
	
	if (indexFormat == DXGI_FORMAT_UNKNOWN)
	{
		if (indexSize == 2) slot->indexBuffer.format = DXGI_FORMAT_R16_UINT;
		else if (indexSize == 4) slot->indexBuffer.format = DXGI_FORMAT_R32_UINT;
	}
	else
		slot->indexBuffer.format = indexFormat;

	if (keepCPUdata)
	{
		//slot->indexBuffer.data = new char[indexSize * indexCount];
		//memcpy(slot->indexBuffer.data, indexData, indexSize * indexCount);
		size_t totalSize = (size_t)indexSize * indexCount;
		slot->indexBuffer.data.assign((uint8_t*)indexData, (uint8_t*)indexData + totalSize);
	}
	else
		//slot->indexBuffer.data = nullptr;
		slot->indexBuffer.data.clear();

	if (!createBuffer(&slot->indexBuffer.buffer, D3D11_BIND_INDEX_BUFFER, indexData, indexSize, indexCount, indexUsage))
		return false;

	mMeshSlots.push_back(std::move(slot));

	return true;
}

bool CMesh::createVertexBuffer(bool keepCPUdata, void* vertexData, UINT vertexSize, UINT vertexCount, D3D11_USAGE vertexUsage)
{
	mVertexBuffer.size = vertexSize;
	mVertexBuffer.count = vertexCount;

	if (keepCPUdata)
	{
		//mVertexBuffer.data = new char[vertexSize * vertexCount];
		//memcpy(mVertexBuffer.data, vertexData, vertexSize * vertexCount);
		size_t totalSize = (size_t)vertexSize * vertexCount;
		mVertexBuffer.data.assign((uint8_t*)vertexData, (uint8_t*)vertexData + totalSize);
	}
	else
		//mVertexBuffer.data = nullptr;
		mVertexBuffer.data.clear();

	if (!createBuffer(&mVertexBuffer.buffer, D3D11_BIND_VERTEX_BUFFER, vertexData, vertexSize, vertexCount, vertexUsage))
		return false;

	return true;
}

bool CMesh::createMesh(bool keepVertexData, void* vertexData, UINT vertexSize, UINT vertexCount, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, bool keepIndexdata, void* indexData, UINT indexSize, UINT indexCount, DXGI_FORMAT indexFormat, D3D11_USAGE indexUsage)
{
	mPrimitive = primitive;
	
	if (!createVertexBuffer(keepVertexData, vertexData, vertexSize, vertexCount, vertexUsage))
		return false;

	if (indexData)
		if (!createIndexBuffer(keepIndexdata, indexData, indexSize, indexCount, indexFormat, indexUsage))
			return false;

	return true;
}
