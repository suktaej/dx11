#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;

struct IDevice
{
	virtual ~IDevice() {}
	virtual void serviceInit() = 0;
	virtual ID3D11Device* getDevice() = 0;
	virtual ID3D11DeviceContext* getContext() = 0;
};

struct IAsset
{
	virtual ~IAsset() {}
	virtual void serviceInit() = 0;
};

struct IMesh
{
	virtual ~IMesh() {}
	virtual void serviceInit() = 0;
};

struct IShader
{
	virtual ~IShader() {}
	virtual void serviceInit() = 0;
};
