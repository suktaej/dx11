#pragma once
#include "MeshComponent.h"
class CStaticMeshComponent :
    public CMeshComponent
{
public:
	CStaticMeshComponent(ComponentKey key);
	CStaticMeshComponent(ComponentKey key, const CStaticMeshComponent& other);
	//CMeshComponent(ComponentKey key, CMeshComponent&& other) noexcept;
	~CStaticMeshComponent() override;

protected:
	class CStaticMesh* mMesh = nullptr;
	class CShader* mShader = nullptr;

public:
	void setShader(const std::string& name);
	void setShader(class CShader& shader);
	void setMesh(const std::string& name);
	void setMesh(class CMesh& mesh);
	CStaticMesh* getMesh() const { return mMesh; }
	CShader* getShader() const { return mShader; }

//private:
	void registMap();

public:
	bool init(class CObject* obj) override;
	bool init(class CObject* obj,const char* name) override;
	void preUpdate(float dt) override;
	void update(float dt) override;
	void postUpdate(float dt) override;
	void collision(float dt) override;
	void preRender() override;
	void render() override;
	void postRender() override;
	std::unique_ptr<CComponent> clone() const override;
};

