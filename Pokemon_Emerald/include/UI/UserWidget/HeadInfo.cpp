#include "HeadInfo.h"

#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"


CHeadInfo::CHeadInfo()
{
}

CHeadInfo::~CHeadInfo()
{
}

bool CHeadInfo::Init()
{
	CUserWidget::Init();

	mNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("NameText");

	mNameText->SetText(L"가나다람");
	mNameText->SetFontSize(20.f);
	mNameText->SetPos(0.f, 0.f);
	mNameText->SetPivot(FVector2D(0.5f, 0.f));
	mNameText->SetAlignH(ETextAlignH::Center);
	mNameText->SetSize(100.f, 100.f);
	mNameText->SetTextColor(255, 255, 255, 255);

	AddWidget(mNameText);
	
	CSharedPtr<CProgressBar> HPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("HPBar");

	HPBar->SetPos(0.f, 0.f);
	HPBar->SetSize(400.f, 50);
	//block_bar.png
	//MPBar.png
	HPBar->SetTexture(EProgressBarImageType::Back, "HPBarBack", TEXT("Texture/MPBar.png"));
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar", TEXT("Texture/block_bar.png"));
	HPBar->SetPercent(0.5f);
	AddWidget(HPBar);


	return true;

}

void CHeadInfo::Render()
{
	CUserWidget::Render();
}

void CHeadInfo::Render(const FVector3D& Pos)
{
	CUserWidget::Render(Pos);

}
