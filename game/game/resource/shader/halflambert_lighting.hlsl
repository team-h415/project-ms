//=========================================================
// halflambert_lighting.hlsl
// author:ryuya nakamura
//=========================================================


//--------------------------------------
// struct
//--------------------------------------
struct V_IN
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texture_uv : TEXCOORD0;
};


struct V_OUT
{
	float4 position : POSITION;
	float2 texture_uv : TEXCOORD;
	float4 color : COLOR0;
};


//--------------------------------------
// register
//--------------------------------------
float3 light_direction;
float4 light_diffuse;
float4x4 matrix_w;
float4x4 matrix_wvp;


//--------------------------------------
// sampler
//--------------------------------------
sampler texture_0 : TEXTURE0;


//=========================================================
// VS()
//=========================================================
V_OUT VS(V_IN v_in)
{
	V_OUT v_out;

	v_out.position = mul(float4(v_in.position, 1.0f), matrix_wvp);

	float3 world_normal = normalize(mul(float4(v_in.normal, 0.0f), matrix_w)).xyz;
	float4 light = dot(world_normal, -light_direction);
	light *= 0.5f;
	light += 0.5f;
	v_out.color = light_diffuse * light;
	v_out.color.a = 1.0f;
	v_out.texture_uv = v_in.texture_uv;

	return v_out;
}


//=========================================================
// PS()
//=========================================================
float4 PS(V_OUT v_out) : COLOR
{
	return tex2D(texture_0,v_out.texture_uv) * v_out.color;
//	return v_out.color;
}