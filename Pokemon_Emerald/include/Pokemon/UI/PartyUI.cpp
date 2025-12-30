#include "PartyUI.h"
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
#include "../../Pokemon/Pokemon/PartyData.h"



CPartyUI::CPartyUI()
{
}

CPartyUI::~CPartyUI()
{
}

void CPartyUI::ChangePokemon(int index)
{
	if (!mPartyData)
	{
		return;
	}
	mPartyData->ChangePokemon(index);
}

const FPokemonInstance& CPartyUI::GetActivePokemon() const
{
	static FPokemonInstance Dummy;
	if (!mPartyData)
	{
		return Dummy;
	}

	return mPartyData->GetActivePokemon();
}

const vector<FPokemonInstance>& CPartyUI::GetPartyList() const
{
	static vector<FPokemonInstance> Empty;
	if (!mPartyData)
	{
		return Empty;
	}
	return mPartyData->GetPartyList();
}


bool CPartyUI::Init()
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