#include "NpcProfessor.h"
#include "../../Scene/Scene.h"
#include "../../Object/PlayerObject.h"
#include "../../Component/StaticMeshComponent.h"
#include "../../Component/SpriteComponent.h"

CNpcProfessor::CNpcProfessor()
{
}

CNpcProfessor::CNpcProfessor(const CNpcProfessor& Obj)
{
}

CNpcProfessor::CNpcProfessor(CNpcProfessor&& Obj)
{
}

CNpcProfessor::~CNpcProfessor()
{
}

bool CNpcProfessor::Init()
{
	if (!CNpcBase::Init())
	{
		return false;
	}

	mRoot->SetTexture("NpcProfessor");
	mRoot->SetPivot(0.5f, 0.1f);

	SetTarget(mScene->FindObjectFromType<CPlayerObject>());

	return true;
}

void CNpcProfessor::Update(float DeltaTime)
{
	CNpcBase::Update(DeltaTime);
}