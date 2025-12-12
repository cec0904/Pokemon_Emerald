#include "Object.h"

CObject::CObject()
{
}

CObject::~CObject()
{
}

void CObject::AddRef()
{
	++mRefCount;
}

void CObject::Release()
{
	--mRefCount;

	if (mRefCount == 0)
	{
		delete this;
	}

}

void CObject::Destroy()
{
	mActive = false;
}
