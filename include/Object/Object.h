#pragma once
#include "../GameInfo.h"

class CObject
{
	//friend struct std::default_delete<CObject>;
public:
	class SceneKey
	{
		friend class CScene;
	private:
		SceneKey() {}
	public:
		SceneKey(const SceneKey& key) {}
	};

public:
	// passkey 패턴을 사용하여 호출 클래스에서만 해당 객체를 생성할 수 있도록 함
	// passkey가 없으면 컴파일 에러가 발생하므로, 외부에서 CObject를 직접 생성할 수 없음
	// 외부에서 생성할 수 없으므로 소멸자 역시 public으로 선언
	// 단, unique_ptr로 관리 시, 엄격한 소멸자 제어를 하려면 
	// 소멸자도 private으로 선언하고 std::default_delete를 friend로 지정
	CObject(SceneKey key);
	CObject(SceneKey key,const CObject& other);
	CObject(SceneKey key,CObject&& other) noexcept;
	virtual ~CObject();

protected:
	class CScene* mScene = nullptr;
	std::string mName;

public:
	virtual bool init();
	virtual bool init(const char* filePath);
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
	class CScene* getScene() const { return mScene; }
	void setName(const std::string& name) { mName = name; }
	const char* getName() const { return mName.c_str(); }
};

