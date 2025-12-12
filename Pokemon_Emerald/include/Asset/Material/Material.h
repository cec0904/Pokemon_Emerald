#pragma once
#include "../Asset.h"

// Mesh라는 모양도 갔고있고

// 머티리얼을 이용해서 색칠을 할것이다.
// 어떤 텍스쳐를 사용하는지
// 어떤 쉐이더를 사용하는지 

namespace ETextureSamplerType
{
	enum Type
	{
		Point,
        Linear,
        Anisotropic,
        End
	};
}

//머티리얼이 들고있을 텍스쳐 정보
struct FMaterialTextureInfo
{
    //텍스쳐 이름 
    string Name;
    //사용할 텍스쳐
    CSharedPtr<class CTexture> Texture;
    //텍스쳐를 넣을 레지스터 번호
    int Register = 0;
    // ShaderBufferType 쉐이더 타입도 넣어준다. 
    int ShaderBufferType = (int)EShaderBufferType::Pixel;
    // 텍스쳐 인덱스
    int TextureIndex = 0;
};

class CMaterial :
    public CAsset
{
    friend class CMaterialManager;

protected:
    CMaterial();
    CMaterial(const CMaterial& Material);
    ~CMaterial();

    
private:
    //샘플러
    //샘플러는 종류마다 만들어서 돌려쓴다!
    static ID3D11SamplerState* mSampler[ETextureSamplerType::End];

private:
    //샘플러 만들기 함수
    static void SetSampler(ETextureSamplerType::Type Type);
    static void DestroySampler();

protected:
    //머티리얼의 구성 요소
    // 텍스쳐 리스트 
    vector<FMaterialTextureInfo*> mTextureList;
    // 해당 머티리얼을 그리는 픽셀 쉐이더
    ID3D11PixelShader* mPS = nullptr;
    //텍스쳐에 추가할 데이터
    FVector4D mBaseColor = FVector4D::White;
    // 0 ~ 1 사이의 불투명도를 지정한다.
    // 0 : 투명   1 : 불투명
    float mOpacity = 1.f;

    // 머티리얼용 상수버퍼 등록
    class CMaterialCBuffer* mCBuffer = nullptr;

    // 이미지 텍스쳐 같이 쌍으로 들어가야할 데이터가
	// 샘플러
    ETextureSamplerType::Type mSamplerType = ETextureSamplerType::Linear;



public:
    //텍스쳐 가져오기
    // CTexture 를 넣어주는 방법 
    void AddTexture(class CTexture* Texture,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);
    //Name으로 찾아와서 텍스쳐를 넣어주는 바업
    void AddTexture(const string& Name,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);
    //파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
    void AddTexture(const string& Name,
        const TCHAR* FileName,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);


    //PS
    //쉐이더 가져오기 
    void SetPixelShader(const string& Name);
    void SetPixelShader(const string& Name, const char* EntryName, const TCHAR* FileName);
    void ClearShader();

    //베이스컬러
    void SetBaseColor(const FVector4D& Color);
    void SetBaseColor(float r, float g, float b, float a);

    //불투명도
    void SetOpacity(float Opacity);

    //샘플러 타입
    void SetSamplerType(ETextureSamplerType::Type SamplerType);


    //그리기 용도로 
    void SetMaterial();
    void ResetMaterial();

    CMaterial* Clone();

};

