//=========================================================
// fort_gage.cpp
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
#include "../../object.h"
#include "../../../resource/texture_manager.h"
#include "fort_gage.h"


//-------------------------------------
// FortGage()
//-------------------------------------
FortGage::FortGage(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
    vertex_ = new Vertex2D[4];
    gauge_uv_y_ = 0.0f;
    CalculateVertex();
    gauge_position_y_ = vertex_[0].position_.y;
    min_position_y_ = vertex_[0].position_.y;
    max_position_y_ = vertex_[2].position_.y;
}


//-------------------------------------
// ~FortGage()
//-------------------------------------
FortGage::~FortGage()
{
    SAFE_DELETE_ARRAY(vertex_);
    texture_ = NULL;
}

//-------------------------------------
// Update
//-------------------------------------
void FortGage::Update()
{
    CalculateVertex();
}


//-------------------------------------
// Draw
//-------------------------------------
void FortGage::Draw()
{
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    DirectX9Holder::device_->SetVertexDeclaration(
        DirectX9Holder::vertex_declaration_2d_);

    DirectX9Holder::device_->SetTexture(0, texture_);

    DirectX9Holder::device_->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP,
        2,
        vertex_,
        sizeof(Vertex2D));

    DirectX9Holder::device_->SetTexture(0, NULL);

    DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//-------------------------------------
// CreateVertex()
//-------------------------------------
void FortGage::CalculateVertex()
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

    vertex_[0].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[1].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[2].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[3].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vertex_[0].texture_ = { 0.0f, gauge_uv_y_ };
    vertex_[1].texture_ = { 1.0f, gauge_uv_y_ };
    vertex_[2].texture_ = { 0.0f, 1.0f };
    vertex_[3].texture_ = { 1.0f, 1.0f };
}


//-------------------------------------
// AddGauge()
//-------------------------------------
void FortGage::AddGauge(const float value)
{
    float add_pos_value = parameter_.scaling_.y_ * value;
    gauge_uv_y_ += value;

    gauge_uv_y_ = std::min<float>(gauge_uv_y_, 1.0f);
    gauge_uv_y_ = std::max<float>(gauge_uv_y_, 0.0f);

    vertex_[0].position_.y += add_pos_value;
    vertex_[1].position_.y += add_pos_value;

    vertex_[0].position_.y = std::min<float>(vertex_[0].position_.y , max_position_y_);
    vertex_[0].position_.y = std::max<float>(vertex_[0].position_.y, min_position_y_);
    vertex_[1].position_.y = std::min<float>(vertex_[1].position_.y, max_position_y_);
    vertex_[1].position_.y = std::max<float>(vertex_[1].position_.y, min_position_y_);

    vertex_[0].texture_ = { 0.0f, gauge_uv_y_ };
    vertex_[1].texture_ = { 1.0f, gauge_uv_y_ };
    vertex_[2].texture_ = { 0.0f, 1.0f };
    vertex_[3].texture_ = { 1.0f, 1.0f };
}

//-------------------------------------
// SetGauge()
//-------------------------------------
void FortGage::SetGauge(const float life)
{

    gauge_uv_y_ = life - 1.0f;
    gauge_uv_y_ = abs(gauge_uv_y_);

    vertex_[0].position_.y = gauge_position_y_ + parameter_.scaling_.y_ * gauge_uv_y_;
    vertex_[1].position_.y = gauge_position_y_ + parameter_.scaling_.y_ * gauge_uv_y_;

    vertex_[0].texture_ = { 0.0f, gauge_uv_y_ };
    vertex_[1].texture_ = { 1.0f, gauge_uv_y_ };
    vertex_[2].texture_ = { 0.0f, 1.0f };
    vertex_[3].texture_ = { 1.0f, 1.0f };
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void FortGage::SetTexture(
    const std::string &path)
{
    texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// end of file
//-------------------------------------
