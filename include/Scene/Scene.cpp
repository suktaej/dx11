#include "Scene.h"
#include "../ServiceLocator.h"
#include "../Object/Object.h"
#include "../Shader/FrameConstantBuffer.h"
#include "../Shader/GraphicShader.h"
#include "../Asset/Mesh/StaticMesh.h"
#include "InputContext.h"
#include "CameraManager.h"


CScene::CScene(SceneKey key)
{
}

CScene::~CScene()
{
}

bool CScene::init()
{
	mInput = std::make_unique<CInputContext>(CInputContext::InputKey{});
	if (!mInput->init()) 
		return false;

	mCameraMgr = std::make_unique<CCameraManager>(CCameraManager::CameraKey{});
	if (!mCameraMgr->init())
		return false;

	mFrameCB = std::make_unique<CFrameConstantBuffer>();
	if (!mFrameCB->init())
		return false;

	CServiceLocator::provideCamera(*mCameraMgr.get());

	return true;
}

bool CScene::init(const char* filePath)
{
	mInput = std::make_unique<CInputContext>(CInputContext::InputKey{});
	if (!mInput->init()) 
		return false;
	
	mCameraMgr = std::make_unique<CCameraManager>(CCameraManager::CameraKey{});
	if (!mCameraMgr->init())
		return false;

	mFrameCB = std::make_unique<CFrameConstantBuffer>();
	if (!mFrameCB->init())
		return false;

	CServiceLocator::provideCamera(*mCameraMgr.get());
	
	return true;
}

void CScene::input(float dt)
{
	mInput->update(dt);
}

void CScene::prevUpdate(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->prevUpdate(dt); });
}

void CScene::update(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->update(dt); });

	mCameraMgr->update(dt);
}

void CScene::postUpdate(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->postUpdate(dt); });

	objectCleanUp();
}

void CScene::prevCollision(float dt)
{
}

void CScene::collision(float dt)
{
}

void CScene::postCollision(float dt)
{
}

void CScene::prevRender()
{
	mInstanceMap.clear();
	
	processObject([](CObject* obj)
		{ obj->prevRender(); });
}

void CScene::render()
{
	updateFrameBuffer();

	processObject([](CObject* obj)
		{ obj->render(); });

	//meshGrouping();
	for (auto& [mesh, matrices] : mInstanceMap)
	{
		updateInstanceBuffer(matrices);
		mesh->renderInstanced((UINT)matrices.size());
	}
}

void CScene::postRender()
{
	processObject([](CObject* obj)
		{ obj->postRender(); });
}

void CScene::updateInstanceBuffer(const std::vector<DirectX::XMFLOAT4X4>& matrices)
{
	IDevice& device = CServiceLocator::getDevice();
	UINT needed = (UINT)matrices.size();

	// 버퍼가 없거나 크기가 부족하면 재생성
	if (!mInstanceBuffer || mInstanceBufferCapacity < needed)
	{
		mInstanceBuffer.Reset();
		mInstanceSRV.Reset();

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(XMFLOAT4X4) * needed;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(XMFLOAT4X4);
		desc.CPUAccessFlags = 0;

		HRESULT bufferHR = device.getDevice()->CreateBuffer(&desc, nullptr, mInstanceBuffer.GetAddressOf());
		if (FAILED(bufferHR))
			return;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = needed;

		HRESULT srvHR = device.getDevice()->CreateShaderResourceView(
			mInstanceBuffer.Get(), &srvDesc, mInstanceSRV.GetAddressOf());
		if (FAILED(srvHR))
			return;

		mInstanceBufferCapacity = needed;
	}

	/*
	// 데이터 업로드
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	device.getContext()->Map(mInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, matrices.data(), sizeof(XMFLOAT4X4) * needed);
	device.getContext()->Unmap(mInstanceBuffer.Get(), 0);
	*/
	
	device.getContext()->UpdateSubresource(
		mInstanceBuffer.Get(),
		0, nullptr,
		matrices.data(),
		0, 0
	);
	
	// VS의 t0 슬롯에 바인딩
	device.getContext()->VSSetShaderResources(0, 1, mInstanceSRV.GetAddressOf());
}

void CScene::setInstanceMap(CStaticMesh* mesh, DirectX::XMFLOAT4X4 world)
{
	mInstanceMap[mesh].push_back(world);
}

void CScene::objectCleanUp()
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}
		else if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->componentCleanUp();
		++it;
	}
}

void CScene::updateFrameBuffer()
{
	// 카메라에서 View,Proj 가져와서 FrameBuffer 한 번만 갱신
	mFrameCB->setView(mCameraMgr->getViewMat());
	mFrameCB->setProjection(mCameraMgr->getProjMat());

	XMMATRIX vp =
		XMLoadFloat4x4(&mCameraMgr->getViewMat()) *
		XMLoadFloat4x4(&mCameraMgr->getProjMat());

	XMFLOAT4X4 mvp;
	XMStoreFloat4x4(&mvp, vp);
	mFrameCB->setVP(mvp);

	mFrameCB->updateBuffer();   // GPU에 한 번만 갱신 
}

/*
// preRender에서 정보취합으로 변경

void CScene::meshGrouping()
{
	std::unordered_map<CStaticMesh*, std::vector<XMFLOAT4X4>> instanceMap;
	std::unordered_map<CStaticMesh*, CGraphicShader*> shaderMap;

	// TODO : Shader type 정리
	for (auto& obj : mObjectList)
		obj->makeStaticMeshBatchList(instanceMap, shaderMap);

	// 메시별로 인스턴싱 드로우
	for (auto& [mesh, matrices] : instanceMap)
	{
		shaderMap[mesh]->setShader();           // 셰이더 바인딩
		updateInstanceBuffer(matrices);         // StructuredBuffer 업데이트
		mesh->renderInstanced((UINT)matrices.size()); // DrawIndexedInstanced
	}
}
*/
