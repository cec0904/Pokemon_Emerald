#pragma once
#include "MonsterObject.h"


class CNearingMonster :
    public CMonsterObject
{
	friend class CScene;


protected:
	CNearingMonster();
	CNearingMonster(const CNearingMonster& Obj);
	CNearingMonster(CNearingMonster&& Obj);
	virtual ~CNearingMonster();

protected:
	float mAttackTime = 1.f;
	float mDetectAngle = 45.f;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);


};

