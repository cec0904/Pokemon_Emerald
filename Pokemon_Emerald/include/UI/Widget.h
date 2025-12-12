#pragma once

#include "../Share/Object.h"

#include "UIinfo.h"
#include "../Object/SceneObject.h"

class CWidget abstract :
	public CObject
{
	friend class CSceneUIManager;

protected:
	CWidget();
	virtual ~CWidget();

	// UI 전용 투영행렬을 만들기
protected:
	static FMatrix mUIProj;

public:
	static void CreateUIProjection(float Width, float Height, float ViewDist)
	{
		// 직교투영 
		mUIProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, Width, 0, Height, 0, ViewDist);
	}

protected:
	//UI가 포함된 Scene
	class CScene* mScene = nullptr;

	//부모 위젯 포인터
	CWidget* mParent = nullptr;

	// 해당 위젯을 들고있는 오브젝트
	CSharedPtr<CSceneObject> mOwnerObject;

	//화면에 그려질 쉐이더 
	CSharedPtr<class CShader> mShader;
	CSharedPtr<class CMesh>     mMesh;
	class CUICBuffer* mUICBuffer = nullptr;
	class CTransformCBuffer* mTransformCBuffer = nullptr;

	string mName;
	
	FVector2D mPos;			// 상대적인 위치
	FVector2D mRenderPos;	// 최종 출력 위치


	//사이즈,
	FVector2D mSize;
	//피봇
	FVector2D mPivot;
	//Z오더
	int mZOrder = 0;
	// 로테이션
	float mRotation = 0.f;

	//마우스가 들어와있나 
	bool mMouseOn = false;

public:
	void SetParent(CWidget* Widget)
	{
		mParent = Widget;
	}

	void SetOwnerObject(class CSceneObject* Object)
	{
		mOwnerObject = Object;
	}

public:
	const string& GetName() const
	{
		return mName;
	}

	const FVector2D& GetPos() const
	{
		return mPos;
	}

	const FVector2D& GetRenderPos() const
	{
		return mRenderPos;
	}

	const FVector2D& GetSize() const
	{
		return mSize;
	}

	const FVector2D& GetPivot() const
	{
		return mPivot;
	}

	int GetZOrder() const
	{
		return mZOrder;
	}

	float GetRotation() const
	{
		return mRotation;
	}

public:
	void SetPos(const FVector2D& Pos)
	{
		mPos = Pos;
	}

	void SetRenderPos(const FVector2D& Pos)
	{
		mRenderPos = Pos;
	}

	void SetPos(float X, float Y)
	{
		mPos.x = X;
		mPos.y = Y;
	}

	virtual void SetSize(const FVector2D& Size)
	{
		mSize = Size;
	}

	virtual void SetSize(float X, float Y)
	{
		mSize.x = X;
		mSize.y = Y;
	}

	void SetPivot(const FVector2D& Pivot)
	{
		mPivot = Pivot;
	}

	void SetZOrder(int ZOrder)
	{
		mZOrder = ZOrder;
	}

	void SetRotation(float Rotation)
	{
		//360 보다 큰경우
		if (Rotation > 360.f)
		{
			//365.5f
			//정수부분
			int Angle1 = static_cast<int>(Rotation);
			//소수부분
			float Angle2 = Rotation - Angle1;

			//정수부분을 0 ~ 360도 사이로 만들고싶어.
			Angle1 %= 360;

			mRotation = Angle1 + Angle2;
		}
		else if (Rotation < 0.f)
		{
			Rotation *= -1;

			//정수부분
			int Angle1 = static_cast<int>(Rotation);
			//소수부분
			float Angle2 = Rotation - Angle1;

			//정수부분을 0 ~ 360도 사이로 만들고싶어.
			Angle1 %= 360;

			mRotation = 360.f - (Angle1 + Angle2);
		}

		mRotation = Rotation;
	}

public:
	//쉐이더 셋팅 
	void SetShader(const string& Name);
	void SetShader(class CShader* Shader);


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);
	virtual void EndFrame();

	// 호버가 시작되었다.
	virtual void MouseHovered();
	// 호버가 끝났다. 
	virtual void MouseUnHovered();

};

