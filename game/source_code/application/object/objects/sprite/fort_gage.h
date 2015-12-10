//=========================================================
// fort_gage.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FortGage_H__
#define __FortGage_H__


//struct FORT{
//    bool life_flg_;
//    float hp_;
//};
//
//
//typedef struct FORT_MANAGER{
//    FORT *id_;
//    
//};


//-------------------------------------
// class
//-------------------------------------
class FortGage : public Object
{
public:
    FortGage(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~FortGage();
    void Update();
    void Draw();
    // 砦のゲージを値で変化させる
    void AddGauge(const float value);
    // 砦のゲージをHPとリンクさせる(max 1.0fで)
    void SetGauge(const float life);
    void SetTexture(
        const std::string &path);
    void SetTexture(
        const LPDIRECT3DTEXTURE9 texture){texture_ = texture;}
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 texture_;
    float gauge_uv_y_;
    float gauge_position_y_;
    float max_position_y_;
    float min_position_y_;
};


#endif //__Fort_H__


//-------------------------------------
// end of file
//-------------------------------------
