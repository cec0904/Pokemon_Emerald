#pragma once
#include "SceneComponent.h"

//Mesh 계열을 출력해주기 위한 컴포넌트다 .
class CMeshComponent :
    public CSceneComponent
{
	//씬이랑 오브젝트 모두 사용할거같다.
	friend class CScene;
	friend class CSceneObject;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& Com);
	CMeshComponent(CMeshComponent&& Com);
	virtual ~CMeshComponent();

protected:
	//class CTransformCBuffer* mTransformCBuffer;

	//머티리얼 정보를 임시 버퍼처럼 사용할 컨테이너
	vector<CSharedPtr<class CMaterial>> mMaterialSlots;

	//시점 함수 
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
	virtual CMeshComponent* Clone();

};

