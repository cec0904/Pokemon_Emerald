#include "TornadoBullet.h"

#include "../Component/StaticMeshComponent.h"

CTornadoBullet::CTornadoBullet()
	: CSceneObject()
{
}

CTornadoBullet::CTornadoBullet(const CTornadoBullet& Obj)
	: CSceneObject(Obj)
{
}

CTornadoBullet::CTornadoBullet(CTornadoBullet&& Obj)
	: CSceneObject(Obj)
{
}

CTornadoBullet::~CTornadoBullet()
{
}

bool CTornadoBullet::Init()
{
	CSceneObject::Init();

	mRoot	= CreateComponent<CSceneComponent>();
	mPivot	= CreateComponent<CSceneComponent>();
	mMesh = CreateComponent<CStaticMeshComponent>();

	SetRootComponent(mRoot);
	mRoot->AddChild(mPivot);
	mPivot->AddChild(mMesh);

	mMesh->SetMesh("CenterRect");
	mMesh->SetShader("ColorMeshShader");
	mMesh->SetWorldScale(0.5f,0.5f, 1.f);
	mMesh->SetRelativePos(1.f, 0.f);

	return true;
}


void CTornadoBullet::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
	//이동
	FVector3D Pos = mRoot->GetWorldPosition();

	Pos += mRoot->GetAxis(EAxis::Y) * mSpeed * DeltaTime;

	mRoot->SetWorldPos(Pos);
	//회전 
	FVector3D Rot = mPivot->GetWorldRotation();

	Rot.z += 360.f * DeltaTime;

	mPivot->SetWorldRotationZ(Rot.z);
}
