#include "SceneCollision.h"
#include "../Component/ColliderBase.h"
#include "CollisionQuadTree.h"
#include "../Share/Log.h"
#include "Input.h"
#include "SceneUIManager.h"

CSceneCollision::CSceneCollision()
{
}

CSceneCollision::~CSceneCollision()
{
	SAFE_DELETE(mQuadTree);
}

void CSceneCollision::AddCollider(CColliderBase* Collider)
{
	if (Collider->GetColliderType() == EColliderType::Collider2D)
	{
		mColliderList2D.emplace_back(Collider);
	}
	else
	{
		mColliderList3D.emplace_back(Collider);
	}
}

bool CSceneCollision::Init()
{
	mQuadTree = new CCollisionQuadTree;


	mQuadTree->mScene = mScene;

	if (!mQuadTree->Init())
	{
		return false;
	}

	return true;
}

void CSceneCollision::Update(float DeltaTime, class CSceneUIManager* UIManager, class CInput* Input)
{
	//mInterval 시간을 지정해야만 인터벌을 사용할 수 있게 하겠다. 
	if (mInterval > 0.f)
	{
		mIntervalTime += DeltaTime;

		if (mIntervalTime < mInterval)
		{
			return;
		}

		mIntervalTime -= mInterval;
	}

	//CLog::PrintLog("Collision Update");

	//쿼드트리를 이용해서 카메라 자표를 먼저 갱신해야 한다.
	mQuadTree->Update(DeltaTime);


	size_t Size = mColliderList2D.size();

	for (size_t i = 0; i < Size;)
	{
		if (!mColliderList2D[i]->IsActive())
		{
			//그냥 덮어씌우기 
			if (i < Size - 1)
			{
				mColliderList2D[i] = mColliderList2D[Size - 1];
			}
			mColliderList2D.pop_back();
			--Size;
			continue;
		}
		else if (!mColliderList2D[i]->IsEnable())
		{
			++i;
			continue;
		}

		//쿼드트리 만들고나서 
		//여기에 쿼드트리에 등록 
		mQuadTree->AddCollider(mColliderList2D[i]);

		++i;
	}


	/////////////
	//여기서 UI 충돌 검사를 할것이다. 
	FVector2D MousePos = Input->GetMousePos();

	UIManager->CollisionMouse(DeltaTime, MousePos);

	/////////////

	//충돌 검사 
	mQuadTree->Collision(DeltaTime);
}


void CSceneCollision::Render()
{
	mQuadTree->Render();
}

void CSceneCollision::ReturnNodePool()
{
	mQuadTree->ReturnNodePool();
}