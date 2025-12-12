#pragma once
#include "ConstantBufferData.h"

class CMaterialCBuffer :
    public CConstantBufferData
{
public:
    CMaterialCBuffer();
    CMaterialCBuffer(const CMaterialCBuffer& Data);
    CMaterialCBuffer(CMaterialCBuffer&& Data);
    virtual ~CMaterialCBuffer();

private:
    FMaterialCBufferInfo mData;

public:
    virtual bool Init();
    virtual void UpdateBuffer();

    virtual CMaterialCBuffer* Clone();

public:
    void SetBaseColor(const FVector4D& Color)
    {
        mData.BaseColor = Color;
    }

    void SetOpacity(float Opacity)
    {
        mData.Opacity = Opacity;
    }

    void SetTextureSize(int Width, int Height)
    {
        mData.TextureWidth = Width;
        mData.TextureHeight = Height;
    }

};

