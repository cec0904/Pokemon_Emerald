#include "Pokemon.h"
#include "../../Component/SpriteComponent.h"
#include "PokemonManager.h"
#include "../../Asset/Animation/Animation2DManager.h"
#include "../../Animation/Animation2D.h"

CPokemon::CPokemon()
{
}

CPokemon::CPokemon(const CPokemon& Obj)
	: CSceneObject(Obj)
{
}

CPokemon::CPokemon(CPokemon&& Obj)
	: CSceneObject(Obj)
{
}

CPokemon::~CPokemon()
{
}

bool CPokemon::Init()
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	

	return true;
}

void CPokemon::Update(float DeltaTime)
{

}

bool CPokemon::Init(int ID)
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	// 1.매니저의 Map 에 직접 접근(find 사용)
	const auto& idMap = CPokemonManager::GetInst()->GetIDMap();
	const auto& posMap = CPokemonManager::GetInst()->GetPosMap();

	auto iterInfo = idMap.find(ID);
	auto iterPos = posMap.find(ID);

	// 2. 데이터가 있다면 내 변수(mInfo)에 복사해서 넣기 (생성 단계)
	if (iterInfo != idMap.end() && iterPos != posMap.end())
	{
		mInfo.Info = iterInfo->second;   // 이름, 타입 등 복사
		mInfo.ImageInfo = iterPos->second; // CSV 좌표 복사
	}
	else { return false; } // 없는 ID면 생성 취소

	// 3. 컴포넌트 생성 및 렌더링 세팅
	mSprite = CreateComponent<CSpriteComponent>();
	SetRootComponent(mSprite);
	mSprite->SetTexture("PokemonAtlas", TEXT("Texture/Pokemon/PokemonSheet.png"));

	mAnimation = mSprite->CreateAnimation2D<CAnimation2D>();
	mAnimation->AddSequence("BackIdle", 1.0f, 1.0f, true);

	// Manager에서 가져온 좌표로 프레임 등록
	mAnimation->AddBrushFrame("BackIdle", mInfo.ImageInfo.Back1, FVector2D(IMAGE_SIZE, IMAGE_SIZE));

	return true;
}

void CPokemon::SetPokemonName(const std::string& Name)
{
	mName = Name;
	mInfo.Info.Name = Name;
}
