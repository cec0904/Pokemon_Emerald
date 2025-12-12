#pragma once
#include "GraphicShader.h"
class CTileShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CTileShader();
    virtual ~CTileShader();

public:
    bool Init();
};

