struct PS_IN
{
    float4 PosH	   : SV_POSITION;
    float4 Color   : COLOR;
};

float4 OutLineMesh_PS(PS_IN pin) : SV_Target
{
    return pin.Color;
}