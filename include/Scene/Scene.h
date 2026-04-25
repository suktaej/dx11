#pragma once
#include "../GameInfo.h"
#include "../Object/Object.h"

struct FInstanceBatch
{
	class CGraphicShader* shader = nullptr;
	std::vector<DirectX::XMFLOAT4X4> matrices;
};

struct FPairHash
{
	size_t operator()(const std::pair<class CMesh*, class CGraphicShader*>& p) const
	{
		size_t h1 = std::hash<class CMesh*>{}(p.first);
		size_t h2 = std::hash<class CGraphicShader*>{}(p.second);
		return h1 ^ (h2 << 1);
	}
};

class CScene abstract
{
public:
	class SceneKey
	{
		friend class CSceneManager;	
		SceneKey() {}
	};

	CScene(SceneKey key);
	virtual ~CScene();

protected:
	std::vector<std::unique_ptr<CObject>> mObjectList;
	std::vector<std::unique_ptr<CObject>> mPendingAdd;
	std::vector<CObject*> mPendingRemove;

	std::unique_ptr<class CInputContext> mInput;
	std::unique_ptr<class CMainCamera> mMainCam;
	std::unique_ptr<class CFrameConstantBuffer> mFrameCB;

#if MESHCALL_TYPE == 1
	std::unordered_map<std::pair<class CMesh*, class CGraphicShader*>, FInstanceBatch, FPairHash> mBatchMap;

	ComPtr<ID3D11Buffer> mInstanceBuffer = nullptr;
	ComPtr<ID3D11ShaderResourceView> mInstanceSRV = nullptr;
	UINT mInstanceBufferCapacity = 0;
#endif

private:
	void objectCleanUp();
	void updateFrameBuffer();
	void debugFPS(float dt);

public:
	virtual bool init();
	virtual bool init(const char* filePath);
	virtual void input(float dt);
	virtual void prevUpdate(float dt);
	virtual void update(float dt);
	virtual void postUpdate(float dt);
	virtual void prevCollision(float dt);
	virtual void collision(float dt);
	virtual void postCollision(float dt);
	virtual void prevRender();
	virtual void render();
	virtual void postRender();

public:
	class CInputContext* getInput() const { return mInput.get(); }
	class CMainCamera* getSceneCamera() const { return mMainCam.get(); }
	void addPendingRemove(class CObject* obj) { mPendingRemove.push_back(obj); }

#if MESHCALL_TYPE == 1
	void setInstanceBatch(class CMesh* mesh, class CGraphicShader* shader, const DirectX::XMFLOAT4X4& world);
	void updateInstanceBuffer(const std::vector<DirectX::XMFLOAT4X4>& matrices);
#endif

public:
	template<typename T>
	T* createObject(const std::string& name = "Object")
	{
		static_assert(std::is_base_of_v<CObject, T>, "T must be derived from CObject");

		//std::unique_ptr<T> newObj(T(typename T::ObjectKey{}));
		std::unique_ptr<T> newObj = std::make_unique<T>(typename T::ObjectKey{});
		newObj->setName(name);
		newObj->setScene(*this);

		if(!newObj->init(this))
			return nullptr;

		T* newObjPtr = newObj.get();

		//mObjectList.push_back(std::move(newObj));
		// 생성요청 프레임에서 처리하지 않고 다음 프레임에서 처리
		mPendingAdd.push_back(std::move(newObj));

		return newObjPtr;
	}

private:
	template<typename F>
	void processObject(F&& func)
	{
		for (auto& obj : mObjectList)
		{
			if (!obj->isActive() || !obj->isEnabled())
				continue;

			func(obj.get());
		}
	}

};

/*
std::unordered_map<class CMesh*, std::vector<DirectX::XMFLOAT4X4>> mInstanceMap;
std::unordered_map<std::pair<class CMesh*, class CGraphicShader*>, class CGraphicShader*> mShaderMap;

void meshGrouping();
void setInstanceMap(class CMesh* mesh, DirectX::XMFLOAT4X4 world);
void setShaderMap(class CMesh* mesh, class CGraphicShader* shader);
void eraseRemovedObjects();

template<typename F>
void processObject(F&& func)
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		CObject* obj = (*it).get();

		if (!obj->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!obj->isEnabled())
		{
			++it;
			continue;
		}

		func(obj);
		++it;
	}
}
*/
