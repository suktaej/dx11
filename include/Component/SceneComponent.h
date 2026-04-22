#pragma once
#include "Component.h"

class CSceneComponent :
	public CComponent
{
public:
	CSceneComponent(ComponentKey key);
	CSceneComponent(ComponentKey key, const CSceneComponent& other);
	//CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept;
	~CSceneComponent() override;

protected:
	CSceneComponent* mParent = nullptr;
	std::vector<CSceneComponent*> mChildList;

	DirectX::XMFLOAT3 mLocalScale;
	DirectX::XMFLOAT4 mLocalRotation;
	DirectX::XMFLOAT3 mLocalPosition;
	DirectX::XMFLOAT4X4 mLocalMatrix;

	DirectX::XMFLOAT3 mWorldScale;
	DirectX::XMFLOAT4 mWorldRotation;
	DirectX::XMFLOAT3 mWorldPosition;
	DirectX::XMFLOAT4X4 mWorldMatrix;

	bool mIsLocalDirty = true;
	bool mIsWorldDirty = true;

private:
	void updateWorldTransform();
	void invalidateTransform();
	bool isDescendant(CSceneComponent* node);
	//void detachFromParent();
	//DirectX::XMFLOAT3 quaternionToEuler(DirectX::XMVECTOR quat);

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
	//std::unique_ptr<CComponent> cloneHierarchy() const override;

	void addChild(CSceneComponent& child);
	void eraseOwner() override;

public:
#pragma region Local
	void setLocalScale(const DirectX::XMFLOAT3& scale);
	void setLocalScale(const EAxis& axis, const float& scale);
	void setLocalScale(float x, float y, float z);

	void setLocalPosition(const DirectX::XMFLOAT3& position);
	void setLocalPosition(const EAxis& axis, const float& pos);
	void setLocalPosition(float x, float y, float z);

	void setLocalRotation(const DirectX::XMFLOAT3& rotation);
	void setLocalRotation(const DirectX::XMFLOAT4& rotation);
	void setLocalRotation(const EAxis& axis, const float& angle);
	void setLocalRotation(float x, float y, float z);

	void addLocalScale(const EAxis& axis, const float& scale);
	void addLocalPosition(const EAxis& axis, const float& pos);
	void addLocalRotation(const EAxis& axis, const float& angle);

	void addLocalPositionByDirection(const DirectX::XMFLOAT3& dir, const float& dist);
#pragma endregion Local

#pragma region World
	void setWorldScale(const DirectX::XMFLOAT3& scale);
	void setWorldScale(const EAxis& axis, const float& scale);
	void setWorldScale(float x, float y, float z);

	void setWorldPosition(const DirectX::XMFLOAT3& position);
	void setWorldPosition(float x, float y, float z);
	void setWorldPosition(const EAxis& axis, const float& pos);

	void setWorldRotation(const DirectX::XMFLOAT3& rotation);
	void setWorldRotation(const DirectX::XMFLOAT4& rotation);
	void setWorldRotation(const EAxis& axis, const float& angle);
	void setWorldRotation(float x, float y, float z);

	void addForwardVector(const float& pos);
	void addRightVector(const float& pos);
	void addUpVector(const float& pos);

	void addWorldScale(const EAxis& axis, const float& scale);

	void addWorldPosition(const EAxis& axis, const float& pos);
	void addWorldPosition(const DirectX::XMFLOAT3& pos);

	void addWorldRotation(const EAxis& axis, const float& angle);
	void addWorldRotation(const DirectX::XMFLOAT3& rotation);
	void addWorldRotation(float x, float y, float z);
#pragma endregion World


#pragma region Getter
	const DirectX::XMFLOAT3& getLocalScale() { return mLocalScale; }
	const DirectX::XMFLOAT4& getLocalRotation() { return mLocalRotation; }
	const DirectX::XMFLOAT3& getLocalPosition() { return mLocalPosition; }
	
	const DirectX::XMFLOAT4X4& getWorldMatrix();
	const DirectX::XMFLOAT3 getWorldScale();
	const DirectX::XMFLOAT3 getWorldPosition();
	const DirectX::XMFLOAT4 getWorldRotation();

	const DirectX::XMFLOAT3 getVector(DirectX::XMFLOAT3 base);
	const DirectX::XMFLOAT3 getForwardVector();
	const DirectX::XMFLOAT3 getRightVector();
	const DirectX::XMFLOAT3 getUpVector();
#pragma endregion Getter
	//void setLocalRotation(EAxis axis, const float& angle);
	//void setWorldRotation(EAxis axis, const float& rotation);

private:
	template<typename Func, typename... Args>
	void processChildren(Func func, Args&&... args)
	{
		auto it = mChildList.begin();
		while (it != mChildList.end())
		{
			CSceneComponent* child = *it;

			if (!child->isActive())
			{
				// TODO : Erase-Remove Idiom
				it = mChildList.erase(it);
				continue;
			}

			if (!child->isEnable())
			{
				++it;
				continue;
			}

			(child->*func)(std::forward<Args>(args)...);

			++it;
		}
	}
};
