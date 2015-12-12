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


//-------------------------------------
// enum
//-------------------------------------
typedef enum FORT_STATE{
    ALIVE = 0,
    DEAD,
};


//-------------------------------------
// class
//-------------------------------------
class FortGage : public Object
{
public:
    FortGage(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~FortGage();
    void Init();
    void Update();
    void Draw();
    // 砦のゲージを値で変化させる
    void AddGauge(const float value);
    // 砦のゲージをHPとリンクさせる(max 1.0fで)
    void SetGauge(const float life);
    // HP情報だけ入れる（余り使われない）
    void SetLife(const float life){ life_ = life; }
    float GetLife(void){ return life_;}
    FORT_STATE GetFortState(void){ return state_mode_; }
    void SetTexture(
        const std::string &path);
    void SetTexture(
        const LPDIRECT3DTEXTURE9 texture){texture_ = texture;}
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 texture_;
    float gauge_uv_y_;
    float origin_top_vertex_y_;
    float max_top_vertex_y_;
    float min_top_vertex_y_;
    float life_;
    FORT_STATE state_mode_;
};


#endif //__Fort_H__


//-------------------------------------
// end of file
//-------------------------------------
