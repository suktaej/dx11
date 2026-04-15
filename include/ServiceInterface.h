#pragma once
#include "GameInfo.h"

struct IGame
{
	virtual ~IGame() {}
	virtual HINSTANCE getInstance() const = 0;
	virtual HWND getHandle() const = 0;
};

struct IDevice
{
	virtual ~IDevice() {}
	//virtual void serviceInit() = 0;
	virtual ID3D11Device* getDevice() = 0;
	virtual ID3D11DeviceContext* getContext() = 0;
	virtual bool getWindowMode() const = 0;
	virtual FResolution getResolution() const = 0;
	virtual DirectX::XMFLOAT2 getResolutionRatio() const = 0;
};

struct IAsset
{
	virtual ~IAsset() {}
	//virtual void serviceInit() = 0;
};

struct IMesh
{
	virtual ~IMesh() {}
	//virtual void serviceInit() = 0;
	virtual class CMesh* getMesh(const std::string& name) = 0;
};

struct IShader
{
	virtual ~IShader() {}
	//virtual void serviceInit() = 0;
	virtual class CShader* getShader(const std::string& name) = 0;
	virtual class CConstantBuffer* getConstant(const std::string& name) = 0;
};
