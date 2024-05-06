//vs_5_0
struct Transform
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};
cbuffer constant : register(b0)
{
    Transform transform;
}
struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    //World Matrix Space
    output.position = mul(input.position, transform.WorldMatrix);
    //View Matrix Space
    output.position = mul(output.position, transform.ViewMatrix);
    //Projection Matrix Space
    output.position = mul(output.position, transform.ProjectionMatrix);
     
    output.color = input.color;
    
    return output;
}