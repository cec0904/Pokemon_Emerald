#pragma once
#include "ConstantBufferData.h"

class CColliderCBuffer :
    public CConstantBufferData
{

public:
    CColliderCBuffer();
    CColliderCBuffer(const CColliderCBuffer& Data);
    CColliderCBuffer(CColliderCBuffer&& Data);
    virtual ~CColliderCBuffer();

private:
    FColliderCBufferInfo mData;

public:
    virtual bool Init();
    virtual void UpdateBuffer();

    virtual CColliderCBuffer* Clone();

public:
    void SetColor(float r, float g, float b, float a)
    {
        mData.Color = FVector4D(r, g, b, a);
    }

};

