#include "NpcProfessor.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Object/PlayerObject.h"
#include "../../Component/StaticMeshComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Animation/Animation2D.h"
#include "../UI/StartPokemonUI.h"

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
        return false;

    SetTarget(mScene->FindObjectFromType<CPlayerObject>());

    mScene->GetInput()->AddBindKey("ProfessorInteract", 'D');
    mScene->GetInput()->AddBindFunction("ProfessorInteract", EInputType::Down, this, &CNpcProfessor::Interact);

    return true;
}

void CNpcProfessor::Update(float DeltaTime)
{
    CNpcBase::Update(DeltaTime);
}

void CNpcProfessor::Interact(float DeltaTime)
{
    if (mStartOnce)
        return;

    CPlayerObject* Player = mScene->FindObjectFromType<CPlayerObject>();
    if (!Player)
        return;

    FVector3D P = Player->GetWorldPosition();
    FVector3D N = GetWorldPosition();

    float dx = P.x - N.x;
    float dy = P.y - N.y;

    const float Range = 100.f;
    if (dx * dx + dy * dy > Range * Range)
        return;



    Player->OpenStartSelectUI();
    mStartOnce = true;

}

