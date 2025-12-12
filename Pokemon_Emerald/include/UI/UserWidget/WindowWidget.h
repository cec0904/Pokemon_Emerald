#pragma once
#include "UserWidget.h"

class CWindowWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CWindowWidget();
	virtual ~CWindowWidget();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);


};

