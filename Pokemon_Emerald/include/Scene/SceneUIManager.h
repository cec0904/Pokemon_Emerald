#pragma once

#include "../UI/UserWidget/WindowWidget.h"

class CSceneUIManager
{
	friend class CScene;

private:
	CSceneUIManager();
	~CSceneUIManager();

private:
	class CScene* mScene = nullptr;

	//WindowWidget을 들고있을것이다.
	// 스탯창, 인벤토리, 설정 등 여러 UI창들을 찾아오는 용도
	unordered_map<string, CSharedPtr<CWindowWidget>> mWindowMap;

	//Scene 에서 생성된 모든 Widget 객체들 들고있을것이다. 
	vector<CSharedPtr<CWidget>> mWidgetList;

	//마우스 호버 된 위젯
	CSharedPtr<CWidget> mMouseHoveredWidget;


public:
	void AddWindowWidget(const string& Name, CWindowWidget* Widget)
	{

		if (FindWindowWidget(Name))
		{
			return;
		}

		mWindowMap.insert(make_pair(Name, Widget));
	}

	CWindowWidget* FindWindowWidget(const string& Name)
	{
		auto iter = mWindowMap.find(Name);

		if (iter == mWindowMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void AddToViewport(CWidget* Widget);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual bool CollisionMouse(float DeltaTime, const FVector2D& MousePos);
	virtual void Render();

public:
	//어떤 위젯클래스로 만들지 모르니까
	// 템플릿
	template<typename T>
	T* CreateWidget(const string& Name, CSceneObject* Owner = nullptr)
	{
		T* Widget = new T;

		Widget->mScene = mScene;
		Widget->mOwnerObject = Owner;
		Widget->mName = Name;

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		return Widget;
	}


	static bool SortCollision(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest);
	static bool SortRender(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest);

};

