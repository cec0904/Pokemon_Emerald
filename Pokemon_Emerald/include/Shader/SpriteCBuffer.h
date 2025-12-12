#pragma once
#include "ConstantBufferData.h"

class CSpriteCBuffer :
    public CConstantBufferData
{
public:
    CSpriteCBuffer();
    CSpriteCBuffer(const CSpriteCBuffer& Data);
    CSpriteCBuffer(CSpriteCBuffer&& Data);
    virtual ~CSpriteCBuffer();

private:
    FSpriteCBufferInfo mData;

public:
    virtual bool Init();
    virtual void UpdateBuffer();

    virtual CSpriteCBuffer* Clone();

public:
    void SetTint(const FVector4D& Color)
    {
        mData.Tint = Color;
    }

    void SetUseTime(bool Use)
    {
        mData.TimeUse = Use ? 1 : 0;
    }

    void SetTime(float Time)
    {
        mData.msTime = Time;
    }


};

