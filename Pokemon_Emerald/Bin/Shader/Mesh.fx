#include "Share.fx"

//정점정보
// 정점위치, UV좌표
struct VS_Input_Tex
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Output_Tex
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

VS_Output_Tex MeshVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex)0;

        // Output.Pos의 X Y Z가 들어가고 
    // W값은 1이 들어간다. 
    output.Pos = mul(float4(input.Pos, 1.f), gmatWVP); // 5일차 수정1 : gmatWVP 사용
                                    // 뒤에 1인 이유는 이동량을 포함시키기 위해서
    output.UV = input.UV;
    
    return output;
}

PS_Output_Single DefaultMaterialPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);

    //가져온 이미지 색상 * 베이스컬러색상 
    Color.rgb *= gMtrlBaseColor.rgb;

    Color.a *= gMtrlOpacity;

    output.Color = Color;

    return output;
}