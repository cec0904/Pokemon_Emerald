#pragma once
#include "NpcBase.h"
#include "../../UI/UserWidget/UserWidget.h"
#include "../../UI/Widget.h"
#include "../../UI/Common/Image.h"
#include "../UI/StartPokemonUI.h"

class CNpcProfessor :
	public CNpcBase
{
	friend class CScene;

protected:
	CNpcProfessor();
	CNpcProfessor(const CNpcProfessor& Obj);
	CNpcProfessor(CNpcProfessor&& Obj);
	virtual ~CNpcProfessor();
private:
	void Interact(float DeltaTime);

private:
	CSharedPtr<CStartPokemonUI> mStartSelect;
	bool mStartOnce = false;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};
