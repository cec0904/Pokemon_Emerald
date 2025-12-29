#include "FadeWidget.h"

#include "../../Device.h"
#include "../../Scene/Input.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Share/Log.h"
#include "../../Object/SpriteEffect.h"
#include "../../Component/SpriteComponent.h"
#include "../../Asset/Animation/Animation2DData.h"
#include "../../Asset/Animation/Animation2DManager.h"
#include "../../Animation/Animation2D.h"
#include "../../Animation/Animation2DSequence.h"
#include "../../UI/Common/TextBlock.h"
#include "../../UI/Common/Button.h"
#include "../../UI/Common/Image.h"
#include "../../Asset/Asset.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Font/Font.h"
#include "../../Asset/Font/FontManager.h"

CFadeWidget::CFadeWidget()
{
}

CFadeWidget::~CFadeWidget()
{
}

bool CFadeWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	CSharedPtr<CImage> Fade = mScene->GetUIManager()->CreateWidget<CImage>("Fade");

	Fade->SetTexture("Fade", TEXT("Texture/Pokemon/BackGround/Fade.png"));

	Fade->SetEnable(false);
	Fade->SetZOrder(999);
	AddWidget(Fade);

	return true;
}

void CFadeWidget::Update(float DeltaTime)
{
	
}