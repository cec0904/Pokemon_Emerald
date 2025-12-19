#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../UI/Widget.h"
#include "../../UI/Common/Image.h"
#include "../../UI/Common/TextBlock.h"
class CMenuUI :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CMenuUI();
	virtual ~CMenuUI();

public:
	vector<wstring> Index =
	{
		L"도감",
		L"포켓몬",
		L"가방",
		L"포켓내비",
		L"봄이",
		L"리포트",
		L"설정",
		L"닫기"
	};


	int mMaxIndex = (int)Index.size();
	int mSelectIndex = 0;
	
	vector<CSharedPtr<CTextBlock>> mTextList;
	CSharedPtr<CImage> mCursor;


public:
	void MoveUp();
	void MoveDown();
	void UpdateCursor();
	void Select();



public:
	virtual bool Init();
	virtual void Render() override;
	
	
};

