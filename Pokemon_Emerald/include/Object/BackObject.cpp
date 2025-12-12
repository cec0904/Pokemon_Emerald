#include "BackObject.h"

#include "../Component/SpriteComponent.h"

CBackObject::CBackObject()
{
}

CBackObject::CBackObject(const CBackObject& Obj)
{
}

CBackObject::CBackObject(CBackObject&& Obj)
{
}

CBackObject::~CBackObject()
{
}

bool CBackObject::Init()
{
	CSceneObject::Init();

	mRoot = CreateComponent<CSpriteComponent>("BackGround");

	//mRoot->SetMesh("CenterTexRect");
	mRoot->SetTexture("BackGround", TEXT("Texture/Back2.png"), 0);
	mRoot->SetTint(1.f, 1.f, 1.f);
	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetOpacity(1.f);
	mRoot->SetUseTime(true);
	SetRootComponent(mRoot);

	return true;
}
