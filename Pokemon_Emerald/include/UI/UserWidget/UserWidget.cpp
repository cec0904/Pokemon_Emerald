#include "UserWidget.h"

CUserWidget::CUserWidget()
{
}

CUserWidget::~CUserWidget()
{
}

bool CUserWidget::Init()
{
	return CWidget::Init();
}

void CUserWidget::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

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
		(*iter)->Update(DeltaTime);
		++iter;
	}

}

void CUserWidget::Render()
{
	CWidget::Render();

	//그려줄 위젯 정렬
	if (mWidgetList.size() >= 2)
	{
		//vector
		//Z오더는 숫가자 클수록 나중에 그려진다.
		//UI는 나중에 그려지는에가 가장 맨위에 그려진다. 
		sort(mWidgetList.begin(), mWidgetList.end(), CUserWidget::SortRender);
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
}

void CUserWidget::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);

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
		(*iter)->Render(Pos);
		++iter;
	}

}

bool CUserWidget::CollisionMouse(CWidget** Result, const FVector2D& MousePos)
{
	//충돌 할것들 정렬 해줄것이다. 
	if (mWidgetList.size() >= 2)
	{
		sort(mWidgetList.begin(), mWidgetList.end(), CUserWidget::SortCollision);
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

		if ((*iter)->CollisionMouse(Result, MousePos))
		{
			return true;
		}
		++iter;
	}

	return false;
}


bool CUserWidget::SortCollision(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}


bool CUserWidget::SortRender(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}
