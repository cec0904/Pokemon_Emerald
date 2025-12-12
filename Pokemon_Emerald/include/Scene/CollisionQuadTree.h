#pragma once

#include "../Gameinfo.h"

#define QUADTREE_DIVISION_COUNT 5
#define QUADTREE_DEPTH_MAX 4

//우리 이진트리 만들어봤잖아요.
//이진 트리 만든다 생각합시다. 다만 노드가 2개였다면
//이제는 노드가 4개일 뿐이다. 

class CCollisionQuadTreeNode
{
	friend class CCollisionQuadTree;

private:
	CCollisionQuadTreeNode();
	~CCollisionQuadTreeNode();

private:
	class CScene* mScene = nullptr;
	CCollisionQuadTree* mTree = nullptr;
	//부모 노드 
	CCollisionQuadTreeNode* mParent = nullptr;
	//자식노드 4개 
	CCollisionQuadTreeNode* mChild[4] = {};
	//노드의 중앙 위치 
	FVector2D mCenter;
	//노드의 길이? 크기
	FVector2D mSize;
	// 트리의 깊이 
	int mDepth = 0;
	// 몇개라면 분할할지 
	int mDivisionCount = QUADTREE_DIVISION_COUNT;
	//해당 노드에 속한 충돌체들 
	vector<CSharedPtr<class CColliderBase>> mColliderList;

#ifdef _DEBUG
	class CTransformCBuffer* mTransformCBuffer;

#endif // _DEBUG



public:
	void AddCollider(class CColliderBase* Collider, vector<CCollisionQuadTreeNode*>& NodePool);
	void CreateChild(vector<CCollisionQuadTreeNode*>& NodePool);
	void Collision(float DeltaTime);
	void ReturnNodePool(vector<CCollisionQuadTreeNode*>& NodePool);
	void Render(class CMesh* Mesh, class CShader* Shader);


private:
	// 충돌체가 이 노드에 속하는지 검사한다. 
	bool IsInCollider(class CColliderBase* Collider);
};


class CCollisionQuadTree
{
	friend class CSceneCollision;

private:
	CCollisionQuadTree();
	~CCollisionQuadTree();

private:
	class CScene* mScene = nullptr;
	CCollisionQuadTreeNode* mRoot = nullptr;
	int mDivisionCount = QUADTREE_DIVISION_COUNT;
	//노드 풀 : 이미 할당된 노드를 재사용하기 위해서
	vector<CCollisionQuadTreeNode*> mNodePool;

	//재귀를 줄이기 위한 노드 충돌 리스트
	//노드내에 2개 이상의 충돌체가 있을 경우에만 충돌 검사를 하겠다
	//노드들을 저장하기 위한 리스트 
	vector<CCollisionQuadTreeNode*> mCollisionNodeList;

public:
	void SetDivisionCount(int Count);
	void AddCollisionNodeList(CCollisionQuadTreeNode* Node)
	{
		mCollisionNodeList.emplace_back(Node);
	}

	void EraseCollisionNodeList(CCollisionQuadTreeNode* Node);

#ifdef _DEBUG
	CSharedPtr<class CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	class CColliderCBuffer* mColliderCBuffer;
#endif // _DEBUG



public:
	bool Init();
	void AddCollider(class CColliderBase* Collider);
	void Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render();
	void ReturnNodePool();
};

