//=========================================================
// water_gage.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __WaterGage_H__
#define __WaterGage_H__


//-------------------------------------
// macro
//-------------------------------------
namespace{
    static const int kMaxGage = 1000;

    static const int kGageDown = 10;

    static const int kGageRegene = 50;
}


//-------------------------------------
// class
//-------------------------------------
class WaterGage : public Object
{
public:
    WaterGage(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~WaterGage();
    void Update();
    void Draw();
    // ゲージの変更割合を入れる
    void SetChangeValue(const float value){ gage_value_ += value; }
    void SetTexture(
        const std::string &path);
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 diffuse_texture_;
    LPDIRECT3DTEXTURE9 alpha_texture_;
    Shader *shader_;
//    LPDIRECT3DTEXTURE9 texture_;
    float gage_value_;
};


#endif //__Watergage_H__


//-------------------------------------
// end of file
//-------------------------------------
