#include "CollisionQuadTree.h"

#include "../Component/ColliderBase.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Device.h"

#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Shader/ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Shader/TransformCBuffer.h"
#include "../Scene/CameraManager.h"

#include "../Shader/ColliderCBuffer.h"

CCollisionQuadTreeNode::CCollisionQuadTreeNode()
{
#ifdef _DEBUG
	mTransformCBuffer = new CTransformCBuffer;

	mTransformCBuffer->Init();

#endif // _DEBUG

}

CCollisionQuadTreeNode::~CCollisionQuadTreeNode()
{

#ifdef _DEBUG
	SAFE_DELETE(mTransformCBuffer);
#endif // _DEBUG
	// 자식 노드가 하나라도 있을경우 무조건 4개 분할되어있으니 4개 전부 제거해준다. 
	if (mChild[0])
	{
		SAFE_DELETE(mChild[0]);
		SAFE_DELETE(mChild[1]);
		SAFE_DELETE(mChild[2]);
		SAFE_DELETE(mChild[3]);
	}
}

void CCollisionQuadTreeNode::AddCollider(CColliderBase* Collider, vector<CCollisionQuadTreeNode*>& NodePool)
{
	//들어온 충돌체가 해당 노드에 해당되는지 검사한다. 
	//좌표값이 노드랑 맞는지 검사해야한다. 
	if (!IsInCollider(Collider))
	{
		return;
	}

	//내가 리프노드니?
	//자식 노드가 없을 경우 
	if (!mChild[0])
	{
		//충돌 목록에 추가한다. 
		mColliderList.emplace_back(Collider);

		// 노드가 가지고 있는 충돌체가 2개 이상일 경우 충돌 검사해야한다고 등록시켜준다. 
		// 2개일때만 등록시켜준 이유는 충돌체가 추가 될때마다 해당 노드를 충돌리스트에 한번만 추가하기 위해서
		if (mColliderList.size() == 2)
		{
			mTree->AddCollisionNodeList(this);
		}


		//mColliderList 에 추가했는데 
		//사이즈가 mDivisionCount 갯수에 도달했는가?
		// 조건 1 : 충돌 목록에 추가된 개수가 노드를 분할해야할 개수 이상인지 판단하고
		// 조건 2 :  너무 많이 분할하면 느려질 수 있기 때문에 최대 분할 개수를 정해둔다. 
		if (mColliderList.size() >= mDivisionCount && mDepth < QUADTREE_DEPTH_MAX)
		{
			// 쿼드 트리 분할을 할것이다. 
			//노드 분할 
			CreateChild(NodePool);

			//4개의 노드가 분할되었는데
			// 현재 노드가 가지고 있는 충돌체들을 각각의 Child에게 분배해준다. 
			size_t Size = mColliderList.size();

			for (size_t i = 0; i < Size; ++i)
			{
				// 여러 자식 영역에 겹쳐있는 충돌체가 있을 수 있으므로 
				// 각 자식안에 들어가는지 판단하여 추가.
				for (int j = 0; j < 4; ++j)
				{
					if (mChild[j]->IsInCollider(mColliderList[i]))
					{
						mChild[j]->AddCollider(mColliderList[i], NodePool);
					}
				}
			}

			//자식에게 모든 분배가 끝났기 때문에 충돌 목록을 비워준다. 
			mColliderList.clear();

			//쪼개졌으니까 나는 이제 충돌 검사 리스트에서 제거해줘 
			mTree->EraseCollisionNodeList(this);
		}
	}
	else
	{
		mChild[0]->AddCollider(Collider, NodePool);
		mChild[1]->AddCollider(Collider, NodePool);
		mChild[2]->AddCollider(Collider, NodePool);
		mChild[3]->AddCollider(Collider, NodePool);
	}
}

void CCollisionQuadTreeNode::CreateChild(vector<CCollisionQuadTreeNode*>& NodePool)
{
	if (NodePool.empty())
	{
		// 노드 풀이 비어있을 경우 4개의 자식노드로 분할 한다. 
		NodePool.emplace_back(new CCollisionQuadTreeNode);
		NodePool.emplace_back(new CCollisionQuadTreeNode);
		NodePool.emplace_back(new CCollisionQuadTreeNode);
		NodePool.emplace_back(new CCollisionQuadTreeNode);
	}

	//만약 노드풀이 이미 있다면 위의 if문은 동작하지 않고 아래 코드로 바로 동작!
	// 만약 노드풀이 이미 존재 했다면 그것을 활용할것이다.
	// 가장 뒤의 노드를 꺼내온다. 
	for (int i = 0; i < 4; ++i)
	{
		// 실제 사용되는 노드로 빠졌기 때문에 노드풀의 가장 뒤의 노드를 지운다. 
		mChild[i] = NodePool.back();
		NodePool.pop_back();

		//모든 자식노드에게 어떤 트리의 노드인지 알려준다.
		mChild[i]->mTree = mTree;

		mChild[i]->mScene = mScene;

		// 자식노드의 크기는 현재 노드의 절반 크기를 지정해준다. 
		mChild[i]->mSize = mSize * 0.5;

		// Center 위치값 구하기 
		// 4사분면 0, 1사분면 1, 2사분면 3, 3사분면 2
		// X 좌표 : 짝수냐 홀수냐 구분해
		// Y 좌표 : 0,1냐 2,3이냐 구분해줄것이다. 
		//X좌표 왼쪽 오른쪽
		if (i % 2 == 0)
		{
			mChild[i]->mCenter.x = mCenter.x - mSize.x * 0.25f;
		}
		else
		{
			mChild[i]->mCenter.x = mCenter.x + mSize.x * 0.25f;
		}

		// Y좌표  0 1 | 2 3 
		if (i <= 1)
		{
			mChild[i]->mCenter.y = mCenter.y + mSize.y * 0.25f;
		}
		else
		{
			mChild[i]->mCenter.y = mCenter.y - mSize.y * 0.25f;
		}

		mChild[i]->mParent = this;
		mChild[i]->mDepth = mDepth + 1;
	}
}

//함수명 고려
// 노드에 해당 충돌체가 포함되어있니?
bool CCollisionQuadTreeNode::IsInCollider(CColliderBase* Collider)
{
	//충돌체 
	FVector2D Min, Max;
	Min.x = Collider->GetMin().x;
	Min.y = Collider->GetMin().y;

	Max.x = Collider->GetMax().x;
	Max.y = Collider->GetMax().y;

	//노드
	FVector2D NodeMin, NodeMax;
	NodeMin = mCenter - mSize * 0.5f;
	NodeMax = mCenter + mSize * 0.5f;

	//좌표
	if (Min.x > NodeMax.x)
	{
		// 충돌체 왼쪽이 노드의 오른쪽 보다 크다.
		return false;
	}
	else if (Max.x < NodeMin.x)
	{
		// 충돌체 오른쪽이 노드의 왼쪽보다 작다 
		return false;
	}
	else if (Min.y > NodeMax.y)
	{
		// 충돌체의 아래가 노드의 위보다 크다.
		return false;
	}
	else if (Max.y < NodeMin.y)
	{
		// 충돌체의 위가 노드의 아래보다 작다.
		return false;
	}

	//겹침이 발생
	return true;
}

void CCollisionQuadTreeNode::Collision(float DeltaTime)
{
	//충돌간의 검사를 해주면 된다.
	//mColliderList 순회하면서 나 충돌 했니? 검사할거다. 

	size_t Size = mColliderList.size();

	for (size_t i = 0; i < Size;)
	{
		CColliderBase* Src = mColliderList[i];

		if (!mColliderList[i]->IsActive())
		{
			if (i < Size - 1)
			{
				//mColliderList[i] = mColliderList.back();
				mColliderList[i] = mColliderList[Size - 1];
			}
			mColliderList.pop_back();
			--Size;
			continue;
		}
		else if (!mColliderList[i]->IsEnable())
		{
			++i;
			continue;
		}

		//i의 프로파일을 지역변수로 가져온다. 
		FCollisionProfile* SrcProfile = mColliderList[i]->GetProfile();

		if (!SrcProfile->Enable)
		{
			++i;
			continue;
		}


		for (size_t j = i + 1; j < Size;)
		{
			CColliderBase* Dest = mColliderList[j];
			if (!mColliderList[j]->IsActive())
			{
				if (j < Size - 1)
				{
					mColliderList[j] = mColliderList[Size - 1];
				}
				mColliderList.pop_back();
				--Size;
				continue;
			}
			else if (!mColliderList[j]->IsEnable())
			{
				++j;
				continue;
			}

			//i의 프로파일을 지역변수로 가져온다. 
			FCollisionProfile* DestProfile = mColliderList[j]->GetProfile();

			if (!DestProfile->Enable)
			{
				++j;
				continue;
			}
			
			//먼저 프로파일이 서로 충돌인지 검사한다. 
			if (SrcProfile->Interaction[DestProfile->Channel] == ECollisionInteraction::Ignore ||
				DestProfile->Interaction[SrcProfile->Channel] == ECollisionInteraction::Ignore)
			{
				++j;
				continue;
			}

			//충돌 했는지 여부를 검사할것이다.
			FVector3D HitPoint;

			//쿼드트리노드에서 내가 충돌했는지 검사한다. 
			if (mColliderList[i]->Collision(HitPoint, mColliderList[j]))
			{
				//둘이 충돌 되었다.
				//Src->함수 호출;
				//처음인지 검사해주자
				if (!Src->CheckCollisionObject(Dest))
				{
					Src->CallCollisionBegin(HitPoint, Dest);
					//Dest->함수 호출;
					Dest->CallCollisionBegin(HitPoint, Src);
				}
				else
				{
					//오버렙중!
				}
			}
			else if (Src->CheckCollisionObject(Dest))// 충돌을 안했을때 
			{
				//두 충돌체가 현재 충돌이 안됬을 경우
				// 이전에 충돌하고있는지 확인하고
				//충돌하고 있었으면 CollisionEnd 함수를 호출해준다.

				Src->CallCollisionEnd(Dest);
				Dest->CallCollisionEnd(Src);
			}

			++j;
		}
		++i;
	}



}

void CCollisionQuadTreeNode::ReturnNodePool(vector<CCollisionQuadTreeNode*>& NodePool)
{
	mColliderList.clear();

	if (mChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			NodePool.emplace_back(mChild[i]);
			mChild[i]->ReturnNodePool(NodePool);
			mChild[i] = nullptr;
		}
	}

}

void CCollisionQuadTreeNode::Render(CMesh* Mesh, CShader* Shader)
{
#ifdef _DEBUG

	//크기, 위치
	FMatrix matScale, matTranslate, matWorld;
	matScale.Scaling(mSize);
	matTranslate.Translation(mCenter);

	// 곱하는 순서 정해져있다. 조심
	//크자이공부 !!!!!
	matWorld = matScale * matTranslate;
	
	FMatrix matView, matProj;
	matView = mScene->GetCameraManager()->GetViewMatrix();
	matProj = mScene->GetCameraManager()->GetProjMatrix();

	mTransformCBuffer->SetWorldMatrix(matWorld);
	mTransformCBuffer->SetViewMatrix(matView);
	mTransformCBuffer->SetProjMatrix(matProj);

	mTransformCBuffer->UpdateBuffer();

	Shader->SetShader();

	Mesh->Render();

	if (mChild[0])
	{
		mChild[0]->Render(Mesh, Shader);
		mChild[1]->Render(Mesh, Shader);
		mChild[2]->Render(Mesh, Shader);
		mChild[3]->Render(Mesh, Shader);
	}

#endif // _DEBUG
}


//쿼드 트리 
CCollisionQuadTree::CCollisionQuadTree()
{
}

CCollisionQuadTree::~CCollisionQuadTree()
{
	SAFE_DELETE(mColliderCBuffer);

	size_t Size = mNodePool.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mNodePool[i]);
	}


	SAFE_DELETE(mRoot);
}

void CCollisionQuadTree::SetDivisionCount(int Count)
{
	mDivisionCount = Count;
}

void CCollisionQuadTree::EraseCollisionNodeList(CCollisionQuadTreeNode* Node)
{
	auto iter = mCollisionNodeList.begin();
	auto iterEnd = mCollisionNodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Node)
		{
			mCollisionNodeList.erase(iter);
			break;
		}
	}
}

bool CCollisionQuadTree::Init()
{

#ifdef _DEBUG
	mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameCenterRect");

	mShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

	mColliderCBuffer = new CColliderCBuffer;

	mColliderCBuffer->Init();

	mColliderCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);

#endif // _DEBUG




	//루트노드 만들어주기 
	mRoot = new CCollisionQuadTreeNode;

	mRoot->mScene = mScene;

	//해상도 가져오기 
	// 쿼드 트리의 노드는 해상도보다 좀더 넓게 검사해야한다. 
	// 왜냐면 화면에 짤리는 오브젝트 들도 충돌하는지 검사해야 하기 때문이다.
	const FResolution& RS = CDevice::GetInst()->GetResolution();
	mRoot->mSize.x = RS.Width * 1.5f;
	mRoot->mSize.y = RS.Height * 1.5f;
	mRoot->mDivisionCount = mDivisionCount;
	mRoot->mTree = this;

	return true;
}

void CCollisionQuadTree::AddCollider(CColliderBase* Collider)
{
	mRoot->AddCollider(Collider, mNodePool);
}


void CCollisionQuadTree::Update(float DeltaTime)
{
	// 카메라를 얻어와서 카메라의 위치로 루트의 센터
	CCameraComponent* Camera = mScene->GetCameraManager()->GetViewTarget();

	// 카메라를 얻어와서 카메라의 위치로 루트의 센터를 지정해준다. 
	// 현재 중앙은 카메라 기준이기 때문이다. 
	if (Camera)
	{
		mRoot->mCenter.x = Camera->GetWorldPosition().x;
		mRoot->mCenter.y = Camera->GetWorldPosition().y;
	}
}

void CCollisionQuadTree::Collision(float DeltaTime)
{
	// 노드를 순회하면서 충돌을 호출 
	auto iter = mCollisionNodeList.begin();
	auto iterEnd = mCollisionNodeList.end();

	for (; iter != iterEnd; ++iter) 
	{
		(*iter)->Collision(DeltaTime);
	}

	// 충돌검사 후 충돌 리스트 제거
	mCollisionNodeList.clear();

	//// 노드 회수 노드풀에 다시 돌려놓는다. 
	//mRoot->ReturnNodePool(mNodePool);
}

void CCollisionQuadTree::Render()
{

#ifdef _DEBUG
	mColliderCBuffer->UpdateBuffer();

	mRoot->Render(mMesh, mShader);
#endif // _DEBUG

}

void CCollisionQuadTree::ReturnNodePool()
{
	mRoot->ReturnNodePool(mNodePool);
}
