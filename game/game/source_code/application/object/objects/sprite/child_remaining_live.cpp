//=========================================================
// child_remaining_live.cpp
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
#include "../../object.h"
#include "../../objects/sprite/sprite2d.h"
#include "../../../resource/texture_manager.h"
#include "number.h"
#include "child_remaining_live.h"


//-------------------------------------
// ChildRemainingLive()
//-------------------------------------
ChildRemainingLive::ChildRemainingLive(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
    remaining_lives_value_ = 99;
    texture_ = NULL;
    p_number_ = NULL;

    figure_offset_ = 0.0f;
    figure_ = 0;

    p_sprite2d_ = NULL;
}


//-------------------------------------
// ~ChildRemainingLive()
//-------------------------------------
ChildRemainingLive::~ChildRemainingLive()
{
    SAFE_DELETE(p_sprite2d_);
    for (unsigned int num = 0; num < figure_; num++)
    {
        SAFE_DELETE(p_number_[num]);
    }
    SAFE_DELETE_ARRAY(p_number_);
    texture_ = NULL;
}


//-------------------------------------
// Initialize()
//-------------------------------------
void ChildRemainingLive::Initialize()
{
    // 子供残機が分かるデザイン
    OBJECT_PARAMETER_DESC remaining_live_design_param;
    remaining_live_design_param.layer_ = LAYER_SPRITE_2D;
    remaining_live_design_param.position_ = {
        1080.0f,
        624.0f,
        0.0f
    };
    remaining_live_design_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    remaining_live_design_param.scaling_ = { 150.0f, 100.0f, 0.0f };
    p_sprite2d_ = new Sprite2D(remaining_live_design_param);
    p_sprite2d_->SetTexture("resource/texture/game/zanki.png");
}


//-------------------------------------
// Update()
//-------------------------------------
void ChildRemainingLive::Update()
{
    p_sprite2d_->Update();
    this->UpdateNumber();
}


//-------------------------------------
// Draw()
//-------------------------------------
void ChildRemainingLive::Draw()
{
    p_sprite2d_->Draw();
    if (p_number_ == NULL){ return; }

    for (unsigned int num = 0; num < figure_; num++)
    {
        p_number_[num]->Draw();
    }
}


//-------------------------------------
// SetValue()
//-------------------------------------
void ChildRemainingLive::SetValue(int value)
{
    remaining_lives_value_ = value;

    if (p_number_ == NULL){ return; }

    int sub_value = value;
    for (unsigned int num = 0; num < figure_; num++)
    {
        // 特定の桁の値を入れる
        int figure_value = sub_value % 10;

        // 値をセット;
        p_number_[num]->SetValue(figure_value);

        sub_value /= 10;
    }
}

//-------------------------------------
// SetTexture()
//-------------------------------------
void ChildRemainingLive::SetTexture(
    const std::string &path)
{
    texture_ = TextureManager::GetTexture(path.c_str());

    if (p_number_ == NULL){ return; }

    for (unsigned int num = 0; num < figure_; num++)
    {
        // テクスチャをセット
        p_number_[num]->SetTexture(texture_);
    }
}
//-------------------------------------
// SetFigureOffset()
//-------------------------------------
void ChildRemainingLive::SetFigureOffset(float Offset)
{
    figure_offset_ = Offset;

    if (p_number_ == NULL){ return; }

    float right_center_offset =
        parameter_.scaling_.x_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f)
        + figure_offset_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f);


    for (unsigned int num = 0; num < figure_; num++)
    {
        Vector3 pos = parameter_.position_;

        // 座標修正
        pos.x_ += right_center_offset - num * (parameter_.scaling_.x_ + figure_offset_);

        // 座標をセット
        p_number_[num]->SetPosition(pos);
    }

}

//-------------------------------------
// GenerateNumber()
//-------------------------------------
void ChildRemainingLive::GenerateNumber()
{
    // 桁数計算
    figure_ = static_cast<unsigned int>(log10(static_cast<double>(remaining_lives_value_)) + 1);

    if (figure_ == 0){ figure_ = 1; }

    if (p_number_){
        for (unsigned int num = 0; num < figure_; num++){
            SAFE_DELETE(p_number_[num]);
        }
        SAFE_DELETE_ARRAY(p_number_);
    }

    p_number_ = new Number *[figure_];

    float right_center_offset =
        parameter_.scaling_.x_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f)
        + figure_offset_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f);

    int value = remaining_lives_value_;
    for (unsigned int num = 0; num < figure_; num++)
    {
        OBJECT_PARAMETER_DESC param = parameter_;
        // 特定の桁の値を入れる
        int figure_value = value % 10;
        // 座標修正
        param.position_.x_ += right_center_offset - num * (parameter_.scaling_.x_ + figure_offset_);

        // 値をセット
        p_number_[num] = new Number(param, value);
        p_number_[num]->SetValue(figure_value);
        if (texture_ != NULL){
            p_number_[num]->SetTexture(texture_);
        }

        value /= 10;
    }

}


//-------------------------------------
// UpdateNumber()
//-------------------------------------
void ChildRemainingLive::UpdateNumber()
{
    if (p_number_ == NULL){ return; }

    for (unsigned int num = 0; num < figure_; num++)
    {
        p_number_[num]->Update();
    }

}


//-------------------------------------
// ChildDeth()
//-------------------------------------
void ChildRemainingLive::ChildDeth()
{
    if (remaining_lives_value_ > 0){
        --remaining_lives_value_;
        SetValue(remaining_lives_value_);
    }
}


//-------------------------------------
// end of file
//-------------------------------------
