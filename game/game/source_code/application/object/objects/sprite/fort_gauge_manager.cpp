//=========================================================
// FortGaugeManager.cpp
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
#include "../../../resource/texture_manager.h"
#include "fort_gauge_manager.h"


//-------------------------------------
// macro
//-------------------------------------
namespace{
    static const Vector3 kPos[3] = { { SCREEN_WIDTH * 0.5f - 50.0f, 100.0f, 0.0f },
                                     { SCREEN_WIDTH * 0.5f + 50.0f, 100.0f, 0.0f },
                                     { SCREEN_WIDTH * 0.5f , 100.0f, 0.0f } };
}


//-------------------------------------
// FortGaugeManager()
//-------------------------------------
FortGaugeManager::FortGaugeManager(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;

    for (int num = 0; num < 3; num++)
    {
        // テクスチャをセット
        fort_gauge_[num] = new FortGage(parameter);
    }

    for (int num = 0; num < 3; num++)
    {
        Vector3 position = parameter_.position_;
        // 座標をセット
        fort_gauge_[num]->SetPosition(kPos[num]);
        fort_gauge_[num]->Init();
    }

}


//-------------------------------------
// ~FortGaugeManager()
//-------------------------------------
FortGaugeManager::~FortGaugeManager()
{
    for (int num = 0; num < 3; num++)
    {
        SAFE_DELETE(fort_gauge_[num]);
    }
    texture_ = NULL;
}


//-------------------------------------
// Update
//-------------------------------------
void FortGaugeManager::Update()
{
    for (int num = 0; num < 3; num++)
    {
        fort_gauge_[num]->Update();
        // ライフが無くなった時に表示を変える
        if (fort_gauge_[num]->GetFortState() == DEAD){
            texture_ = TextureManager::GetTexture("resource/texture/game/map2.png");
            fort_gauge_[num]->SetTexture(texture_);
        }
    }
}


//-------------------------------------
// Draw
//-------------------------------------
void FortGaugeManager::Draw()
{
    for (int num = 0; num < 3; num++)
    {
        fort_gauge_[num]->Draw();
    }
}

//-------------------------------------
// SetTexture()
//-------------------------------------
void FortGaugeManager::SetTexture(
    const std::string &path)
{
    texture_ = TextureManager::GetTexture(path.c_str());

    if (fort_gauge_ == NULL){ return; }

    for (int num = 0; num < 3; num++)
    {
        // テクスチャをセット
        fort_gauge_[num]->SetTexture(texture_);
    }
}

//-------------------------------------
// SetFortLife()
//-------------------------------------
void FortGaugeManager::SetFortLife(
    const int id , const float life)
{
    if (life > 0.0f)
        fort_gauge_[id]->SetGauge(life);
    else
    {
        fort_gauge_[id]->SetLife(life);
        fort_gauge_[id]->Init();
    }
}

//-------------------------------------
// end of file
//-------------------------------------
