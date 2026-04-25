#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <functional>

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <dxgi1_6.h>

#include <crtdbg.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#define MESHCALL_TYPE = 0 // 0: object 순회, 1: instanceMap 사용

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; }}
#define	SAFE_DELETE_ARRAY(p) {if(p)	{ delete[] p; p = nullptr; }}
#define	SAFE_RELEASE(p)	{if(p)	{ p->Release(); p = nullptr; }}
#define DEFINITION_SINGLE(Type) Type* Type::mInst = nullptr;
#define DECLARE_SINGLE(Type) \
private:\
	Type();\
	~Type();\
private:\
	static Type* mInst;\
public:\
	static Type* getInst()\
	{\
		if(!mInst)\
			mInst = new Type;\
		return mInst;\
	}\
	static void destroyInst()\
	{\
		SAFE_DELETE(mInst);\
	}


struct FResolution
{
	unsigned int width = 0;
	unsigned int height = 0;
};

struct FBuffer
{
	ComPtr<ID3D11Buffer> buffer = nullptr;
	std::vector<uint8_t> data;
	UINT size = 0;
	UINT count = 0;

	virtual ~FBuffer() {};
};

struct FVertexBuffer : FBuffer { }; 
struct FIndexBuffer : FBuffer { DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN; };

struct FMeshDesc
{
	// 정점 데이터
	std::string Name;
	UINT VertexSize = 0;
	UINT VertexCount = 0;
	D3D11_USAGE VertexUsage = D3D11_USAGE_DEFAULT;
	D3D11_PRIMITIVE_TOPOLOGY Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 실제 데이터 (주소값만 전달)
	const void* pVertexData = nullptr;

	// 인덱스 데이터
	const void* pIndexData = nullptr;
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_UNKNOWN;
	D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT;

	// 옵션
	bool bKeepVertexData = true;
	bool bKeepIndexData = true;
};

struct FVertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

enum class EShaderBufferType
{
	None = 0x0,
	Vertex = 0x1,
	Pixel = 0x2,
	Hull = 0x4,
	Domain = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Hull | Domain | Geometry,
	All = Graphic | Compute
};

inline EShaderBufferType operator|(EShaderBufferType a, EShaderBufferType b)
{
	return static_cast<EShaderBufferType>(static_cast<int>(a) | static_cast<int>(b));
}

inline EShaderBufferType operator&(EShaderBufferType a, EShaderBufferType b)
{
	return static_cast<EShaderBufferType>(static_cast<int>(a) & static_cast<int>(b));
}

inline EShaderBufferType& operator|=(EShaderBufferType& a, EShaderBufferType b)
{
	a = a | b;
	return a;
}

enum class EAssetType
{
	Mesh,
	Texture,
	Material,
	Sound
};

enum class EAxis { None = -1, x, y, z, end };

struct FAxis {
	static DirectX::XMVECTOR Get(EAxis axis)
	{
		switch (axis) {
		case EAxis::x: return DirectX::g_XMIdentityR0; // {1, 0, 0, 0}
		case EAxis::y: return DirectX::g_XMIdentityR1; // {0, 1, 0, 0}
		case EAxis::z: return DirectX::g_XMIdentityR2; // {0, 0, 1, 0}
		default: return DirectX::XMVectorZero();
		}
	}
};

enum class EInputType : uint8_t
{
	Down,
	Hold,
	Up,
	End
};

enum class EModifier : uint8_t
{
	None = 0,
	Ctrl = 1 << 0,
	Alt = 1 << 1,
	Shift = 1 << 2
};

inline EModifier operator|(EModifier a, EModifier b)
{
	return static_cast<EModifier>(uint8_t(a) | uint8_t(b));
}

inline bool operator&(EModifier a, EModifier b)
{
	return (uint8_t(a) & uint8_t(b)) != 0;
}

struct FKeyState
{
	unsigned char key = 0;
	bool down = false;
	bool hold = false;
	bool up = false;
};

/*
struct FIndexBuffer
{
	// 데이터를 저장하기 위한 CPU 메모리 공간
	ComPtr<ID3D11Buffer> buffer = nullptr;
	// 데이터 크기
	UINT size = 0;
	// 데이터 개수
	UINT count = 0;
	// 포멧
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	// 임의의 데이터 포인터 (인덱스 버퍼에 저장된 인덱스 데이터를 CPU에서 접근하기 위한 용도)
	//void* data = nullptr;
	std::vector<uint8_t> data;

	//FIndexBuffer() {}
	//~FIndexBuffer() { SAFE_DELETE_ARRAY(data); }
};

struct FVertexBuffer
{
	// 데이터를 저장하기 위한 CPU 메모리 공간
	ComPtr<ID3D11Buffer> buffer = nullptr;
	// 데이터 크기
	UINT size = 0;
	// 데이터 개수
	UINT count = 0;
	// 임의의 데이터 포인터 (버텍스 버퍼에 저장된 버텍스 데이터를 CPU에서 접근하기 위한 용도)
	//void* data = nullptr;
	std::vector<uint8_t> data;

	//FVertexBuffer() {}
	//~FVertexBuffer() { SAFE_DELETE_ARRAY(data); }
};
*/
