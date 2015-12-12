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
    // �Ԃ̃Q�[�W��l�ŕω�������
    void AddGauge(const float value);
    // �Ԃ̃Q�[�W��HP�ƃ����N������(max 1.0f��)
    void SetGauge(const float life);
    // HP��񂾂������i�]��g���Ȃ��j
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
