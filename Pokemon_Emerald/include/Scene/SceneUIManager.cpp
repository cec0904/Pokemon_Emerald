#include "SceneUIManager.h"
#include "../Share/Log.h"

CSceneUIManager::CSceneUIManager()
{
}

CSceneUIManager::~CSceneUIManager()
{
}

void CSceneUIManager::AddToViewport(CWidget* Widget)
{
	mWidgetList.emplace_back(Widget);
}

bool CSceneUIManager::Init()
{



	return true;
}

void CSceneUIManager::Update(float DeltaTime)
{
	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(DeltaTime);
		++iter;
	}

	if (mMouseHoveredWidget)
	{
		//CLog::PrintLog(mMouseHoveredWidget->GetName());
	}
	
}

bool CSceneUIManager::CollisionMouse(float DeltaTime, const FVector2D& MousePos)
{
	//충돌 할것들 정렬 해줄것이다. 
	if (mWidgetList.size() >=2)
	{
		sort(mWidgetList.begin(), mWidgetList.end(), CSceneUIManager::SortCollision);
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		
		CWidget* HoveredWidget = nullptr;

		// 충돌 처리를 해줄것이다.
		if ((*iter)->CollisionMouse(&HoveredWidget, MousePos))
		{
			if (mMouseHoveredWidget != HoveredWidget)
			{
				if (mMouseHoveredWidget)
				{
					mMouseHoveredWidget->MouseUnHovered();
				}
				// 처음으로 호버링 됬습니다 콜백 해줄것이다. 
				mMouseHoveredWidget = HoveredWidget;
			}
			return true;
		}

		++iter;
	}

	//충돌이 없었는데 만약 mMouseHoveredWidget 가 nullptr이 아니야?
	if (mMouseHoveredWidget)
	{
		mMouseHoveredWidget = nullptr;
	}


	return false;
}

//void CSceneUIManager::Collision(float DeltaTime)
//{
//	// UI 충돌을 해줄것이다. 
//}

void CSceneUIManager::Render()
{
	//그려줄 위젯 정렬
	if (mWidgetList.size() >= 2)
	{
		//vector
		//Z오더는 숫가자 클수록 나중에 그려진다.
		//UI는 나중에 그려지는에가 가장 맨위에 그려진다. 
		sort(mWidgetList.begin(), mWidgetList.end(), CSceneUIManager::SortRender);
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		// UI 그려줘
		(*iter)->Render();
		++iter;
	}

	iter = mWidgetList.begin();
	iterEnd = mWidgetList.end();

	for (;iter != iterEnd; ++iter)
	{
		(*iter)->EndFrame();
	}
}



// 오름차순 : 충돌 처리는 앞에서부터 뒤로 검사하여 가장 앞에 있는 UI가 충돌됬느지 검사하기 위해서 
bool CSceneUIManager::SortCollision(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

bool CSceneUIManager::SortRender(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	// 내림차순
	return Src->GetZOrder() < Dest->GetZOrder();
}
