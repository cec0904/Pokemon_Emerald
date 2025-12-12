#pragma once
#include "../Gameinfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	//충돌할 충돌체들을 모아둔다.
	vector<CSharedPtr<class CColliderBase>> mColliderList2D;
	vector<CSharedPtr<class CColliderBase>> mColliderList3D;
	class CCollisionQuadTree* mQuadTree = nullptr;
	class CScene* mScene = nullptr;

	//충돌체 인터벌
	float mInterval = 0.016f;
	float mIntervalTime = 0.f;


public:
	void AddCollider(class CColliderBase* Collider);

public:
	bool Init();
	void Update(float DeltaTime, class CSceneUIManager* UIManager, class CInput* Input);
	void Render();
	void ReturnNodePool();

};

