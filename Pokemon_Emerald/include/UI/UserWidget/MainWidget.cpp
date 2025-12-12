#include "MainWidget.h"

#include "../../Device.h"
#include "../Common/Button.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../Common/TextBlock.h"
#include "../Common/ProgressBar.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	CWindowWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	

	return true;
}
