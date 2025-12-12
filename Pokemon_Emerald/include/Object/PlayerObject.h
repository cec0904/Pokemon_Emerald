#pragma once
#include "SceneObject.h"

enum class ESkill4State
{
	Expansion,	//확장
	Maintain,	//유지
	Reduction	//축소
};

class CPlayerObject :
	public CSceneObject
{
	friend class CScene;

protected:
	CPlayerObject();
	CPlayerObject(const CPlayerObject& Obj);
	CPlayerObject(CPlayerObject&& Obj);
	virtual ~CPlayerObject();

protected:
	//CSharedPtr<class CStaticMeshComponent> mRoot;
	CSharedPtr<class CSpriteComponent> mRoot;
	CSharedPtr<class CColliderAABB2D> mBody;
	//CSharedPtr<class CColliderSphere2D> mBody;
	//CSharedPtr<class CColliderOBB2D> mBody;
	//CSharedPtr<class CColliderLine2D> mLine;

	CSharedPtr<class CSceneComponent> mRotationPivot;
	CSharedPtr<class CSpriteComponent> mSub;
	//MovementComponent
	//RotationComponent
	CSharedPtr<class CMovementComponent> mMovement;
	CSharedPtr<class CRotationComponent> mRotation;

	//CameraComponent
	CSharedPtr<class CCameraComponent> mCamera;

	//애니메이션
	class CAnimation2D* mAnimation = nullptr;

	//Idel 상태값으로 돌아갈건지 여부 
	bool mAutoBasePose = true;

	class CProgressBar* mHPBar;

	class CPedometer* mPedometer;




public:
	virtual bool Init();
	virtual void Update(float DeltaTime);


private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveRight(float DeltaTime);
	void MoveLeft(float DeltaTime);

	bool Block = false;
	void IsOnCollision(const FVector3D& HitPoint, class CColliderBase* Dest);
	void IsOffCollision(class CColliderBase* Dest);




	void rotationZ(float DeltaTime);
	void rotationZInv(float DeltaTime);



	//애니메이션 바인드 함수 
	void AttackEnd();
	void AttackNotify();
};

