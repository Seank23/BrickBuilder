struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer ScreenBuffer : register(b0)
{
    float4 CameraPosition;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 InvViewMatrix;
    float4x4 InvProjectionMatrix;
    float2 ScreenSize;
};

cbuffer GridConfig : register(b1)
{
    float3 Origin;
    float CellSize;
    float LineThickness;
    float3 BaseColor;
    float3 LineColor;
    float Opacity;
};

Texture2D depthMap : register(t0);
Texture2D sceneMap : register(t1);
SamplerState samp : register(s0);

float4 DrawGrid(PSInput input)
{
    float2 ndc = input.texCoord * 2.0f - 1.0f;
    ndc.y *= -1.0f;
    
    float4 clip = float4(ndc, 1.0f, 1.0f);
    float4 view = mul(InvProjectionMatrix, clip);
    view /= view.w;
    float3 rayDir = normalize(mul((float3x3) InvViewMatrix, view.xyz));
    
    float3 rayOrigin = CameraPosition.xyz;
    
    float t = -rayOrigin.y / rayDir.y;
    if (t < 0)
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 hitPoint = rayOrigin + rayDir * t;
    
    float2 coords = hitPoint.xz / CellSize;
    float2 fracCoords = frac(Origin.xz + coords);
    
    float lineX = min(fracCoords.x, 1.0f - fracCoords.x);
    float lineZ = min(fracCoords.y, 1.0f - fracCoords.y);
    float gridLine = step(lineX, LineThickness) + step(lineZ, LineThickness);
    
    float3 gridColor = lerp(BaseColor, LineColor, saturate(gridLine));
    
    float dist = distance(rayOrigin.xz, hitPoint.xz);
    float a = saturate(1.0f - dist / 100.0f);
    return float4(gridColor, a * Opacity);
}

float4 main(PSInput input) : SV_TARGET
{
    float4 sceneColor = sceneMap.Sample(samp, input.texCoord);
    float depth = depthMap.Sample(samp, input.texCoord).r;
    
    float4 gridColor = DrawGrid(input);
    float4 gridBlend = gridColor * gridColor.a + sceneColor * (1.0f - gridColor.a);
    return floor(depth) > 0.0f ? gridBlend : sceneColor;
}