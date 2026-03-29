#include "Mesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	size_t size = mMeshSlots.size();

	for (size_t i = 0; i < size; ++i)
		SAFE_DELETE(mMeshSlots[i]);
}

bool CMesh::createMesh(void* vertexData, int vertexSize, int vertexCount, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, int indexSize, int indexCount, DXGI_FORMAT indexFormat, D3D11_USAGE indexUsage)
{
	mVertexBuffer.size = vertexSize;
	mVertexBuffer.count = vertexCount;
	mVertexBuffer.data = new char[vertexSize * vertexCount];
	memcpy(mVertexBuffer.data, vertexData, vertexSize * vertexCount);

	mPrimitive = primitive;
	return false;
}

bool CMesh::CreateBuffer(ID3D11Buffer** buffer, D3D11_BIND_FLAG flag, void* data, int size, int count, D3D11_USAGE usage)
{
	D3D11_BUFFER_DESC bufferDesc = {};

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

	D3D11_SUBRESOURCE_DATA bufferData = {};

	bufferData.pSysMem = data;


	return false;
}
