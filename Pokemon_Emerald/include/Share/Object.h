#pragma once

#include "../Gameinfo.h"

//추상클래스 
// 키워드로 추상 클래스를 만들 수 있다. 
// abstract 해당 클래스는 추상 클래스다~ 라고 정의하는거다.

class CObject abstract	
{
protected:
	CObject();
	virtual ~CObject();

protected:
	int mRefCount = 0;
	bool mActive = true;	// false -> 삭제가 될거다.
	bool mEnable = true;	// false ->시점함수 호출만 안할것이다. 

public:
	bool IsActive()
	{
		return mActive;
	}

	bool IsEnable()
	{
		return mEnable;
	}

	int GetRefCount() const
	{
		return mRefCount;
	}

public:
	virtual void AddRef();
	virtual void Release();
	virtual void Destroy();
	void SetEnable(bool Enable)
	{
		mEnable = Enable;
	}

};

