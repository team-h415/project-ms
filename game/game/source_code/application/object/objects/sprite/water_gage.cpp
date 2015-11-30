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
#include "../../object.h"
#include "sprite2d.h"
#include "water_gage.h"


//-------------------------------------
// WaterGage()
//-------------------------------------
WaterGage::WaterGage(
    const OBJECT_PARAMETER_DESC &parameter) :Sprite2D(parameter)
{
    parameter_ = parameter;
    vertex_ = new Vertex2D[4];
    texture_ = NULL;

    gage_volume_ = kMaxGage;
}


//-------------------------------------
// ~WaterGage()
//-------------------------------------
WaterGage::~WaterGage()
{
    SAFE_DELETE_ARRAY(vertex_);
	texture_ = NULL;
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

    vertex_[0].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[1].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[2].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_[3].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vertex_[0].texture_ = { 0.0f, 0.0f };
    vertex_[1].texture_ = { 1.0f, 0.0f };
    vertex_[2].texture_ = { 0.0f, 1.0f };
    vertex_[3].texture_ = { 1.0f, 1.0f };
}

//-------------------------------------
// end of file
//-------------------------------------
