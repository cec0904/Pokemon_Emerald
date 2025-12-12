#pragma once
#include "SceneComponent.h"

class CColliderBase :
    public CSceneComponent
{
	//CSceneComponent 꺼 기본 복사 
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderBase();
	CColliderBase(const CColliderBase& Com);
	CColliderBase(CColliderBase&& Com);
	virtual ~CColliderBase();

protected:
	//2D인지 3D인지?
	EColliderType mColliderType;
	//충돌모양?
	EColliderShape mColliderShape;
	//충돌정보
	FVector3D mMin;
	FVector3D mMax;

	//충돌 프로파일 
	FCollisionProfile* mProfile = nullptr;
	//충돌 중 여부 
	bool mCollision = false;
	
	//충돌 시점 함수
	//충돌 시작
	//충돌 위치 , 누구랑 충돌했는지
	function<void(const FVector3D&, CColliderBase*)> mCollisionBeginFunc;
	//Begin함수 Object
	class CSceneObject* mBeginObj;

	//충돌 종료 
	function<void(CColliderBase*)> mCollisionEndFunc;
	class CSceneObject* mEndObj;

protected:
	//충돌한 충돌체의 데이터를 가지고 있을 것이다.
	//용도 : 내가 애랑 충돌했니? 탐색
	//충돌했을때 새로운 데이터가 추가가 자주 일어난다.
	unordered_map<CColliderBase*, bool> mCollisionObjects;



	//디버그용 출력 해주기 
#ifdef _DEBUG
	//위치용 상수버퍼 
	class CTransformCBuffer* mTransformCBuffer = nullptr;
	//충돌체 상수버퍼 
	class CColliderCBuffer* mCBuffer = nullptr;
	//메쉬
	CSharedPtr<class CMesh> mMesh;
	//쉐이더 
	CSharedPtr<class CShader> mShader;
	//회전 유무
	bool mEnableRotation = false;

#endif // _DEBUG


public:
	FCollisionProfile* GetProfile() const
	{
		return mProfile;
	}

	EColliderType GetColliderType()
	{
		return mColliderType;
	}

	EColliderShape GetColliderShape()
	{
		return mColliderShape;
	}

	const FVector3D& GetMin()
	{
		return mMin;
	}
	const FVector3D& GetMax()
	{
		return mMax;
	}

public:
	void SetCollisionProfile(const string& Name);
	void CallCollisionBegin(const FVector3D& HitPoint, CColliderBase* Dest);
	void CallCollisionEnd(CColliderBase* Dest);

public:
	// 충돌한 오브젝트를 추가하는 함수
	void  AddCollisionObject(CColliderBase* Collider);
	// 충돌한 오브젝트인지 확인하는 함수
	bool CheckCollisionObject(CColliderBase* Collider);
	// 충돌이 끝났으면 제거하는 함수
	void EraseCollisionObject(CColliderBase* Collider);


public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderBase* Clone();
	virtual void EraseOwner();
public:
	//순수가상함수 
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest) = 0;


public:
	template<typename T>
	void SetCollisionBeginFunc(T* Obj, void(T::* Func)(const FVector3D&, CColliderBase*))
	{
		mBeginObj = Obj;
		mCollisionBeginFunc = bind(Func, Obj, placeholders::_1, placeholders::_2);
	}

	template<typename T>
	void SetCollisionEndFunc(T* Obj, void(T::* Func)(CColliderBase*))
	{
		mEndObj = Obj;
		mCollisionEndFunc = bind(Func, Obj, placeholders::_1);
	}




};

