#include "Share.fx"

cbuffer Collider : register(b4)
{
    float4 ColliderColor;
}

//우리의 정점정보
//12바이트 정점 16바이트 컬러
//미리 시멘트를 만들어 둔다. 
struct VS_Input_Color
{
    float3 Pos : POSITION; //POSITION 0번 레지스터 
    float4 Color : COLOR; //COLOR 0번 레지스터
};

struct VS_Output_Color
{
    //투영행렬의 w값도 포함되어야 하기때문에 결과값에는
    // 4개의 float4 값을 반환해준다. 
    
    // SV 를 붙여줘야 레스터라이즈 단계에서 애가 위치값이다를 인식한다. 
    //만약 SV를 안붙이고 일반 POSITION 이라고 한다면 
    // 레스터라이즈 단계에서 이게 위치값인지 아닌지 모른다.
    float4 Pos : SV_POSITION; //SV_POSITION 0번 레지스터
    float4 Color : COLOR; //COLOR 0번 레지스터
};

VS_Output_Color ColorMeshVS(VS_Input_Color input)
{
    //유연한 형변환 0으로 초기화 
    //
    VS_Output_Color output = (VS_Output_Color) 0;
    
    // Output.Pos의 X Y Z가 들어가고 
    // W값은 1이 들어간다. 
    output.Pos = mul(float4(input.Pos, 1.f), gmatWVP); // 5일차 수정1 : gmatWVP 사용
                                    // 뒤에 1인 이유는 이동량을 포함시키기 위해서
    
    output.Color = input.Color;
    
    return output;
}


PS_Output_Single ColorMeshPS(VS_Output_Color input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.Color = input.Color;
    
    return output;
}

//main 진입점
//속성 -> HLSL 컴파일러 -> 진입점 이름 ColorMeshVS 로 설정 
//쉐이더 형식 효과(/fx)
//쉐이더 모델 5.0


//FrameCeterRect 
float4 FrameMeshVS(float3 Pos : POSITION) : SV_POSITION
{
    //유연한 형변환 0으로 초기화 
    //
    float4 output = (float4) 0;
    
    // Output.Pos의 X Y Z가 들어가고 
    // W값은 1이 들어간다. 
    output = mul(float4(Pos, 1.f), gmatWVP);
    
    return output;
}

PS_Output_Single FrameMeshPS(float4 Pos : SV_POSITION)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.Color = ColliderColor;
    
    return output;
}