#pragma once
#include "Component.h"

class CSceneComponent :
	public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& Com);
	CSceneComponent(CSceneComponent&& Com);
	virtual ~CSceneComponent();

protected:
	CSceneComponent* mParent = nullptr;
	vector<CSharedPtr<CSceneComponent>>	mChildList;
	//레이어 이름
	string mRenderLayerName;
	//해당 SceneComponent가 그려질지 여부
	EComponentRender mRenderType = EComponentRender::None;

public:
	void AddChild(CSceneComponent* Child);

	const string& GetRenderLayerName() const
	{
		return mRenderLayerName;
	}

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
	virtual CSceneComponent* Clone();
	virtual void EraseOwner();

	//////
	//Trnasform 위치 관련
protected:
	class CTransformCBuffer* mTransformCBuffer;

	// 상대 크기 회전 위치
	FVector3D mRelativeScale = FVector3D(1.f, 1.f, 1.f);
	FVector3D mRelativeRot;
	FVector3D mRelativePos;

	// 절대 크기 회전 위치
	FVector3D mWorldScale = FVector3D(1.f, 1.f, 1.f);;
	FVector3D mWorldRot;
	FVector3D mWorldPos;

	//Pivot
	FVector3D mPivot;


	// 변환 행렬
	FMatrix		mmatScale;
	FMatrix		mmatRot;
	FMatrix		mmatTranslate;
	FMatrix		mmatWorld;

	//cn
	FVector3D	mAxis[EAxis::End] =
	{
		FVector3D(1.f, 0.f, 0.f),
		FVector3D(0.f, 1.f, 0.f),
		FVector3D(0.f, 0.f, 1.f)
	};

public:
	//get
	// 9일차 추가. 
	const FMatrix& GetScaleMatrix()	const
	{
		return mmatScale;
	}

	const FMatrix& GetRotationMatrix()	const
	{
		return mmatRot;
	}

	const FMatrix& GetTranslateMatrix()	const
	{
		return mmatTranslate;
	}

	const FMatrix& GetWorldMatrix()	const
	{
		return mmatWorld;
	}

	const FVector3D& GetAxis(EAxis::Type Axis)
	{
		return mAxis[Axis];
	}

	const FVector3D& GetRelativeScale()	const
	{
		return mRelativeScale;
	}

	const FVector3D& GetRelativeRotation()	const
	{
		return mRelativeRot;
	}

	const FVector3D& GetRelativePosition()	const
	{
		return mRelativePos;
	}

	const FVector3D& GetWorldScale()	const
	{
		return mWorldScale;
	}

	const FVector3D& GetWorldRotation()	const
	{
		return mWorldRot;
	}

	const FVector3D& GetWorldPosition()	const
	{
		return mWorldPos;
	}

	//18일차 Pivot
	const FVector3D&  GetPivot() const
	{
		return mPivot;
	}

public:
	void SetPivot(const FVector3D& Pivot)
	{
		mPivot = Pivot;
	}

	void SetPivot(const FVector2D& Pivot)
	{
		mPivot.x = Pivot.x;
		mPivot.y = Pivot.y;
	}

	void SetPivot(float x, float y, float z)
	{
		mPivot.x = x;
		mPivot.y = y;
		mPivot.z = z;
	}

	void SetPivot(float x, float y)
	{
		mPivot.x = x;
		mPivot.y = y;
	}

public:
	//set
	// 상대좌표 
	void SetRelativeScale(const FVector3D& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeScale(const FVector2D& Scale);
	void SetRelativeScale(float x, float y);

	void SetRelativeRotation(const FVector3D& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotation(const FVector2D& Rot);
	void SetRelativeRotation(float x, float y);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativeRotationAxis(float Angle, const FVector3D& Axis);

	void SetRelativePos(const FVector3D& Pos);
	void SetRelativePos(float x, float y, float z);
	void SetRelativePos(const FVector2D& Pos);
	void SetRelativePos(float x, float y);

	// 월드 좌표 
	void SetWorldScale(const FVector3D& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScale(const FVector2D& Scale);
	void SetWorldScale(float x, float y);

	void SetWorldRotation(const FVector3D& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotation(const FVector2D& Rot);
	void SetWorldRotation(float x, float y);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldRotationAxis(float Angle, const FVector3D& Axis);

	void SetWorldPos(const FVector3D& Pos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPos(const FVector2D& Pos);
	void SetWorldPos(float x, float y);

	//추가 함수
public:
	void AddRelativeScale(const FVector3D& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeScale(const FVector2D& Scale);
	void AddRelativeScale(float x, float y);

	void AddRelativeRotation(const FVector3D& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotation(const FVector2D& Rot);
	void AddRelativeRotation(float x, float y);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

	void AddRelativePos(const FVector3D& Pos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativePos(const FVector2D& Pos);
	void AddRelativePos(float x, float y);

	void AddWorldScale(const FVector3D& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldScale(const FVector2D& Scale);
	void AddWorldScale(float x, float y);

	void AddWorldRotation(const FVector3D& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotation(const FVector2D& Rot);
	void AddWorldRotation(float x, float y);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);

	void AddWorldPos(const FVector3D& Pos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldPos(const FVector2D& Pos);
	void AddWorldPos(float x, float y);



private:
	//
	void ComputeTransform();
};

