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
	float4 color : COLOR0;
	float3 normal : NORMAL0;
	float2 texture_uv : TEXCOORD0;
	float3 weight : BLENDWEIGHT0;
	float4 blend_index : BLENDINDICES;
};


struct V_OUT
{
	float4 position : POSITION;
	float2 texture_uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : COLOR0;
};


//--------------------------------------
// register
//--------------------------------------
float3 light_direction;
float4 light_diffuse;
float4x4 matrix_bone_w[60];
float4x4 matrix_vp;


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

	int index0, index1, index2, index3;
	float weight0, weight1, weight2, weight3;
	float4x4 matrix_w0, matrix_w1, matrix_w2, matrix_w3;

	index0 = v_in.blend_index.x;
	index1 = v_in.blend_index.y;
	index2 = v_in.blend_index.z;
	index3 = v_in.blend_index.w;

	weight0 = v_in.weight.x;
	weight1 = v_in.weight.y;
	weight2 = v_in.weight.z;
	weight3 = 1.0f - weight0 - weight1 - weight2;

	matrix_w0 = matrix_bone_w[index0];
	matrix_w1 = matrix_bone_w[index1];
	matrix_w2 = matrix_bone_w[index2];
	matrix_w3 = matrix_bone_w[index3];

	//worldÇ∆vpÇÇ©ÇØÇ†ÇÌÇπÇÎÅI
	float4x4 matrix_w = matrix_w0 * weight0
						+ matrix_w1 * weight1
						+ matrix_w2 * weight2
						+ matrix_w3 * weight3;

	v_out.position = mul(float4(v_in.position, 1.0f), matrix_w);
	v_out.position = mul(v_out.position, matrix_vp);

	float3 world_normal = normalize(mul(float4(v_in.normal, 0.0f), matrix_w)).xyz;
	float4 light = dot(world_normal, -light_direction);
	light *= 0.5f;
	light += 0.5f;
	v_out.normal = world_normal.xyz;
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

}