//=========================================================
// damage_effect.hlsl
// author:shohei matsumoto
//=========================================================


//--------------------------------------
// struct
//--------------------------------------
struct V_IN
{
    float3 position : POSITION0;
    float2 texture_uv : TEXCOORD;
};

struct V_OUT
{
    float4 position : POSITION;
	float2 texture_uv : TEXCOORD0;
};


//--------------------------------------
// register
//--------------------------------------
float current_damage;
float effect_alpha;


//--------------------------------------
// sampler
//--------------------------------------
sampler diffuse_texture : TEXTURE0;
sampler alpha_texture : TEXTURE1;


//=========================================================
// VS()
//=========================================================
V_OUT VS(V_IN v_in)
{
    V_OUT v_out = (V_OUT)0;

    v_out.position = float4(v_in.position , 0.0f);
    v_out.texture_uv = v_in.texture_uv;

    return v_out;
}

//=========================================================
// PS()
//=========================================================
float4 PS(V_OUT v_out) : COLOR0
{
    float alpha = tex2D(alpha_texture, v_out.texture_uv).r;
    float a = abs(alpha - 1.0f);
    float4 color = tex2D(diffuse_texture, v_out.texture_uv);

    color.a = color.a * a * current_damage;
    return color;
}