//=========================================================
// water_gage.cpp
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../../input/input.h"
#include "../../../input/inputs/keyboard.h"
#include "../../../shader/shader.h"
#include "../../../shader/shader_manager.h"
#include "../../object.h"
#include "../../../resource/texture_manager.h"
#include "water_gage.h"


//-------------------------------------
// WaterGage()
//-------------------------------------
WaterGage::WaterGage(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
    gauge_value_ = 1.0f;
    vertex_ = new Vertex2D[4];
    CalculateVertex();
    diffuse_texture_ = NULL;
    alpha_texture_ = NULL;
    shader_ = nullptr;
    shader_ = ShaderManager::Get("resource/shader/water_gage.hlsl");
    D3DXCreateTextureFromFile(
        DirectX9Holder::device_,
        "resource/texture/game/water_gage_alpha.png", &alpha_texture_);
}

//-------------------------------------
// ~WaterGage()
//-------------------------------------
WaterGage::~WaterGage()
{
    SAFE_DELETE_ARRAY(vertex_);
    diffuse_texture_ = NULL;
    SAFE_RELEASE(alpha_texture_);
    shader_ = nullptr;
}


//-------------------------------------
// Update
//-------------------------------------
void WaterGage::Update()
{
	//-------------------------------------
	// ç≈è¨ÅEç≈ëÂílê›íË
	gauge_value_ = std::min<float>(gauge_value_, 1.0f);
	gauge_value_ = std::max<float>(gauge_value_, 0.0f);
}


//-------------------------------------
// Draw
//-------------------------------------
void WaterGage::Draw()
{
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    DirectX9Holder::device_->SetVertexDeclaration(
        DirectX9Holder::vertex_declaration_2d_);

    DirectX9Holder::device_->SetTexture(shader_->pixel_table()->GetSamplerIndex("diffuse_texture"), diffuse_texture_);
    DirectX9Holder::device_->SetTexture(shader_->pixel_table()->GetSamplerIndex("alpha_texture"), alpha_texture_);

    shader_->pixel_table()->SetFloat(DirectX9Holder::device_, "current_gage", gauge_value_);

    DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
    DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());


    DirectX9Holder::device_->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP,
        2,
        vertex_,
        sizeof(Vertex2D));

    DirectX9Holder::device_->SetTexture(0, NULL);

    DirectX9Holder::device_->SetVertexShader(NULL);
    DirectX9Holder::device_->SetPixelShader(NULL);

    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//-------------------------------------
// CreateVertex()
//-------------------------------------
void WaterGage::CalculateVertex()
{
    if (!vertex_) return;

    vertex_[0].position_ = {
        parameter_.position_.x_ - parameter_.scaling_.x_ * 0.5f,
        parameter_.position_.y_ - parameter_.scaling_.y_ * 0.5f,
        0.0f
    };

    vertex_[1].position_ = {
        parameter_.position_.x_ + parameter_.scaling_.x_ * 0.5f,
        parameter_.position_.y_ - parameter_.scaling_.y_ * 0.5f,
        0.0f
    };

    vertex_[2].position_ = {
        parameter_.position_.x_ - parameter_.scaling_.x_ * 0.5f,
        parameter_.position_.y_ + parameter_.scaling_.y_ * 0.5f,
        0.0f
    };

    vertex_[3].position_ = {
        parameter_.position_.x_ + parameter_.scaling_.x_ * 0.5f,
        parameter_.position_.y_ + parameter_.scaling_.y_ * 0.5f,
        0.0f
    };

    vertex_[0].rhw_ = 1.0f;
    vertex_[1].rhw_ = 1.0f;
    vertex_[2].rhw_ = 1.0f;
    vertex_[3].rhw_ = 1.0f;

    vertex_[0].texture_ = { 0.0f, 0.0f };
    vertex_[1].texture_ = { 1.0f, 0.0f };
    vertex_[2].texture_ = { 0.0f, 1.0f };
    vertex_[3].texture_ = { 1.0f, 1.0f };
}

//-------------------------------------
// SetTexture()
//-------------------------------------
void WaterGage::SetTexture(
    const std::string &path)
{
    diffuse_texture_ = TextureManager::GetTexture(path.c_str());
}

//-------------------------------------
// end of file
//-------------------------------------
