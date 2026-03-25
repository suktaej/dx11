#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <DirectXMath.h>
using namespace DirectX;
#include <dxgi1_6.h>


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

struct FResolution
{
	unsigned int width = 0;
	unsigned int height = 0;
};
