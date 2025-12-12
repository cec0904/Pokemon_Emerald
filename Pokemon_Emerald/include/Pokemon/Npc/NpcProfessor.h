#pragma once
#include "NpcBase.h"

class CNpcProfessor :
	public CNpcBase
{
	friend class CScene;

protected:
	CNpcProfessor();
	CNpcProfessor(const CNpcProfessor& Obj);
	CNpcProfessor(CNpcProfessor&& Obj);
	virtual ~CNpcProfessor();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

