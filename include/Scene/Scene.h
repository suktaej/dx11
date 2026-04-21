#pragma once
#include "../GameInfo.h"
#include "../Object/Object.h"

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
	std::unique_ptr<class CInputContext> mInput;
	std::unique_ptr<class CCameraManager> mCameraMgr;
	std::unique_ptr<class CFrameConstantBuffer> mFrameCB;
	ComPtr<ID3D11Buffer> mInstanceBuffer = nullptr;
	ComPtr<ID3D11ShaderResourceView> mInstanceSRV = nullptr;
	UINT mInstanceBufferCapacity = 0;

	std::unordered_map<class CStaticMesh*, std::vector<DirectX::XMFLOAT4X4>> mInstanceMap;
	
private:
	void objectCleanUp();
	void updateFrameBuffer();
	//void meshGrouping();

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

	void updateInstanceBuffer(const std::vector<DirectX::XMFLOAT4X4>& matrices);

public:
	class CInputContext* getInput() const { return mInput.get(); }
	class CCameraManager* getCameraManager() const { return mCameraMgr.get(); }
	void setInstanceMap(class CStaticMesh* mesh, DirectX::XMFLOAT4X4 world);
	//void setShaderMap(class CStaticMesh* mesh, class CGraphicShader* shader);

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

		mObjectList.push_back(std::move(newObj));

		return newObjPtr;
	}

private:
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
};

