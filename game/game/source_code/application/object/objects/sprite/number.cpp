//=========================================================
// Number.cpp
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
#include "Number.h"


//-------------------------------------
// Number()
//-------------------------------------
Number::Number(
    OBJECT_PARAMETER_DESC &parameter , const int value)
{
    parameter_ = parameter;
    vertex_ = new Vertex2D[4];
    value_ = value;
    CalculateVertex();
    texture_ = NULL;
}


//-------------------------------------
// ~Number()
//-------------------------------------
Number::~Number()
{
    SAFE_DELETE_ARRAY(vertex_);
    SAFE_RELEASE(texture_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Number::Update()
{
    CalculateVertex();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Number::Draw()
{
    if (!vertex_) return;
    vertex_[0].texture_ = { (value_ * 0.1f), 0.0f };
    vertex_[1].texture_ = { (value_ * 0.1f + 0.1f), 0.0f };
    vertex_[2].texture_ = { (value_ * 0.1f), 1.0f };
    vertex_[3].texture_ = { (value_ * 0.1f + 0.1f), 1.0f };

    DirectX9Holder::device_->SetVertexDeclaration(
        DirectX9Holder::vertex_declaration_2d_);

    DirectX9Holder::device_->SetTexture(0, texture_);

    DirectX9Holder::device_->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP,
        2,
        vertex_,
        sizeof(Vertex2D));

    DirectX9Holder::device_->SetTexture(0, NULL);
}


//-------------------------------------
// CreateVertex()
//-------------------------------------
void Number::CalculateVertex()
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

    vertex_[0].texture_ = { (value_ * 0.1f), 0.0f };
    vertex_[1].texture_ = { (value_ * 0.1f + 0.1f), 0.0f };
    vertex_[2].texture_ = { (value_ * 0.1f), 1.0f };
    vertex_[3].texture_ = { (value_ * 0.1f + 0.1f), 1.0f };
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void Number::SetTexture(
    const std::string &path)
{
    D3DXCreateTextureFromFile(
        DirectX9Holder::device_,
        path.c_str(), &texture_);
}


//-------------------------------------
// end of file
//-------------------------------------
