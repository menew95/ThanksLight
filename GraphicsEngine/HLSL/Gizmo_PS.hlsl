struct PS_IN
{
    float4 PosH	   : SV_POSITION;
    float4 Color   : COLOR;
};

//cbuffer perGizmo : register(b4)
//{
//    float4 color;
//};

float4 Gizmo_PS(PS_IN pin) : SV_Target
{
    return pin.Color;
}