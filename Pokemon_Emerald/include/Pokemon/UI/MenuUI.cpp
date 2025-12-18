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
		CLog::PrintLog("포켓몬 선택");
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
	float W = RS.Width;
	float H = RS.Height;

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

	return true;
}

void CMenuUI::Render()
{

	CUserWidget::Render();

	CFont* Font = CAssetManager::GetInst()->GetFontManager()->FindFont("Default");
	if (!Font)
	{
		return;
	}

	ID2D1SolidColorBrush* Brush =
		CAssetManager::GetInst()->GetFontManager()->FindFontColor(0, 0, 0, 255);
	if (!Brush)
	{
		return;
	}

	FResolution RS = CDevice::GetInst()->GetResolution();
	float W = RS.Width;
	float H = RS.Height;

	float panelW = 384.f;
	float panelH = 576.f;
	float panelRight = W - 16.f;
	float panelTop = H - 16.f;
	float panelLeft = panelRight - panelW;

	float textX = panelLeft + 90.f;
	float baseY = panelTop - 130.f;
	float gapY = 52.f;

	for (int i = 0; i < (int)Index.size(); i++)
	{
		const wstring& txt = Index[i];
		IDWriteTextLayout* Layout = Font->CreateLayout(txt.c_str(), (int)txt.size(), 220.f, 40.f);

		if (!Layout)
		{
			continue;
		}

		float uiY = baseY - gapY * i;
		float d2dY = H - uiY - 40.f;

		CDevice::GetInst()->Get2DTarget()->BeginDraw();
		CDevice::GetInst()->Get2DTarget()->DrawTextLayout(
			D2D1::Point2F(textX, d2dY),
			Layout,
			Brush
		);
		CDevice::GetInst()->Get2DTarget()->EndDraw();




		Layout->Release();
	}
}

