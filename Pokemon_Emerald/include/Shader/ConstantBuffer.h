#pragma once

#include "../Share/Object.h"
#include "CBufferContainer.h"

class CConstantBuffer :
    public CObject
{
    friend class CShaderManager;

private:
    CConstantBuffer();
    ~CConstantBuffer();

private:
    ID3D11Buffer* mBuffer = nullptr;
    int mSize = 0;
    int mRegister = 0;
    int mShaderBufferType = 0;

public:
    bool Init(int Size, int Register, int ShaderBufferType);
    void Update(void* Data);


};

