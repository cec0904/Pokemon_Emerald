#pragma once
#include "Animation2DCBuffer.h"
#include "ConstantBufferData.h"

class CUICBuffer :
    public CConstantBufferData
{
public:
    CUICBuffer();
    CUICBuffer(const CUICBuffer& Data);
    CUICBuffer(CUICBuffer&& Data);
    virtual ~CUICBuffer();

private:
    FUICBufferInfo mData;

public:
    virtual bool Init();
    virtual void UpdateBuffer();

    virtual CUICBuffer* Clone();

public:
    void SetTint(const FVector4D& Color)
    {
        mData.BrushTint = Color;
    }

    void SetWidgetColor(const FVector4D& Color)
    {
        mData.Color = Color;
    }

    void SetTextureEnable(bool Enable)
    {
        mData.BrushTextureEnable = Enable ? 1 : 0;
    }

    void SetUV(float LTX, float LTY, float RBX, float RBY)
    {
        mData.LTUV.x = LTX;
        mData.LTUV.y = LTY;
        mData.RBUV.x = RBX;
        mData.RBUV.y = RBY;
    }

    void SetAnimationEnable(bool Enable)
    {
        mData.BrushAnimEnable = Enable ? 1 : 0;
    }

    void SetFlip(bool Flip)
    {
        mData.UIFlip = Flip;
    }

    void SetUseColorKey(bool Use)
    {
        mData.UseColorKey = Use ? 1 : 0;
    }

    void SetKeyThreshold(float T)
    {
        mData.KeyThreshold = T;
    }

    void SetColorKey(const FVector3D& Key)
    {
        mData.ColorKey = Key;
    }

};

