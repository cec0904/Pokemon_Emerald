#include "WindowWidget.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CWindowWidget::CWindowWidget()
{
}

CWindowWidget::~CWindowWidget()
{
}

bool CWindowWidget::Init()
{
	CUserWidget::Init();

	mScene->GetUIManager()->AddWindowWidget(mName, this);

	return true;
}

void CWindowWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);
}

void CWindowWidget::Render()
{
	CUserWidget::Render();
}

void CWindowWidget::Render(const FVector3D& Pos)
{
	CUserWidget::Render(Pos);
}
