#include "MenuUI.h"
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
#include "Inventory.h"
#include "PartyUI.h"
#include "../../Object/PlayerObject.h"
#include "BattleWidget.h"

CMenuUI::CMenuUI()
{
}

CMenuUI::~CMenuUI()
{
}


void CMenuUI::MoveUp()
{
	if (mSelectIndex > 0)
	{
		--mSelectIndex;
		UpdateCursor();
	}
}

void CMenuUI::MoveDown()
{
	if (mSelectIndex < mMaxIndex - 1)
	{
		++mSelectIndex;
		UpdateCursor();
	}
}


void CMenuUI::UpdateCursor()
{
	if (!mCursor) return;

	FResolution RS = CDevice::GetInst()->GetResolution();
	float W = RS.Width;
	float H = RS.Height;

	float panelW = 384.f;
	float panelRight = W - 16.f;
	float panelTop = H - 16.f;
	float panelLeft = panelRight - panelW;

	float cursorX = panelLeft + 60.f;

	float baseY = panelTop - 130.f;
	float gapY = 52.f;

	float y = baseY - gapY * mSelectIndex;
	y += 10.f;
	mCursor->SetPos(cursorX, y);
}

void CMenuUI::Select()
{
	switch (mSelectIndex)
	{
	case 0 :
		CLog::PrintLog("도감 선택");
		break;
	case 1:
	{
		CLog::PrintLog("포켓몬 선택");

		CSceneMain* Party = dynamic_cast<CSceneMain*>(mScene);
		if (Party)
		{
			CPlayerObject* Player = Party->GetPlayer();
			if (Player && Player->GetParty())
			{
				Player->GetParty()->SetEnable(true);
			}
		}
		SetEnable(false);
	}

		break;
	case 2:
		CLog::PrintLog("가방 선택");
		break;
	case 3:
		CLog::PrintLog("포켓내비 선택");
		break;
	case 4:
		CLog::PrintLog("봄이 선택");
		break;
	case 5:
		CLog::PrintLog("리포트 선택");
		break;
	case 6:
		CLog::PrintLog("설정 선택");
		break;
	case 7:
		CLog::PrintLog("닫기 선택");
		break;
	}
}

bool CMenuUI::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();
	float W = (float)RS.Width;
	float H = (float)RS.Height;

	CSharedPtr<CImage> Panel = mScene->GetUIManager()->CreateWidget<CImage>("MenuPanel");

	Panel->SetTexture("MenuPanel", TEXT("Texture/Pokemon/Player/MenuPanel.png"));

	Panel->SetBrushAnimation(false);
	Panel->SetSize(384.f, 576.f);
	Panel->SetPivot(FVector2D(1.f, 1.f));
	Panel->SetPos(W - 16.f, H - 16.f);
	
	Panel->SetZOrder(100);
	AddWidget(Panel);

	mCursor = mScene->GetUIManager()->CreateWidget<CImage>("MenuCursor");

	
	mCursor->SetTexture("MenuCursor", TEXT("Texture/Pokemon/Player/Cursor.png"));

	mCursor->SetBrushAnimation(false);
	mCursor->SetSize(32.f, 32.f);
	mCursor->SetPivot(FVector2D(0.f, 0.f));
	mCursor->SetZOrder(101);

	AddWidget(mCursor);

	UpdateCursor();


	float panelW = 384.f;
	float panelRight = W - 16.f;
	float panelTop = H - 16.f;
	float panelLeft = panelRight - panelW;

	float textX = panelLeft + 90.f;
	float baseY = panelTop - 130.f;
	float gapY = 52.f;

	mTextList.clear();
	mTextList.reserve(Index.size());

	for (int i = 0; i < (int)Index.size(); i++)
	{
		string name = "MenuText_" + to_string(i);

		CSharedPtr<CTextBlock> Text = mScene->GetUIManager()->CreateWidget<CTextBlock>(name);

		// 글씨 박스 크기
		Text->SetSize(260.f, 60.f);
		Text->SetFontSize(40.f);

		// 폰트/텍스트/색
		Text->SetFont("Default");
		Text->SetText(Index[i].c_str());
		Text->SetTextColor(0, 0, 0, 255);

		
		float y = baseY - gapY * i;
		Text->SetPos(textX, y);

		Text->SetZOrder(102); 
		AddWidget(Text);

		mTextList.push_back(Text);
	}

	return true;
}

void CMenuUI::Render()
{
	CUserWidget::Render();
}
