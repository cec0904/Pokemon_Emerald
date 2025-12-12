#pragma once
#include "UserWidget.h"
#include "../Common/TextBlock.h"
#include "../Common/ProgressBar.h"


class CHeadInfo :
    public CUserWidget
{
    friend class CSceneUIManager;

protected:
    CHeadInfo();
    ~CHeadInfo();

protected:
    CSharedPtr<CTextBlock> mNameText;

public:
    virtual bool Init();
    virtual void Render();
    virtual void Render(const FVector3D& Pos);

};

