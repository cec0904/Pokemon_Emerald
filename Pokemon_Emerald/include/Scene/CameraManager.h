#pragma once

#include "../GameInfo.h"

//Input과 같은시스템이다. 

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	// 
	CSharedPtr<class CCameraComponent> mViewTarget;
	//기본 단위행렬
	FMatrix mIdentity;

public:
	class CCameraComponent* GetViewTarget()	const
	{
		return mViewTarget;
	}

	const FMatrix& GetViewMatrix()	const;

	const FMatrix& GetProjMatrix()	const;

	const FVector3D& GetCameraWorldPos() const;


public:
	void SetViewTarget(class CCameraComponent* ViewTarget);

public:
	bool Init();
	void Update(float DeltaTime);

};

