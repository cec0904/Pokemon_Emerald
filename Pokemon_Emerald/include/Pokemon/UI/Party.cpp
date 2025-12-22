#include "Party.h"
#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../../UI/Common/Button.h"
#include "../../UI/Common/Image.h"
#include "../../Share/Log.h"
#include "../../Asset/Asset.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Font/Font.h"
#include "../../Asset/Font/FontManager.h"
#include "../../UI/Common/TextBlock.h"
#include "../../UI/UserWidget/UserWidget.h"
#include "../../UI/Widget.h"
#include "../../Pokemon/Pokemon/Pokemon.h"
#include "../../Pokemon/Pokemon/PokemonManager.h"


CParty::CParty()
{
}

CParty::~CParty()
{
}

bool CParty::AddPokemon(int id, int level)
{
	if (mPokemonParty.size() >= 6)
	{
		return false;
	}

	auto& IDMap = CPokemonManager::GetInst()->GetIDMap();
	auto& StatMap = CPokemonManager::GetInst()->GetDefaultStatMap();
	auto& PosMap = CPokemonManager::GetInst()->GetPosMap();

	if (IDMap.find(id) == IDMap.end())
	{
		return false;
	}

	FPokemonInstance newInstance;
	newInstance.SpeciesID = id;
	newInstance.Info = IDMap.at(id);
	newInstance.Level = level;
	newInstance.ImageInfo = PosMap.at(id);

	// 능력치 결정
	FBaseStats base = StatMap.at(id);
	//hp = ((종족값 * 2) * 레벨 / 100) + 10 + 레벨
	newInstance.CurrentState.HP = ((base.HP * 2) * level / 100) + 10 + level;
	newInstance.CurrentHP = newInstance.CurrentState.HP;

	//나머지 능력치 = (종족값 * 2) * 레벨 / 100 + 5
	newInstance.CurrentState.Atk = ((base.Atk * 2) * level / 100) + 5;
	newInstance.CurrentState.Def = ((base.Def * 2) * level / 100) + 5;
	newInstance.CurrentState.SpAtk = ((base.SpAtk * 2) * level / 100) + 5;
	newInstance.CurrentState.SpDef = ((base.SpDef * 2) * level / 100) + 5;
	newInstance.CurrentState.Spd = ((base.Spd * 2) * level / 100) + 5;

	mPokemonParty.push_back(newInstance);
	return true;
}

bool CParty::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	float ScreenW = RS.Width;	// 1280
	float ScreenH = RS.Height;	// 720

	// 배경
	CSharedPtr<CImage> Party = mScene->GetUIManager()->CreateWidget<CImage>("Party");

	Party->SetTexture("PartyBack", TEXT("Texture/Pokemon/Pokemon/Party.png"));
	Party->SetBrushAnimation(true);
	Party->AddBrushFrame(8.f, 24.f, 240.f, 160.f);
	Party->SetSize(960.f, 640);
	Party->SetPivot(FVector2D(0.5f, 0.5f));
	Party->SetPos(ScreenW * 0.5f, ScreenH * 0.5);

	Party->SetZOrder(200);
	AddWidget(Party);

	return true;

}