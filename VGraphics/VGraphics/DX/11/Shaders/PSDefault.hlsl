//ps_5_0

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};
float4 psmain(VS_OUTPUT input) : SV_Target
{
    return float4(input.color, 1);
}