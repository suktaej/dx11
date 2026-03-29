#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <DirectXMath.h>
using namespace DirectX;
#include <dxgi1_6.h>

#include <crtdbg.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

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

#define LEAK_POS(val) _CrtSetBreakAlloc(val);

struct FResolution
{
	unsigned int width = 0;
	unsigned int height = 0;
};

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
	void* data = nullptr;

	FIndexBuffer() {}
	~FIndexBuffer() { SAFE_DELETE_ARRAY(data); }
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
	void* data = nullptr;

	FVertexBuffer() {}
	~FVertexBuffer() { SAFE_DELETE_ARRAY(data); }
};
