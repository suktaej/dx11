#pragma once
#include "../../GameInfo.h"

class CMeshManager
{
	friend class CAssetManager;
	friend std::default_delete<CMeshManager>;

private:
	CMeshManager();
	~CMeshManager();

private:
	class CDeviceManager* mDeviceMgr = nullptr;
	std::unordered_map<std::string, std::unique_ptr<class CMesh>> mMeshMap;

private:
    bool init(class CDeviceManager& device);

public:
	class CMesh* findMesh(const std::string& name);
    void testCode();

private:
    template <typename T>
    bool createMesh(const FMeshDesc& desc)
    {
        static_assert(std::is_base_of<CMesh, T>::value, "T must inherit from CMesh");

        if (findMesh(desc.Name))
            return true;

        auto mesh = std::make_unique<T>();
		//std::unique_ptr<T> mesh(new T(typename T::MeshKey{}));
        mesh->setName(desc.Name);

        if (!mesh->createMesh(
            desc.device,
            desc.bKeepVertexData, (void*)desc.pVertexData, desc.VertexSize, desc.VertexCount, desc.VertexUsage,
            desc.Primitive,
            desc.bKeepIndexData, (void*)desc.pIndexData, desc.IndexSize, desc.IndexCount, desc.IndexFormat, desc.IndexUsage))
            return false;

        mMeshMap.insert(std::make_pair(desc.Name, std::move(mesh)));
        return true;
    }
};
