//=========================================================
// damage_effect.h
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
#include "../../object.h"
#include "damage_effect.h"


//-------------------------------------
// HitPoint()
//-------------------------------------
DamageEffect::DamageEffect(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
    flash_flg_ = false;
    flash_mode_ = FLASH_IN;
    flash_alpha_ = 0.7f;
    hp_ = 1.0f;
    vertex_ = new Vertex2D[4];
    CalculateVertex();
    diffuse_texture_ = NULL;
    alpha_texture_ = NULL;
    shader_ = nullptr;
    shader_ = new Shader("resource/shader/damage_effect.hlsl");
    D3DXCreateTextureFromFile(
        DirectX9Holder::device_,
        "resource/texture/game/damage_diffuse.png", &diffuse_texture_);
    D3DXCreateTextureFromFile(
        DirectX9Holder::device_,
        "resource/texture/game/ellipse_alpha.png", &alpha_texture_);
}


//-------------------------------------
// ~HitPoint()
//-------------------------------------
DamageEffect::~DamageEffect()
{
    SAFE_DELETE_ARRAY(vertex_);
    SAFE_RELEASE(diffuse_texture_);
    SAFE_RELEASE(alpha_texture_);
    SAFE_DELETE(shader_);
}


//-------------------------------------
// Update
//-------------------------------------
void DamageEffect::Update()
{
    // ダメージを食らいすぎる
    if (hp_ < 0.3f){
        // 点滅フラグON
        flash_flg_ = true;
        if (flash_alpha_ <= 0.3f){
            flash_mode_ = FLASH_OUT;
        }
        else if (flash_alpha_ >= 0.8f){
            flash_mode_ = FLASH_IN;
        }
        if (flash_mode_ == FLASH_IN)
            flash_alpha_ -= 0.005f;
        else
            flash_alpha_ += 0.005f;

    }
    else{
        // 点滅フラグがONの時
        if (flash_flg_){
            if (flash_alpha_ >= 0.7f){
                flash_mode_ = FLASH_OUT;
            }
            else if (flash_alpha_ <= 0.2f){
                flash_mode_ = FLASH_IN;
            }
            if (flash_mode_ == FLASH_IN)
                flash_alpha_ += 0.005f;
            else
                flash_alpha_ -= 0.005f;
            // 本来のアルファ値と近くなったら点滅をやめる
            if (fabs(flash_alpha_ - damage_value_) < 0.02f)
                flash_flg_ = false;
        }
        else
            flash_alpha_ = damage_value_;
    }

	damage_value_ = 1.0f - hp_;
	damage_value_ = std::min<float>(damage_value_, 0.7f);
}


//-------------------------------------
// Draw
//-------------------------------------
void DamageEffect::Draw()
{
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    DirectX9Holder::device_->SetVertexDeclaration(
        DirectX9Holder::vertex_declaration_2d_);

    DirectX9Holder::device_->SetTexture(shader_->pixel_table()->GetSamplerIndex("diffuse_texture"), diffuse_texture_);
    DirectX9Holder::device_->SetTexture(shader_->pixel_table()->GetSamplerIndex("alpha_texture"), alpha_texture_);

    shader_->pixel_table()->SetFloat(DirectX9Holder::device_, "current_damage", damage_value_);
    shader_->pixel_table()->SetFloat(DirectX9Holder::device_, "effect_alpha", flash_alpha_);

    DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
    DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

    DirectX9Holder::device_->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP,
        2,
        vertex_,
        sizeof(Vertex2D));

	DirectX9Holder::device_->SetTexture(0, NULL);
    DirectX9Holder::device_->SetTexture(1, NULL);

    DirectX9Holder::device_->SetVertexShader(NULL);
    DirectX9Holder::device_->SetPixelShader(NULL);


	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// CreateVertex()
//-------------------------------------
void DamageEffect::CalculateVertex()
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
// end of file
//-------------------------------------
