
struct PS_Output_Single
{
    //자료형 변수명 : 시멘틱
    // SV  시스템 벨류 (시스템에서 자동으로 넣어주는 값)
    // SV_TARGET을 하면 뒤에 0이 생략 된다. 숫자는 레지스터 넘버이다. 
    // SV_TARGET0 
    // 시멘틱을 통해서 내가 원하는 레지스터에 등록할 수 있다.
    float4 Color : SV_TARGET;
};

/////////////////////////////
// 상수 레지스터 
// 상수 버퍼 
// C++의 값을 쉐이더로 보낼때 사용한다.
// 상수버퍼와 구조화버퍼 등이 있다. 

//상수 레지스터 0번을 사용하겠다.
cbuffer Transform : register(b0)
{
    matrix gmatWorld;   //월드 행렬
    matrix gmatView;    //카메라(뷰) 행렬
    matrix gmatProj;    //투영(프로젝션) 행렬
    matrix gmatWV;      // 월드 * 카메라
    matrix gmatWVP;     // 월드 * 카메라 * 투영
    float3 gPivot;      // 피봇
    float gTransformEmpty;
}

cbuffer Material : register(b1)
{
	// 재질 기본 색상
    float4 gMtrlBaseColor;
	// 재질 투명도
    float gMtrlOpacity;
	// 재질 텍스쳐 가로 크기
    int gMtrlTextureWidth;
	// 재질 텍스쳐 세로 크기
    int gMtrlTextureHeight;
	// 16 바이트 맞추기용 더미 하나 추가해준다. 
    float gMtrlEmpty;
}

cbuffer Animation2D : register(b2)
{
    	//LT Left-Top
    float2 gAnim2DLTUV; //8
	//RB Right-Bottom
    float2 gAnim2DRBUV; //8
	// 애니메이션 사용 여부
    int gAnim2DEnable; // 4
	// 애니메이션 반전 여부
    int gAnim2DFlip;
	//빈 데이터 16 배수로 보내줘야한다. 
    float2 Empty;
}

SamplerState gBaseSampler : register(s0);

Texture2D gBaseTexture : register(t0);
//Texture2D gBaseTexture2 : register(t1);
//Texture2D gBaseTexture3 : register(t2);
//Texture2D gBaseTexture4 : register(t3);

float2 UpdateAnimation2D(float2 UV)
{
    if (gAnim2DEnable == 0)
    {
        return UV;
    }

    float2 Result = (float2)0.f;

    if (UV.x == 0.f) //좌
    {
        Result.x = gAnim2DLTUV.x;
    }
	else //우
    {
        Result.x = gAnim2DRBUV.x;
    }

    if (UV.y == 0.f) //위
    {
        Result.y = gAnim2DLTUV.y;
    }
    else //아래
    {
        Result.y = gAnim2DRBUV.y;
    }

    return Result;
}


